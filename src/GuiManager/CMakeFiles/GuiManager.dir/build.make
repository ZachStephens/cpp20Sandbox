# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /snap/cmake/619/bin/cmake

# The command to remove a file.
RM = /snap/cmake/619/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/zach/work/basic0/cpp_starter_project/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/zach/work/basic0/cpp_starter_project/src

# Include any dependencies generated for this target.
include GuiManager/CMakeFiles/GuiManager.dir/depend.make

# Include the progress variables for this target.
include GuiManager/CMakeFiles/GuiManager.dir/progress.make

# Include the compile flags for this target's objects.
include GuiManager/CMakeFiles/GuiManager.dir/flags.make

GuiManager/CMakeFiles/GuiManager.dir/GuiManager.o: GuiManager/CMakeFiles/GuiManager.dir/flags.make
GuiManager/CMakeFiles/GuiManager.dir/GuiManager.o: GuiManager/GuiManager.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zach/work/basic0/cpp_starter_project/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object GuiManager/CMakeFiles/GuiManager.dir/GuiManager.o"
	cd /home/zach/work/basic0/cpp_starter_project/src/GuiManager && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/GuiManager.dir/GuiManager.o -c /home/zach/work/basic0/cpp_starter_project/src/GuiManager/GuiManager.cpp

GuiManager/CMakeFiles/GuiManager.dir/GuiManager.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/GuiManager.dir/GuiManager.i"
	cd /home/zach/work/basic0/cpp_starter_project/src/GuiManager && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zach/work/basic0/cpp_starter_project/src/GuiManager/GuiManager.cpp > CMakeFiles/GuiManager.dir/GuiManager.i

GuiManager/CMakeFiles/GuiManager.dir/GuiManager.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/GuiManager.dir/GuiManager.s"
	cd /home/zach/work/basic0/cpp_starter_project/src/GuiManager && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zach/work/basic0/cpp_starter_project/src/GuiManager/GuiManager.cpp -o CMakeFiles/GuiManager.dir/GuiManager.s

# Object files for target GuiManager
GuiManager_OBJECTS = \
"CMakeFiles/GuiManager.dir/GuiManager.o"

# External object files for target GuiManager
GuiManager_EXTERNAL_OBJECTS =

GuiManager/libGuiManager.a: GuiManager/CMakeFiles/GuiManager.dir/GuiManager.o
GuiManager/libGuiManager.a: GuiManager/CMakeFiles/GuiManager.dir/build.make
GuiManager/libGuiManager.a: GuiManager/CMakeFiles/GuiManager.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/zach/work/basic0/cpp_starter_project/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libGuiManager.a"
	cd /home/zach/work/basic0/cpp_starter_project/src/GuiManager && $(CMAKE_COMMAND) -P CMakeFiles/GuiManager.dir/cmake_clean_target.cmake
	cd /home/zach/work/basic0/cpp_starter_project/src/GuiManager && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/GuiManager.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
GuiManager/CMakeFiles/GuiManager.dir/build: GuiManager/libGuiManager.a

.PHONY : GuiManager/CMakeFiles/GuiManager.dir/build

GuiManager/CMakeFiles/GuiManager.dir/clean:
	cd /home/zach/work/basic0/cpp_starter_project/src/GuiManager && $(CMAKE_COMMAND) -P CMakeFiles/GuiManager.dir/cmake_clean.cmake
.PHONY : GuiManager/CMakeFiles/GuiManager.dir/clean

GuiManager/CMakeFiles/GuiManager.dir/depend:
	cd /home/zach/work/basic0/cpp_starter_project/src && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/zach/work/basic0/cpp_starter_project/src /home/zach/work/basic0/cpp_starter_project/src/GuiManager /home/zach/work/basic0/cpp_starter_project/src /home/zach/work/basic0/cpp_starter_project/src/GuiManager /home/zach/work/basic0/cpp_starter_project/src/GuiManager/CMakeFiles/GuiManager.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : GuiManager/CMakeFiles/GuiManager.dir/depend

