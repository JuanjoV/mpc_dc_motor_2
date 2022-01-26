# Create project

cd [file dirname [info script]]
create_project -force mpc_dc_motor_2 -part xc7z010clg400-1 mpc_dc_motor_2
set_property BOARD_PART digilentinc.com:zybo:part0:1.0 [current_project]

# Set path to IP repository

set_property IP_REPO_PATHS axi_mpc/ [current_fileset]
update_ip_catalog -rebuild

# Create block diagram

source bd_dc_motor_2.tcl
read_verilog [make_wrapper -top -files [get_files bd_dc_motor_2.bd]]

# Create IP synthesis runs

set bd [get_files bd_dc_motor_2.bd]
generate_target all $bd
export_ip_user_files -of_objects $bd -no_script -sync -force -quiet
create_ip_run $bd

# Configure synthesis

set runs [get_runs *synth_1]
set_property -name "flow" -value "Vivado Synthesis 2021" -objects $runs
set_property -name "strategy" -value "Flow_PerfOptimized_high" -objects $runs
set_property -name "steps.synth_design.args.fsm_extraction" -value "one_hot" -objects $runs
set_property -name "steps.synth_design.args.keep_equivalent_registers" -value "1" -objects $runs
set_property -name "steps.synth_design.args.resource_sharing" -value "off" -objects $runs
set_property -name "steps.synth_design.args.no_lc" -value "1" -objects $runs
set_property -name "steps.synth_design.args.shreg_min_size" -value "5" -objects $runs
current_run -synthesis [get_runs synth_1]

# Configure implementation

set_property -name "flow" -value "Vivado Implementation 2021" -objects [get_runs impl_1]
set_property -name "strategy" -value "Performance_ExploreWithRemap" -objects [get_runs impl_1]
set_property -name "steps.opt_design.args.directive" -value "ExploreWithRemap" -objects [get_runs impl_1]
set_property -name "steps.place_design.args.directive" -value "Explore" -objects [get_runs impl_1]
set_property -name "steps.phys_opt_design.is_enabled" -value "1" -objects [get_runs impl_1]
set_property -name "steps.phys_opt_design.args.directive" -value "Explore" -objects [get_runs impl_1]
set_property -name "steps.route_design.args.directive" -value "NoTimingRelaxation" -objects [get_runs impl_1]
set_property -name "steps.route_design.args.more options" -value "-tns_cleanup" -objects [get_runs impl_1]
set_property -name "steps.post_route_phys_opt_design.is_enabled" -value "1" -objects [get_runs impl_1]
set_property -name "steps.post_route_phys_opt_design.args.directive" -value "Explore" -objects [get_runs impl_1]
set_property -name "steps.write_bitstream.args.readback_file" -value "0" -objects [get_runs impl_1]
set_property -name "steps.write_bitstream.args.verbose" -value "0" -objects [get_runs impl_1]
current_run -implementation [get_runs impl_1]
