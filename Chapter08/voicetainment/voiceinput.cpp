

#include <QDebug>
#include <QThread>

#include "voiceinput.h"

//#include "pocketsphinx.h"
//#include "ps_search.h"
////#include "psphinx_trigger.h"
//#include "kws_search.h"

//#include "err.h"
//#include "ckd_alloc.h"
//#include "strfuncs.h"
//#include "pio.h"
//#include "cmd_ln.h"

extern "C" {
#include <sphinxbase/err.h>
#include <sphinxbase/ad.h>
}


// --- CONSTRUCTOR ---
VoiceInput::VoiceInput(QObject *parent) : QObject(parent) {
    //
}


// --- RUN ---
void VoiceInput::run() {
    // Perform keyword search until registered trigger detected.
    // Detect command, then activate procedure.
    const int32 buffsize = 2048;
    int16 adbuf[buffsize];
    uint8 utt_started, in_speech;
    qint64 k = 0;
    char const* hyp;
    
    cmd_ln_t* config = cmd_ln_init(NULL,    // Load the configuration structure
                                   ps_args(),  //  Passes the default values
                                   TRUE,   
                                   "-hmm", MODELDIR "/en-us/en-us",  // path to the standard english language model
                                   "-lm", MODELDIR "/en-us/en-us.lm.bin",                                         // custom language model (file must be present)
                                   "-dict", 
                                   MODELDIR "/en-us/cmudict-en-us.dict",                                    // custom dictionary (file must be present)
                                   NULL);
    ps_decoder_t *ps = ps_init(config);
    
    state = true;
    
    // PocketSphinx requires the following audio input format:
    // monaural, little-endian, raw 16-bit signed PCM audio at 16000 Hz.
    QAudioFormat format;
    format.setSampleRate(16000);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);
    
    // Check that the audio device supports this format.
    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
    if (!info.isFormatSupported(format)) {
       qWarning() << "Default format not supported, aborting.";
       state = false;
       return;
    }
   
    audioInput = new QAudioInput(format, this);
    audioInput->setBufferSize(buffsize * 2);   
    audioDevice = audioInput->start();
    
    // Set keyword to search for. Keyword is 'computer'.
    //const char* keyfile = "COMPUTER/3.16227766016838e-13/\n";
    const char* keyphrase = "computer";
    if (ps_set_keyphrase(ps, "keyword_search", keyphrase) != 0) {
        return;
    }
    
    if (ps_set_search(ps, "keyword_search") != 0) {
        return;
    }
    
    if (ps_start_utt(ps) < 0) {
        E_FATAL("Failed to start utterance\n");
    }
    
    utt_started = FALSE;
    E_INFO("Ready....\n");
    
    const char* gramfile = "#JSGF V1.0; grammar asr;\
            \
            public <rule> = <action> [<preposition>] [<objects>] [<preposition>] [<objects>];\
            \
            <action> = stop | play | record;\
            \
            <objects> = bluetooth | local | remote | message;\
            \
            <preposition> = from | to;";
    ps_set_jsgf_string(ps, "jsgf", gramfile);

    // Start reading audio samples and process them, first listening for the
    // keyword.
    bool kws = true;
    uint32 samplesReady = 0;
    for (;;) {        
        // Read in the available bytes from the audio device buffer.
        if ((k = audioDevice->read((char*) &adbuf, 4096)) < 0) {
            E_FATAL("Failed to read audio.\n");
        }
        
        samplesReady = k / 2;
        ps_process_raw(ps, adbuf, samplesReady, FALSE, FALSE);
        in_speech = ps_get_in_speech(ps);
        
        if (in_speech && !utt_started) {
            utt_started = TRUE;
            E_INFO("Listening...\n");
        }
        
        if (!in_speech && utt_started) {
            // speech -> silence transition, time to start new utterance.
            // The duration of silence to start a new utterance is 1 second.
            ps_end_utt(ps);
            hyp = ps_get_hyp(ps, nullptr);
            if (hyp != nullptr) {
                // We have a hypothesis.
                
                // Check the hypothesis for a match.
                // if the detected keyword is 'computer' then we switch to the 
                // grammar search mode.
                if (kws && strstr(hyp, "computer") != nullptr) {
                    // We're in keyword search mode, so we switch to the
                    // grammar detection mode to detect the phrases we are
                    // looking for.
                    if (ps_set_search(ps, "jsgf") != 0) {
                        E_FATAL("ERROR: Cannot switch to jsgf mode.\n");
                    }
                    
                    kws = false;
                    E_INFO("Switched to jsgf mode \n");                            
                    E_INFO("Mode: %s\n", ps_get_search(ps));
                }
                else if (!kws) {
                    // We're in grammar detection mode. If we have a hypothesis
                    // we want to perform the requested action before returning
                    // to keyword search mode.
                    if (hyp != nullptr) {
                        // Check each action.
                        if (strncmp(hyp, "play bluetooth", 14) == 0) {
                            emit playBluetooth();
                        }
                        else if (strncmp(hyp, "stop bluetooth", 14) == 0) {
                            emit stopBluetooth();
                        }
                        else if (strncmp(hyp, "play local", 10) == 0) {
                            emit playLocal();
                        }
                        else if (strncmp(hyp, "stop local", 10) == 0) {
                            emit stopLocal();
                        }
                        else if (strncmp(hyp, "play remote", 11) == 0) {
                            emit stopBluetooth();
                        }
                        else if (strncmp(hyp, "stop remote", 11) == 0) {
                            emit stopBluetooth();
                        }
                        else if (strncmp(hyp, "record message", 14) == 0) {
                            emit stopBluetooth();
                        }
                        else if (strncmp(hyp, "play message", 12) == 0) {
                            emit stopBluetooth();
                        }
                    } 
                    else {
                        if (ps_set_search(ps, "keyword_search") != 0){
                            E_FATAL("ERROR: Cannot switch to kws mode.\n");
                        }
                       
                        kws = true;
                        E_INFO("Switched to kws mode.\n");
                    }
                }                
            }

            if (ps_start_utt(ps) < 0) {
                E_FATAL("Failed to start utterance\n");
            }
            
            utt_started = FALSE;
            E_INFO("Ready....\n");
        }
        
        QThread::msleep(100);
    }
    
}
