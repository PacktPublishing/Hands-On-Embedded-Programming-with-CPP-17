
#Begin clock constraint
define_clock -name {DVI_clkgen|CLKOS3_inferred_clock} {n:DVI_clkgen|CLKOS3_inferred_clock} -period 3.333 -clockgroup Autoconstr_clkgroup_0 -rise 0.000 -fall 1.666 -route 0.000 
#End clock constraint

#Begin clock constraint
define_clock -name {DVI_clkgen|CLKOS2_inferred_clock} {n:DVI_clkgen|CLKOS2_inferred_clock} -period 3.352 -clockgroup Autoconstr_clkgroup_1 -rise 0.000 -fall 1.676 -route 0.000 
#End clock constraint

#Begin clock constraint
define_clock -name {DVI_clkgen|CLKOP_inferred_clock} {n:DVI_clkgen|CLKOP_inferred_clock} -period 2.219 -clockgroup Autoconstr_clkgroup_2 -rise 0.000 -fall 1.110 -route 0.000 
#End clock constraint

#Begin clock constraint
define_clock -name {ADC_PLL|CLKOP_inferred_clock} {n:ADC_PLL|CLKOP_inferred_clock} -period 4.532 -clockgroup Autoconstr_clkgroup_3 -rise 0.000 -fall 2.266 -route 0.000 
#End clock constraint

#Begin clock constraint
define_clock -name {ADC_PLL|CLKOS_inferred_clock} {n:ADC_PLL|CLKOS_inferred_clock} -period 1000.000 -clockgroup Autoconstr_clkgroup_4 -rise 0.000 -fall 500.000 -route 0.000 
#End clock constraint

#Begin clock constraint
define_clock -name {vga_controller|v_sync_derived_clock} {n:vga_controller|v_sync_derived_clock} -period 1000.000 -clockgroup Autoconstr_clkgroup_4 -rise 0.000 -fall 500.000 -route 0.000 
#End clock constraint
