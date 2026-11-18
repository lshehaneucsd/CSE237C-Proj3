open_component dft.comp -reset
add_files [list dft.cpp]
add_files -tb [list dft-top.cpp]
set_top dft
puts "Running: set_top dft"
set_part xczu3eg-sfvc784-2-e
puts "Running: set_part xczu3eg-sfvc784-2-e"
create_clock -period 10
csynth_design

exit