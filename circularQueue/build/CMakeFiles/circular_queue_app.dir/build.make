# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/isbozeng/Downloads/wwl_code_test/circularQueue

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/isbozeng/Downloads/wwl_code_test/circularQueue/build

# Include any dependencies generated for this target.
include CMakeFiles/circular_queue_app.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/circular_queue_app.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/circular_queue_app.dir/flags.make

CMakeFiles/circular_queue_app.dir/main.c.o: CMakeFiles/circular_queue_app.dir/flags.make
CMakeFiles/circular_queue_app.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/isbozeng/Downloads/wwl_code_test/circularQueue/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/circular_queue_app.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/circular_queue_app.dir/main.c.o   -c /home/isbozeng/Downloads/wwl_code_test/circularQueue/main.c

CMakeFiles/circular_queue_app.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/circular_queue_app.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/isbozeng/Downloads/wwl_code_test/circularQueue/main.c > CMakeFiles/circular_queue_app.dir/main.c.i

CMakeFiles/circular_queue_app.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/circular_queue_app.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/isbozeng/Downloads/wwl_code_test/circularQueue/main.c -o CMakeFiles/circular_queue_app.dir/main.c.s

CMakeFiles/circular_queue_app.dir/circular_queue.c.o: CMakeFiles/circular_queue_app.dir/flags.make
CMakeFiles/circular_queue_app.dir/circular_queue.c.o: ../circular_queue.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/isbozeng/Downloads/wwl_code_test/circularQueue/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/circular_queue_app.dir/circular_queue.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/circular_queue_app.dir/circular_queue.c.o   -c /home/isbozeng/Downloads/wwl_code_test/circularQueue/circular_queue.c

CMakeFiles/circular_queue_app.dir/circular_queue.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/circular_queue_app.dir/circular_queue.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/isbozeng/Downloads/wwl_code_test/circularQueue/circular_queue.c > CMakeFiles/circular_queue_app.dir/circular_queue.c.i

CMakeFiles/circular_queue_app.dir/circular_queue.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/circular_queue_app.dir/circular_queue.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/isbozeng/Downloads/wwl_code_test/circularQueue/circular_queue.c -o CMakeFiles/circular_queue_app.dir/circular_queue.c.s

# Object files for target circular_queue_app
circular_queue_app_OBJECTS = \
"CMakeFiles/circular_queue_app.dir/main.c.o" \
"CMakeFiles/circular_queue_app.dir/circular_queue.c.o"

# External object files for target circular_queue_app
circular_queue_app_EXTERNAL_OBJECTS =

circular_queue_app: CMakeFiles/circular_queue_app.dir/main.c.o
circular_queue_app: CMakeFiles/circular_queue_app.dir/circular_queue.c.o
circular_queue_app: CMakeFiles/circular_queue_app.dir/build.make
circular_queue_app: CMakeFiles/circular_queue_app.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/isbozeng/Downloads/wwl_code_test/circularQueue/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable circular_queue_app"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/circular_queue_app.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/circular_queue_app.dir/build: circular_queue_app

.PHONY : CMakeFiles/circular_queue_app.dir/build

CMakeFiles/circular_queue_app.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/circular_queue_app.dir/cmake_clean.cmake
.PHONY : CMakeFiles/circular_queue_app.dir/clean

CMakeFiles/circular_queue_app.dir/depend:
	cd /home/isbozeng/Downloads/wwl_code_test/circularQueue/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/isbozeng/Downloads/wwl_code_test/circularQueue /home/isbozeng/Downloads/wwl_code_test/circularQueue /home/isbozeng/Downloads/wwl_code_test/circularQueue/build /home/isbozeng/Downloads/wwl_code_test/circularQueue/build /home/isbozeng/Downloads/wwl_code_test/circularQueue/build/CMakeFiles/circular_queue_app.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/circular_queue_app.dir/depend

