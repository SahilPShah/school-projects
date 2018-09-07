transcript on
if {[file exists rtl_work]} {
	vdel -lib rtl_work -all
}
vlib rtl_work
vmap work rtl_work

vlog -sv -work work +incdir+C:/Users/posid/OneDrive/Documents/ECE\ 385\ LAB/Lab\ 4/Adders\ provided\ code {C:/Users/posid/OneDrive/Documents/ECE 385 LAB/Lab 4/Adders provided code/HexDriver.sv}
vlog -sv -work work +incdir+C:/Users/posid/OneDrive/Documents/ECE\ 385\ LAB/Lab\ 4/Adders\ provided\ code {C:/Users/posid/OneDrive/Documents/ECE 385 LAB/Lab 4/Adders provided code/carry_select_adder.sv}
vlog -sv -work work +incdir+C:/Users/posid/OneDrive/Documents/ECE\ 385\ LAB/Lab\ 4/All\ Adders\ PROJECT {C:/Users/posid/OneDrive/Documents/ECE 385 LAB/Lab 4/All Adders PROJECT/Full_Ripple_Adder.sv}
vlog -sv -work work +incdir+C:/Users/posid/OneDrive/Documents/ECE\ 385\ LAB/Lab\ 4/All\ Adders\ PROJECT {C:/Users/posid/OneDrive/Documents/ECE 385 LAB/Lab 4/All Adders PROJECT/Carry_Select_Adder_4_bit.sv}
vlog -sv -work work +incdir+C:/Users/posid/OneDrive/Documents/ECE\ 385\ LAB/Lab\ 4/Adders\ provided\ code {C:/Users/posid/OneDrive/Documents/ECE 385 LAB/Lab 4/Adders provided code/lab4_adders_toplevel.sv}

vlog -sv -work work +incdir+C:/Users/posid/OneDrive/Documents/ECE\ 385\ LAB/Lab\ 4/All\ Adders\ PROJECT {C:/Users/posid/OneDrive/Documents/ECE 385 LAB/Lab 4/All Adders PROJECT/testbench.sv}

vsim -t 1ps -L altera_ver -L lpm_ver -L sgate_ver -L altera_mf_ver -L altera_lnsim_ver -L cycloneive_ver -L rtl_work -L work -voptargs="+acc"  testbench

add wave *
view structure
view signals
run 200 ns
