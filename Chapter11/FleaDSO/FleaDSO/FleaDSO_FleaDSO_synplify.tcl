#-- Lattice Semiconductor Corporation Ltd.
#-- Synplify OEM project file

#device options
set_option -technology ECP5U
set_option -part LFE5U_25F
set_option -package BG381C
set_option -speed_grade -6

#compilation/mapping options
set_option -symbolic_fsm_compiler false
set_option -resource_sharing true

#use verilog 2001 standard option
set_option -vlog_std v2001

#map options
set_option -frequency auto
set_option -maxfan 1000
set_option -auto_constrain_io 0
set_option -disable_io_insertion false
set_option -retiming true; set_option -pipe true
set_option -force_gsr false
set_option -compiler_compatible 0
set_option -dup false

set_option -default_enum_encoding onehot

#simulation options


#timing analysis options



#automatic place and route (vendor) options
set_option -write_apr_constraint 1

#synplifyPro options
set_option -fix_gated_and_generated_clocks 1
set_option -update_models_cp 0
set_option -resolve_multiple_driver 0


#-- add_file options
add_file -vhdl {C:/lscc/diamond/3.9_x64/cae_library/synthesis/vhdl/ecp5u.vhd}
add_file -vhdl -lib "work" {C:/lscc/diamond/3.9_x64/examples/FleaDSO/source/FleaFPGA_2v5_DSO_toplevel.vhd}
add_file -vhdl -lib "work" {C:/lscc/diamond/3.9_x64/examples/FleaDSO/source/FleaFPGA_DSO.vhd}
add_file -vhdl -lib "work" {C:/lscc/diamond/3.9_x64/examples/FleaDSO/source/Simple_VGA_CRTC.vhd}
add_file -vhdl -lib "work" {C:/lscc/diamond/3.9_x64/examples/FleaDSO/ddr_out/ddr_out.vhd}
add_file -vhdl -lib "work" {C:/lscc/diamond/3.9_x64/examples/FleaDSO/DVI_clkgen/DVI_clkgen.vhd}
add_file -vhdl -lib "work" {C:/lscc/diamond/3.9_x64/examples/FleaDSO/source/TDMS_encoder.vhd}
add_file -vhdl -lib "work" {C:/lscc/diamond/3.9_x64/examples/FleaDSO/source/DVI_D.vhd}
add_file -vhdl -lib "work" {C:/lscc/diamond/3.9_x64/examples/FleaDSO/DSO_RAMBUFFER_CH1/DSO_RAMBUFFER_CH1.vhd}
add_file -vhdl -lib "work" {C:/lscc/diamond/3.9_x64/examples/FleaDSO/ADC_PLL/ADC_PLL.vhd}
add_file -vhdl -lib "work" {C:/lscc/diamond/3.9_x64/examples/FleaDSO/source/VGA.vhd}
add_file -vhdl -lib "work" {C:/lscc/diamond/3.9_x64/examples/FleaDSO/source/simple_uart.vhd}

#-- top module name
set_option -top_module FleaFPGA_Ohm_A5

#-- set result format/file last
project -result_file {C:/lscc/diamond/3.9_x64/examples/FleaDSO/FleaDSO/FleaDSO_FleaDSO.edi}

#-- error message log file
project -log_file {FleaDSO_FleaDSO.srf}

#-- set any command lines input by customer


#-- run Synplify with 'arrange HDL file'
project -run hdl_info_gen -fileorder
project -run
