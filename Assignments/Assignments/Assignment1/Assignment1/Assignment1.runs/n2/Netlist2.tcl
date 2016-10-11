proc start_step { step } {
  set stopFile ".stop.rst"
  if {[file isfile .stop.rst]} {
    puts ""
    puts "*** Halting run - EA reset detected ***"
    puts ""
    puts ""
    return -code error
  }
  set beginFile ".$step.begin.rst"
  set platform "$::tcl_platform(platform)"
  set user "$::tcl_platform(user)"
  set pid [pid]
  set host ""
  if { [string equal $platform unix] } {
    if { [info exist ::env(HOSTNAME)] } {
      set host $::env(HOSTNAME)
    }
  } else {
    if { [info exist ::env(COMPUTERNAME)] } {
      set host $::env(COMPUTERNAME)
    }
  }
  set ch [open $beginFile w]
  puts $ch "<?xml version=\"1.0\"?>"
  puts $ch "<ProcessHandle Version=\"1\" Minor=\"0\">"
  puts $ch "    <Process Command=\".planAhead.\" Owner=\"$user\" Host=\"$host\" Pid=\"$pid\">"
  puts $ch "    </Process>"
  puts $ch "</ProcessHandle>"
  close $ch
}

proc end_step { step } {
  set endFile ".$step.end.rst"
  set ch [open $endFile w]
  close $ch
}

proc step_failed { step } {
  set endFile ".$step.error.rst"
  set ch [open $endFile w]
  close $ch
}

set_msg_config -id {HDL 9-1061} -limit 100000
set_msg_config -id {HDL 9-1654} -limit 100000
set_msg_config -id {Synth 8-256} -limit 10000
set_msg_config -id {Synth 8-638} -limit 10000

start_step init_design
set rc [catch {
  create_msg_db init_design.pb
  set_property design_mode GateLvl [current_fileset]
  set_param project.singleFileAddWarning.threshold 0
  set_property webtalk.parent_dir {C:/Users/Schifman Ben/Documents/UofA/Senior 2016-2017/Fall 2016/574 ECE/ECE574/Assignments/Assignment1/Assignment1/Assignment1.cache/wt} [current_project]
  set_property parent.project_path {C:/Users/Schifman Ben/Documents/UofA/Senior 2016-2017/Fall 2016/574 ECE/ECE574/Assignments/Assignment1/Assignment1/Assignment1.xpr} [current_project]
  set_property ip_repo_paths {{c:/Users/Schifman Ben/Documents/UofA/Senior 2016-2017/Fall 2016/574 ECE/ECE574/Assignments/Assignment1/Assignment1/Assignment1.cache/ip}} [current_project]
  set_property ip_output_repo {{c:/Users/Schifman Ben/Documents/UofA/Senior 2016-2017/Fall 2016/574 ECE/ECE574/Assignments/Assignment1/Assignment1/Assignment1.cache/ip}} [current_project]
  add_files -quiet {{C:/Users/Schifman Ben/Documents/UofA/Senior 2016-2017/Fall 2016/574 ECE/ECE574/Assignments/Assignment1/Assignment1/Assignment1.runs/netlist2/Netlist2.dcp}}
  link_design -top Netlist2 -part xc7a100tcsg324-1
  write_hwdef -file Netlist2.hwdef
  close_msg_db -file init_design.pb
} RESULT]
if {$rc} {
  step_failed init_design
  return -code error $RESULT
} else {
  end_step init_design
}

start_step opt_design
set rc [catch {
  create_msg_db opt_design.pb
  opt_design 
  write_checkpoint -force Netlist2_opt.dcp
  report_drc -file Netlist2_drc_opted.rpt
  close_msg_db -file opt_design.pb
} RESULT]
if {$rc} {
  step_failed opt_design
  return -code error $RESULT
} else {
  end_step opt_design
}

start_step place_design
set rc [catch {
  create_msg_db place_design.pb
  implement_debug_core 
  place_design 
  write_checkpoint -force Netlist2_placed.dcp
  report_io -file Netlist2_io_placed.rpt
  report_utilization -file Netlist2_utilization_placed.rpt -pb Netlist2_utilization_placed.pb
  report_control_sets -verbose -file Netlist2_control_sets_placed.rpt
  close_msg_db -file place_design.pb
} RESULT]
if {$rc} {
  step_failed place_design
  return -code error $RESULT
} else {
  end_step place_design
}

start_step route_design
set rc [catch {
  create_msg_db route_design.pb
  route_design 
  write_checkpoint -force Netlist2_routed.dcp
  report_drc -file Netlist2_drc_routed.rpt -pb Netlist2_drc_routed.pb
  report_timing_summary -warn_on_violation -max_paths 10 -file Netlist2_timing_summary_routed.rpt -rpx Netlist2_timing_summary_routed.rpx
  report_power -file Netlist2_power_routed.rpt -pb Netlist2_power_summary_routed.pb -rpx Netlist2_power_routed.rpx
  report_route_status -file Netlist2_route_status.rpt -pb Netlist2_route_status.pb
  report_clock_utilization -file Netlist2_clock_utilization_routed.rpt
  close_msg_db -file route_design.pb
} RESULT]
if {$rc} {
  step_failed route_design
  return -code error $RESULT
} else {
  end_step route_design
}

