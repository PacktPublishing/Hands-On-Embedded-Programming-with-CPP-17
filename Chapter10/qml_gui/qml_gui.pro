TEMPLATE = app

QT += 3dextras
CONFIG += resources_big

QT += 3dcore 3dquick qml quick multimedia

SOURCES += \
    main.cpp \
    interface.cpp

HEADERS += \
    interface.h

OTHER_FILES += \
    *.qml

RESOURCES += \
	qml_gui.qrc
