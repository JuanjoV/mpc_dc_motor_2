############################################################
## This file is generated automatically by Vitis HLS.
## Please DO NOT edit it.
## Copyright 1986-2021 Xilinx, Inc. All Rights Reserved.
############################################################
open_project -reset mpc_dc_motor_2
set_top hls_main
add_files src/autogen/init_dc_motor_2.cpp -cflags "-std=c++14"
add_files src/hls_generic_dense.cpp -cflags "-std=c++14"
add_files -tb src/tb_generic_dense.cpp -cflags "-std=c++14 -Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
add_files -tb src/autogen/cosim_dc_motor_2.cpp -cflags "-std=c++14 -Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
open_solution "solution1" -flow_target vivado
set_part {xc7z010clg400-1}
create_clock -period 10 -name default
config_export -format ip_catalog -output ../vivado/axi_mpc.zip -rtl verilog

#csim_design
#csynth_design
#cosim_design

