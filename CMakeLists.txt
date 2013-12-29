cmake_minimum_required (VERSION 2.6)
set(CMAKE_C_FLAGS "-std=c11 -g -Wall -Wextra")
project (autodeathd)

add_executable(
	add 
	main.c)

target_link_libraries(add udev)
