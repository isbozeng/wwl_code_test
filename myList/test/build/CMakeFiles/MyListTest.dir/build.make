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
CMAKE_SOURCE_DIR = /home/isbozeng/Downloads/wwl_code_test/myList/test

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/isbozeng/Downloads/wwl_code_test/myList/test/build

# Include any dependencies generated for this target.
include CMakeFiles/MyListTest.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/MyListTest.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/MyListTest.dir/flags.make

CMakeFiles/MyListTest.dir/myListTest.cpp.o: CMakeFiles/MyListTest.dir/flags.make
CMakeFiles/MyListTest.dir/myListTest.cpp.o: ../myListTest.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/isbozeng/Downloads/wwl_code_test/myList/test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/MyListTest.dir/myListTest.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MyListTest.dir/myListTest.cpp.o -c /home/isbozeng/Downloads/wwl_code_test/myList/test/myListTest.cpp

CMakeFiles/MyListTest.dir/myListTest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MyListTest.dir/myListTest.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/isbozeng/Downloads/wwl_code_test/myList/test/myListTest.cpp > CMakeFiles/MyListTest.dir/myListTest.cpp.i

CMakeFiles/MyListTest.dir/myListTest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MyListTest.dir/myListTest.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/isbozeng/Downloads/wwl_code_test/myList/test/myListTest.cpp -o CMakeFiles/MyListTest.dir/myListTest.cpp.s

# Object files for target MyListTest
MyListTest_OBJECTS = \
"CMakeFiles/MyListTest.dir/myListTest.cpp.o"

# External object files for target MyListTest
MyListTest_EXTERNAL_OBJECTS =

MyListTest: CMakeFiles/MyListTest.dir/myListTest.cpp.o
MyListTest: CMakeFiles/MyListTest.dir/build.make
MyListTest: /usr/lib/x86_64-linux-gnu/libgtest.a
MyListTest: CMakeFiles/MyListTest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/isbozeng/Downloads/wwl_code_test/myList/test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable MyListTest"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/MyListTest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/MyListTest.dir/build: MyListTest

.PHONY : CMakeFiles/MyListTest.dir/build

CMakeFiles/MyListTest.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/MyListTest.dir/cmake_clean.cmake
.PHONY : CMakeFiles/MyListTest.dir/clean

CMakeFiles/MyListTest.dir/depend:
	cd /home/isbozeng/Downloads/wwl_code_test/myList/test/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/isbozeng/Downloads/wwl_code_test/myList/test /home/isbozeng/Downloads/wwl_code_test/myList/test /home/isbozeng/Downloads/wwl_code_test/myList/test/build /home/isbozeng/Downloads/wwl_code_test/myList/test/build /home/isbozeng/Downloads/wwl_code_test/myList/test/build/CMakeFiles/MyListTest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/MyListTest.dir/depend

