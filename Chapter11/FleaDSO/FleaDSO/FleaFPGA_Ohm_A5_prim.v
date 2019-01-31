// Verilog netlist produced by program LSE :  version Diamond (64-bit) 3.9.1.119
// Netlist written on Sat Feb 17 21:16:49 2018
//
// Verilog Description of module FleaFPGA_Ohm_A5
//

module FleaFPGA_Ohm_A5 (sys_clock, sys_reset, n_led1, LVDS_Red, LVDS_Green, 
            LVDS_Blue, LVDS_ck, slave_tx_o, slave_rx_i, slave_cts_i, 
            Dram_Clk, Dram_CKE, Dram_n_Ras, Dram_n_Cas, Dram_n_We, 
            Dram_BA, Dram_Addr, Dram_Data, Dram_n_cs, Dram_DQMH, Dram_DQML, 
            GPIO_2, GPIO_3, GPIO_4, GPIO_6, GPIO_7, GPIO_8, GPIO_9, 
            GPIO_10, GPIO_11, GPIO_12, GPIO_13, GPIO_14, GPIO_15, 
            GPIO_16, GPIO_17, GPIO_18, GPIO_19, GPIO_20, GPIO_21, 
            GPIO_22, GPIO_23, GPIO_24, GPIO_25, GPIO_26, GPIO_27, 
            GPIO_IDSD, GPIO_IDSC, ADC0_input, ADC0_error, mmc_dat1, 
            mmc_dat2, mmc_n_cs, mmc_clk, mmc_mosi, mmc_miso, PS2_enable, 
            PS2_clk1, PS2_data1, PS2_clk2, PS2_data2);   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(19[8:23])
    input sys_clock;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(23[2:11])
    input sys_reset;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(24[2:11])
    output n_led1;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(27[2:8])
    output [0:0]LVDS_Red;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(30[2:10])
    output [0:0]LVDS_Green;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(31[2:12])
    output [0:0]LVDS_Blue;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(32[2:11])
    output [0:0]LVDS_ck;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(33[2:9])
    output slave_tx_o;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(36[2:12])
    input slave_rx_i;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(37[2:12])
    input slave_cts_i;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(38[2:13])
    output Dram_Clk;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(41[2:10])
    output Dram_CKE;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(42[2:10])
    output Dram_n_Ras;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(43[2:12])
    output Dram_n_Cas;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(44[2:12])
    output Dram_n_We;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(45[2:11])
    output [1:0]Dram_BA;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(46[2:9])
    output [12:0]Dram_Addr;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(47[2:11])
    input [15:0]Dram_Data;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(48[2:11])
    output Dram_n_cs;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(49[2:11])
    output Dram_DQMH;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(51[2:11])
    output Dram_DQML;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(52[2:11])
    input GPIO_2 /* synthesis .original_dir=IN_OUT */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(55[2:8])
    input GPIO_3 /* synthesis .original_dir=IN_OUT */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(56[2:8])
    input GPIO_4 /* synthesis .original_dir=IN_OUT */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(57[2:8])
    input GPIO_6 /* synthesis .original_dir=IN_OUT */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(59[2:8])
    input GPIO_7 /* synthesis .original_dir=IN_OUT */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(60[2:8])
    input GPIO_8 /* synthesis .original_dir=IN_OUT */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(61[2:8])
    input GPIO_9 /* synthesis .original_dir=IN_OUT */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(62[2:8])
    input GPIO_10 /* synthesis .original_dir=IN_OUT */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(63[2:9])
    input GPIO_11 /* synthesis .original_dir=IN_OUT */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(64[2:9])
    input GPIO_12 /* synthesis .original_dir=IN_OUT */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(65[2:9])
    input GPIO_13 /* synthesis .original_dir=IN_OUT */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(66[2:9])
    input GPIO_14 /* synthesis .original_dir=IN_OUT */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(67[2:9])
    input GPIO_15 /* synthesis .original_dir=IN_OUT */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(68[2:9])
    input GPIO_16 /* synthesis .original_dir=IN_OUT */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(69[2:9])
    input GPIO_17 /* synthesis .original_dir=IN_OUT */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(70[2:9])
    input GPIO_18 /* synthesis .original_dir=IN_OUT */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(71[2:9])
    input GPIO_19 /* synthesis .original_dir=IN_OUT */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(72[2:9])
    input GPIO_20;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(73[2:9])
    input GPIO_21;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(74[2:9])
    input GPIO_22 /* synthesis .original_dir=IN_OUT */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(75[2:9])
    input GPIO_23 /* synthesis .original_dir=IN_OUT */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(76[2:9])
    input GPIO_24 /* synthesis .original_dir=IN_OUT */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(77[2:9])
    input GPIO_25 /* synthesis .original_dir=IN_OUT */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(78[2:9])
    input GPIO_26 /* synthesis .original_dir=IN_OUT */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(79[2:9])
    input GPIO_27 /* synthesis .original_dir=IN_OUT */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(80[2:9])
    input GPIO_IDSD /* synthesis .original_dir=IN_OUT */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(81[2:11])
    input GPIO_IDSC /* synthesis .original_dir=IN_OUT */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(82[2:11])
    input ADC0_input;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(86[2:12])
    output ADC0_error;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(87[2:12])
    input mmc_dat1;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(96[2:10])
    input mmc_dat2;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(97[2:10])
    output mmc_n_cs;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(98[2:10])
    output mmc_clk;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(99[2:9])
    output mmc_mosi;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(100[2:10])
    input mmc_miso;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(101[2:10])
    output PS2_enable;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(104[2:12])
    input PS2_clk1 /* synthesis .original_dir=IN_OUT */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(105[2:10])
    input PS2_data1 /* synthesis .original_dir=IN_OUT */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(106[2:11])
    input PS2_clk2 /* synthesis .original_dir=IN_OUT */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(108[2:10])
    input PS2_data2 /* synthesis .original_dir=IN_OUT */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(109[2:11])
    
    wire VCC_net /* synthesis is_clock=1 */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(98[2:10])
    wire sys_clock_c /* synthesis is_clock=1 */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(23[2:11])
    wire clk_dvi /* synthesis is_clock=1, SET_AS_NETWORK=clk_dvi */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(115[11:18])
    
    wire GND_net, sys_reset_c, GPIO_20_c, ADC0_error_c, buf_douto0, 
        buf_douto0_adj_250, buf_douto0_adj_251, buf_douto0_adj_252;
    
    VHI i2 (.Z(VCC_net));
    OB Dram_n_Ras_pad (.I(GND_net), .O(Dram_n_Ras));   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(43[2:12])
    OB ADC0_error_pad (.I(ADC0_error_c), .O(ADC0_error));   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(87[2:12])
    OB Dram_DQML_pad (.I(GND_net), .O(Dram_DQML));   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(52[2:11])
    OB Dram_DQMH_pad (.I(GND_net), .O(Dram_DQMH));   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(51[2:11])
    OB PS2_enable_pad (.I(VCC_net), .O(PS2_enable));   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(104[2:12])
    IB sys_clock_pad (.I(sys_clock), .O(sys_clock_c));   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(23[2:11])
    OB mmc_clk_pad (.I(GND_net), .O(mmc_clk));   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(99[2:9])
    OB mmc_n_cs_pad (.I(VCC_net), .O(mmc_n_cs));   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(98[2:10])
    OB mmc_mosi_pad (.I(GND_net), .O(mmc_mosi));   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(100[2:10])
    OB Dram_n_cs_pad (.I(VCC_net), .O(Dram_n_cs));   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(49[2:11])
    OB Dram_Addr_pad_0 (.I(GND_net), .O(Dram_Addr[0]));   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(47[2:11])
    OB Dram_Addr_pad_1 (.I(GND_net), .O(Dram_Addr[1]));   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(47[2:11])
    OB Dram_Addr_pad_2 (.I(GND_net), .O(Dram_Addr[2]));   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(47[2:11])
    OB Dram_Addr_pad_3 (.I(GND_net), .O(Dram_Addr[3]));   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(47[2:11])
    OB Dram_Addr_pad_4 (.I(GND_net), .O(Dram_Addr[4]));   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(47[2:11])
    VLO i1 (.Z(GND_net));
    OB Dram_Addr_pad_5 (.I(GND_net), .O(Dram_Addr[5]));   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(47[2:11])
    OB Dram_CKE_pad (.I(GND_net), .O(Dram_CKE));   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(42[2:10])
    dvid u100 (.clk_dvi(clk_dvi), .GND_net(GND_net), .buf_douto0(buf_douto0_adj_252), 
         .buf_douto0_adj_1(buf_douto0_adj_251), .buf_douto0_adj_2(buf_douto0_adj_250), 
         .buf_douto0_adj_3(buf_douto0));   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(176[9:25])
    OB Dram_Addr_pad_6 (.I(GND_net), .O(Dram_Addr[6]));   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(47[2:11])
    OB Dram_Addr_pad_7 (.I(GND_net), .O(Dram_Addr[7]));   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(47[2:11])
    OB Dram_Addr_pad_8 (.I(GND_net), .O(Dram_Addr[8]));   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(47[2:11])
    OB Dram_Addr_pad_9 (.I(GND_net), .O(Dram_Addr[9]));   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(47[2:11])
    OB Dram_Addr_pad_10 (.I(GND_net), .O(Dram_Addr[10]));   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(47[2:11])
    OB Dram_Addr_pad_11 (.I(GND_net), .O(Dram_Addr[11]));   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(47[2:11])
    IB GPIO_20_pad (.I(GPIO_20), .O(GPIO_20_c));   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(73[2:9])
    IB sys_reset_pad (.I(sys_reset), .O(sys_reset_c));   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(24[2:11])
    OB Inst1_OB0 (.I(buf_douto0), .O(LVDS_Red[0])) /* synthesis syn_black_box=true, IO_TYPE="LVCMOS25", syn_instantiated=1, LSE_LINE_FILE_ID=26, LSE_LCOL=7, LSE_RCOL=26, LSE_LLINE=82, LSE_RLINE=82 */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/ddr_out/ddr_out.vhd(59[16:18])
    FleaFPGA_DSO user_module1 (.ADC0_error_c(ADC0_error_c), .sys_reset_c(sys_reset_c));   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(143[17:41])
    OB Inst1_OB0_adj_5 (.I(buf_douto0_adj_250), .O(LVDS_Green[0])) /* synthesis syn_black_box=true, IO_TYPE="LVCMOS25", syn_instantiated=1, LSE_LINE_FILE_ID=26, LSE_LCOL=7, LSE_RCOL=26, LSE_LLINE=86, LSE_RLINE=86 */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/ddr_out/ddr_out.vhd(59[16:18])
    DVI_clkgen u0 (.sys_clock_c(sys_clock_c), .clk_dvi(clk_dvi), .GND_net(GND_net)) /* synthesis NGD_DRC_MASK=1 */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(168[7:29])
    OB Dram_Addr_pad_12 (.I(GND_net), .O(Dram_Addr[12]));   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(47[2:11])
    OB Inst1_OB0_adj_6 (.I(buf_douto0_adj_251), .O(LVDS_Blue[0])) /* synthesis syn_black_box=true, IO_TYPE="LVCMOS25", syn_instantiated=1, LSE_LINE_FILE_ID=26, LSE_LCOL=7, LSE_RCOL=26, LSE_LLINE=90, LSE_RLINE=90 */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/ddr_out/ddr_out.vhd(59[16:18])
    PUR PUR_INST (.PUR(VCC_net));
    defparam PUR_INST.RST_PULSE = 1;
    OB Inst1_OB0_adj_7 (.I(buf_douto0_adj_252), .O(LVDS_ck[0])) /* synthesis syn_black_box=true, IO_TYPE="LVCMOS25", syn_instantiated=1, LSE_LINE_FILE_ID=26, LSE_LCOL=7, LSE_RCOL=26, LSE_LLINE=94, LSE_RLINE=94 */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/ddr_out/ddr_out.vhd(59[16:18])
    OB Dram_Clk_pad (.I(GND_net), .O(Dram_Clk));   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(41[2:10])
    OB Dram_n_Cas_pad (.I(GND_net), .O(Dram_n_Cas));   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(44[2:12])
    OB slave_tx_o_pad (.I(GND_net), .O(slave_tx_o));   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(36[2:12])
    OB n_led1_pad (.I(VCC_net), .O(n_led1));   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(27[2:8])
    GSR GSR_INST (.GSR(VCC_net));
    OB Dram_BA_pad_0 (.I(GND_net), .O(Dram_BA[0]));   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(46[2:9])
    OB Dram_BA_pad_1 (.I(GND_net), .O(Dram_BA[1]));   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(46[2:9])
    OB Dram_n_We_pad (.I(GND_net), .O(Dram_n_We));   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(45[2:11])
    
endmodule
//
// Verilog Description of module dvid
//

module dvid (clk_dvi, GND_net, buf_douto0, buf_douto0_adj_1, buf_douto0_adj_2, 
            buf_douto0_adj_3);
    input clk_dvi;
    input GND_net;
    output buf_douto0;
    output buf_douto0_adj_1;
    output buf_douto0_adj_2;
    output buf_douto0_adj_3;
    
    wire clk_dvi /* synthesis is_clock=1, SET_AS_NETWORK=clk_dvi */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(115[11:18])
    wire [9:0]shift_red;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/dvi_d.vhd(44[11:20])
    wire [9:0]shift_green_9__N_148;
    wire [9:0]shift_clock;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/dvi_d.vhd(46[11:22])
    
    wire n397, n393, n416;
    wire [9:0]shift_blue;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/dvi_d.vhd(44[39:49])
    
    FD1S3JX shift_red_i6 (.D(shift_red[9]), .CK(clk_dvi), .PD(shift_green_9__N_148[9]), 
            .Q(shift_red[6])) /* synthesis lse_init_val=0, LSE_LINE_FILE_ID=20, LSE_LCOL=9, LSE_RCOL=25, LSE_LLINE=176, LSE_RLINE=176 */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/dvi_d.vhd(110[7] 121[14])
    defparam shift_red_i6.GSR = "ENABLED";
    FD1S3IX shift_red_i5 (.D(shift_red[7]), .CK(clk_dvi), .CD(shift_green_9__N_148[9]), 
            .Q(shift_red[5])) /* synthesis lse_init_val=0, LSE_LINE_FILE_ID=20, LSE_LCOL=9, LSE_RCOL=25, LSE_LLINE=176, LSE_RLINE=176 */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/dvi_d.vhd(110[7] 121[14])
    defparam shift_red_i5.GSR = "ENABLED";
    FD1S3IX shift_red_i0 (.D(shift_red[2]), .CK(clk_dvi), .CD(shift_green_9__N_148[9]), 
            .Q(shift_red[0])) /* synthesis lse_init_val=0, LSE_LINE_FILE_ID=20, LSE_LCOL=9, LSE_RCOL=25, LSE_LLINE=176, LSE_RLINE=176 */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/dvi_d.vhd(110[7] 121[14])
    defparam shift_red_i0.GSR = "ENABLED";
    FD1S3JX shift_red_i4 (.D(shift_red[6]), .CK(clk_dvi), .PD(shift_green_9__N_148[9]), 
            .Q(shift_red[4])) /* synthesis lse_init_val=0, LSE_LINE_FILE_ID=20, LSE_LCOL=9, LSE_RCOL=25, LSE_LLINE=176, LSE_RLINE=176 */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/dvi_d.vhd(110[7] 121[14])
    defparam shift_red_i4.GSR = "ENABLED";
    FD1S3AY shift_clock_i0 (.D(shift_clock[2]), .CK(clk_dvi), .Q(shift_clock[0])) /* synthesis lse_init_val=1, LSE_LINE_FILE_ID=20, LSE_LCOL=9, LSE_RCOL=25, LSE_LLINE=176, LSE_RLINE=176 */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/dvi_d.vhd(110[7] 121[14])
    defparam shift_clock_i0.GSR = "ENABLED";
    LUT4 i1_3_lut (.A(shift_clock[9]), .B(shift_clock[6]), .C(shift_clock[8]), 
         .Z(n397)) /* synthesis lut_function=(A+(B+(C))) */ ;
    defparam i1_3_lut.init = 16'hfefe;
    FD1S3AY shift_clock_i1 (.D(shift_clock[3]), .CK(clk_dvi), .Q(shift_clock[1])) /* synthesis lse_init_val=1, LSE_LINE_FILE_ID=20, LSE_LCOL=9, LSE_RCOL=25, LSE_LLINE=176, LSE_RLINE=176 */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/dvi_d.vhd(110[7] 121[14])
    defparam shift_clock_i1.GSR = "ENABLED";
    FD1S3AY shift_clock_i2 (.D(shift_clock[4]), .CK(clk_dvi), .Q(shift_clock[2])) /* synthesis lse_init_val=1, LSE_LINE_FILE_ID=20, LSE_LCOL=9, LSE_RCOL=25, LSE_LLINE=176, LSE_RLINE=176 */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/dvi_d.vhd(110[7] 121[14])
    defparam shift_clock_i2.GSR = "ENABLED";
    FD1S3AY shift_clock_i3 (.D(shift_clock[5]), .CK(clk_dvi), .Q(shift_clock[3])) /* synthesis lse_init_val=1, LSE_LINE_FILE_ID=20, LSE_LCOL=9, LSE_RCOL=25, LSE_LLINE=176, LSE_RLINE=176 */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/dvi_d.vhd(110[7] 121[14])
    defparam shift_clock_i3.GSR = "ENABLED";
    FD1S3AY shift_clock_i4 (.D(shift_clock[6]), .CK(clk_dvi), .Q(shift_clock[4])) /* synthesis lse_init_val=1, LSE_LINE_FILE_ID=20, LSE_LCOL=9, LSE_RCOL=25, LSE_LLINE=176, LSE_RLINE=176 */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/dvi_d.vhd(110[7] 121[14])
    defparam shift_clock_i4.GSR = "ENABLED";
    FD1S3AX shift_clock_i5 (.D(shift_clock[7]), .CK(clk_dvi), .Q(shift_clock[5])) /* synthesis lse_init_val=0, LSE_LINE_FILE_ID=20, LSE_LCOL=9, LSE_RCOL=25, LSE_LLINE=176, LSE_RLINE=176 */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/dvi_d.vhd(110[7] 121[14])
    defparam shift_clock_i5.GSR = "ENABLED";
    FD1S3AX shift_clock_i6 (.D(shift_clock[8]), .CK(clk_dvi), .Q(shift_clock[6])) /* synthesis lse_init_val=0, LSE_LINE_FILE_ID=20, LSE_LCOL=9, LSE_RCOL=25, LSE_LLINE=176, LSE_RLINE=176 */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/dvi_d.vhd(110[7] 121[14])
    defparam shift_clock_i6.GSR = "ENABLED";
    FD1S3AX shift_clock_i7 (.D(shift_clock[9]), .CK(clk_dvi), .Q(shift_clock[7])) /* synthesis lse_init_val=0, LSE_LINE_FILE_ID=20, LSE_LCOL=9, LSE_RCOL=25, LSE_LLINE=176, LSE_RLINE=176 */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/dvi_d.vhd(110[7] 121[14])
    defparam shift_clock_i7.GSR = "ENABLED";
    FD1S3AX shift_clock_i8 (.D(shift_clock[0]), .CK(clk_dvi), .Q(shift_clock[8])) /* synthesis lse_init_val=0, LSE_LINE_FILE_ID=20, LSE_LCOL=9, LSE_RCOL=25, LSE_LLINE=176, LSE_RLINE=176 */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/dvi_d.vhd(110[7] 121[14])
    defparam shift_clock_i8.GSR = "ENABLED";
    FD1S3AX shift_clock_i9 (.D(shift_clock[1]), .CK(clk_dvi), .Q(shift_clock[9])) /* synthesis lse_init_val=0, LSE_LINE_FILE_ID=20, LSE_LCOL=9, LSE_RCOL=25, LSE_LLINE=176, LSE_RLINE=176 */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/dvi_d.vhd(110[7] 121[14])
    defparam shift_clock_i9.GSR = "ENABLED";
    FD1S3IX shift_red_i3 (.D(shift_red[5]), .CK(clk_dvi), .CD(shift_green_9__N_148[9]), 
            .Q(shift_red[3])) /* synthesis lse_init_val=0, LSE_LINE_FILE_ID=20, LSE_LCOL=9, LSE_RCOL=25, LSE_LLINE=176, LSE_RLINE=176 */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/dvi_d.vhd(110[7] 121[14])
    defparam shift_red_i3.GSR = "ENABLED";
    FD1S3JX shift_red_i2 (.D(shift_red[4]), .CK(clk_dvi), .PD(shift_green_9__N_148[9]), 
            .Q(shift_red[2])) /* synthesis lse_init_val=0, LSE_LINE_FILE_ID=20, LSE_LCOL=9, LSE_RCOL=25, LSE_LLINE=176, LSE_RLINE=176 */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/dvi_d.vhd(110[7] 121[14])
    defparam shift_red_i2.GSR = "ENABLED";
    FD1S3IX shift_red_i1 (.D(shift_red[3]), .CK(clk_dvi), .CD(shift_green_9__N_148[9]), 
            .Q(shift_red[1])) /* synthesis lse_init_val=0, LSE_LINE_FILE_ID=20, LSE_LCOL=9, LSE_RCOL=25, LSE_LLINE=176, LSE_RLINE=176 */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/dvi_d.vhd(110[7] 121[14])
    defparam shift_red_i1.GSR = "ENABLED";
    FD1S3AX shift_red_i9 (.D(shift_green_9__N_148[9]), .CK(clk_dvi), .Q(shift_red[9])) /* synthesis lse_init_val=0, LSE_LINE_FILE_ID=20, LSE_LCOL=9, LSE_RCOL=25, LSE_LLINE=176, LSE_RLINE=176 */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/dvi_d.vhd(110[7] 121[14])
    defparam shift_red_i9.GSR = "ENABLED";
    LUT4 i1_2_lut (.A(shift_clock[5]), .B(shift_clock[7]), .Z(n393)) /* synthesis lut_function=(A+(B)) */ ;
    defparam i1_2_lut.init = 16'heeee;
    LUT4 i299_4_lut (.A(shift_clock[1]), .B(shift_clock[2]), .C(shift_clock[3]), 
         .D(shift_clock[4]), .Z(n416)) /* synthesis lut_function=(A (B (C (D)))) */ ;
    defparam i299_4_lut.init = 16'h8000;
    FD1S3JX shift_blue_i1 (.D(shift_red[2]), .CK(clk_dvi), .PD(shift_green_9__N_148[9]), 
            .Q(shift_blue[1])) /* synthesis lse_init_val=0, LSE_LINE_FILE_ID=20, LSE_LCOL=9, LSE_RCOL=25, LSE_LLINE=176, LSE_RLINE=176 */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/dvi_d.vhd(110[7] 121[14])
    defparam shift_blue_i1.GSR = "ENABLED";
    FD1S3IX shift_red_i7 (.D(shift_red[9]), .CK(clk_dvi), .CD(shift_green_9__N_148[9]), 
            .Q(shift_red[7])) /* synthesis lse_init_val=0, LSE_LINE_FILE_ID=20, LSE_LCOL=9, LSE_RCOL=25, LSE_LLINE=176, LSE_RLINE=176 */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/dvi_d.vhd(110[7] 121[14])
    defparam shift_red_i7.GSR = "ENABLED";
    LUT4 i310_4_lut (.A(n416), .B(n397), .C(shift_clock[0]), .D(n393), 
         .Z(shift_green_9__N_148[9])) /* synthesis lut_function=(!((B+((D)+!C))+!A)) */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/dvi_d.vhd(111[10] 119[17])
    defparam i310_4_lut.init = 16'h0020;
    ddr_out u5 (.clk_dvi(clk_dvi), .GND_net(GND_net), .\shift_clock[1] (shift_clock[1]), 
            .\shift_clock[0] (shift_clock[0]), .buf_douto0(buf_douto0)) /* synthesis NGD_DRC_MASK=1 */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/dvi_d.vhd(94[7:26])
    ddr_out_U0 u4 (.clk_dvi(clk_dvi), .GND_net(GND_net), .\shift_blue[1] (shift_blue[1]), 
            .\shift_red[9] (shift_red[9]), .buf_douto0(buf_douto0_adj_1)) /* synthesis NGD_DRC_MASK=1 */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/dvi_d.vhd(90[7:26])
    ddr_out_U1 u3 (.clk_dvi(clk_dvi), .GND_net(GND_net), .\shift_red[1] (shift_red[1]), 
            .\shift_red[0] (shift_red[0]), .buf_douto0(buf_douto0_adj_2)) /* synthesis NGD_DRC_MASK=1 */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/dvi_d.vhd(86[7:26])
    ddr_out_U2 u2 (.clk_dvi(clk_dvi), .GND_net(GND_net), .\shift_red[1] (shift_red[1]), 
            .\shift_red[0] (shift_red[0]), .buf_douto0(buf_douto0_adj_3)) /* synthesis NGD_DRC_MASK=1 */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/dvi_d.vhd(82[7:26])
    
endmodule
//
// Verilog Description of module ddr_out
//

module ddr_out (clk_dvi, GND_net, \shift_clock[1] , \shift_clock[0] , 
            buf_douto0) /* synthesis NGD_DRC_MASK=1 */ ;
    input clk_dvi;
    input GND_net;
    input \shift_clock[1] ;
    input \shift_clock[0] ;
    output buf_douto0;
    
    wire clk_dvi /* synthesis is_clock=1, SET_AS_NETWORK=clk_dvi */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(115[11:18])
    
    ODDRX1F Inst4_ODDRX1F0 (.D0(\shift_clock[0] ), .D1(\shift_clock[1] ), 
            .SCLK(clk_dvi), .RST(GND_net), .Q(buf_douto0)) /* synthesis syn_black_box=true, syn_instantiated=1, LSE_LINE_FILE_ID=26, LSE_LCOL=7, LSE_RCOL=26, LSE_LLINE=94, LSE_RLINE=94 */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/dvi_d.vhd(94[7:26])
    defparam Inst4_ODDRX1F0.GSR = "ENABLED";
    
endmodule
//
// Verilog Description of module ddr_out_U0
//

module ddr_out_U0 (clk_dvi, GND_net, \shift_blue[1] , \shift_red[9] , 
            buf_douto0) /* synthesis NGD_DRC_MASK=1 */ ;
    input clk_dvi;
    input GND_net;
    input \shift_blue[1] ;
    input \shift_red[9] ;
    output buf_douto0;
    
    wire clk_dvi /* synthesis is_clock=1, SET_AS_NETWORK=clk_dvi */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(115[11:18])
    
    ODDRX1F Inst4_ODDRX1F0 (.D0(\shift_red[9] ), .D1(\shift_blue[1] ), .SCLK(clk_dvi), 
            .RST(GND_net), .Q(buf_douto0)) /* synthesis syn_black_box=true, syn_instantiated=1, LSE_LINE_FILE_ID=26, LSE_LCOL=7, LSE_RCOL=26, LSE_LLINE=90, LSE_RLINE=90 */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/dvi_d.vhd(90[7:26])
    defparam Inst4_ODDRX1F0.GSR = "ENABLED";
    
endmodule
//
// Verilog Description of module ddr_out_U1
//

module ddr_out_U1 (clk_dvi, GND_net, \shift_red[1] , \shift_red[0] , 
            buf_douto0) /* synthesis NGD_DRC_MASK=1 */ ;
    input clk_dvi;
    input GND_net;
    input \shift_red[1] ;
    input \shift_red[0] ;
    output buf_douto0;
    
    wire clk_dvi /* synthesis is_clock=1, SET_AS_NETWORK=clk_dvi */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(115[11:18])
    
    ODDRX1F Inst4_ODDRX1F0 (.D0(\shift_red[0] ), .D1(\shift_red[1] ), .SCLK(clk_dvi), 
            .RST(GND_net), .Q(buf_douto0)) /* synthesis syn_black_box=true, syn_instantiated=1, LSE_LINE_FILE_ID=26, LSE_LCOL=7, LSE_RCOL=26, LSE_LLINE=86, LSE_RLINE=86 */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/dvi_d.vhd(86[7:26])
    defparam Inst4_ODDRX1F0.GSR = "ENABLED";
    
endmodule
//
// Verilog Description of module ddr_out_U2
//

module ddr_out_U2 (clk_dvi, GND_net, \shift_red[1] , \shift_red[0] , 
            buf_douto0) /* synthesis NGD_DRC_MASK=1 */ ;
    input clk_dvi;
    input GND_net;
    input \shift_red[1] ;
    input \shift_red[0] ;
    output buf_douto0;
    
    wire clk_dvi /* synthesis is_clock=1, SET_AS_NETWORK=clk_dvi */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(115[11:18])
    
    ODDRX1F Inst4_ODDRX1F0 (.D0(\shift_red[0] ), .D1(\shift_red[1] ), .SCLK(clk_dvi), 
            .RST(GND_net), .Q(buf_douto0)) /* synthesis syn_black_box=true, syn_instantiated=1, LSE_LINE_FILE_ID=26, LSE_LCOL=7, LSE_RCOL=26, LSE_LLINE=82, LSE_RLINE=82 */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/dvi_d.vhd(82[7:26])
    defparam Inst4_ODDRX1F0.GSR = "ENABLED";
    
endmodule
//
// Verilog Description of module FleaFPGA_DSO
//

module FleaFPGA_DSO (ADC0_error_c, sys_reset_c);
    output ADC0_error_c;
    input sys_reset_c;
    
    
    LUT4 i312_3_lut (.A(ADC0_error_c), .B(sys_reset_c), .Z(ADC0_error_c)) /* synthesis lut_function=(!((B)+!A)) */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_dso.vhd(158[3] 211[14])
    defparam i312_3_lut.init = 16'h2222;
    
endmodule
//
// Verilog Description of module DVI_clkgen
//

module DVI_clkgen (sys_clock_c, clk_dvi, GND_net) /* synthesis NGD_DRC_MASK=1 */ ;
    input sys_clock_c;
    output clk_dvi;
    input GND_net;
    
    wire sys_clock_c /* synthesis is_clock=1 */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(23[2:11])
    wire clk_dvi /* synthesis is_clock=1, SET_AS_NETWORK=clk_dvi */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(115[11:18])
    
    EHXPLLL PLLInst_0 (.CLKI(sys_clock_c), .CLKFB(clk_dvi), .PHASESEL0(GND_net), 
            .PHASESEL1(GND_net), .PHASEDIR(GND_net), .PHASESTEP(GND_net), 
            .PHASELOADREG(GND_net), .STDBY(GND_net), .PLLWAKESYNC(GND_net), 
            .RST(GND_net), .ENCLKOP(GND_net), .ENCLKOS(GND_net), .ENCLKOS2(GND_net), 
            .ENCLKOS3(GND_net), .CLKOP(clk_dvi)) /* synthesis syn_black_box=true, FREQUENCY_PIN_CLKOS2="25.000000", FREQUENCY_PIN_CLKOS="125.000000", FREQUENCY_PIN_CLKOP="125.000000", FREQUENCY_PIN_CLKI="25.000000", ICP_CURRENT="12", LPF_RESISTOR="16", syn_instantiated=1, LSE_LINE_FILE_ID=20, LSE_LCOL=7, LSE_RCOL=29, LSE_LLINE=168, LSE_RLINE=168 */ ;   // c:/lscc/diamond/3.9_x64/examples/fleadso/source/fleafpga_2v5_dso_toplevel.vhd(168[7:29])
    defparam PLLInst_0.CLKI_DIV = 1;
    defparam PLLInst_0.CLKFB_DIV = 5;
    defparam PLLInst_0.CLKOP_DIV = 5;
    defparam PLLInst_0.CLKOS_DIV = 5;
    defparam PLLInst_0.CLKOS2_DIV = 25;
    defparam PLLInst_0.CLKOS3_DIV = 1;
    defparam PLLInst_0.CLKOP_ENABLE = "ENABLED";
    defparam PLLInst_0.CLKOS_ENABLE = "ENABLED";
    defparam PLLInst_0.CLKOS2_ENABLE = "ENABLED";
    defparam PLLInst_0.CLKOS3_ENABLE = "DISABLED";
    defparam PLLInst_0.CLKOP_CPHASE = 4;
    defparam PLLInst_0.CLKOS_CPHASE = 5;
    defparam PLLInst_0.CLKOS2_CPHASE = 24;
    defparam PLLInst_0.CLKOS3_CPHASE = 0;
    defparam PLLInst_0.CLKOP_FPHASE = 0;
    defparam PLLInst_0.CLKOS_FPHASE = 2;
    defparam PLLInst_0.CLKOS2_FPHASE = 0;
    defparam PLLInst_0.CLKOS3_FPHASE = 0;
    defparam PLLInst_0.FEEDBK_PATH = "CLKOP";
    defparam PLLInst_0.CLKOP_TRIM_POL = "FALLING";
    defparam PLLInst_0.CLKOP_TRIM_DELAY = 0;
    defparam PLLInst_0.CLKOS_TRIM_POL = "FALLING";
    defparam PLLInst_0.CLKOS_TRIM_DELAY = 0;
    defparam PLLInst_0.OUTDIVIDER_MUXA = "DIVA";
    defparam PLLInst_0.OUTDIVIDER_MUXB = "DIVB";
    defparam PLLInst_0.OUTDIVIDER_MUXC = "DIVC";
    defparam PLLInst_0.OUTDIVIDER_MUXD = "DIVD";
    defparam PLLInst_0.PLL_LOCK_MODE = 0;
    defparam PLLInst_0.PLL_LOCK_DELAY = 200;
    defparam PLLInst_0.STDBY_ENABLE = "DISABLED";
    defparam PLLInst_0.REFIN_RESET = "DISABLED";
    defparam PLLInst_0.SYNC_ENABLE = "DISABLED";
    defparam PLLInst_0.INT_LOCK_STICKY = "ENABLED";
    defparam PLLInst_0.DPHASE_SOURCE = "DISABLED";
    defparam PLLInst_0.PLLRST_ENA = "DISABLED";
    defparam PLLInst_0.INTFB_WAKE = "DISABLED";
    
endmodule
//
// Verilog Description of module PUR
// module not written out since it is a black-box. 
//

