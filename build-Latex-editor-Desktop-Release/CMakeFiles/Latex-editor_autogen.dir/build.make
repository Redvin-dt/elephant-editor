# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/home/pavel/HSE project/main_project_folder/Latex-editor"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/pavel/HSE project/main_project_folder/build-Latex-editor-Desktop-Release"

# Utility rule file for Latex-editor_autogen.

# Include any custom commands dependencies for this target.
include CMakeFiles/Latex-editor_autogen.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Latex-editor_autogen.dir/progress.make

CMakeFiles/Latex-editor_autogen:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir="/home/pavel/HSE project/main_project_folder/build-Latex-editor-Desktop-Release/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Automatic MOC and UIC for target Latex-editor"
	/usr/bin/cmake -E cmake_autogen "/home/pavel/HSE project/main_project_folder/build-Latex-editor-Desktop-Release/CMakeFiles/Latex-editor_autogen.dir/AutogenInfo.json" Release

Latex-editor_autogen: CMakeFiles/Latex-editor_autogen
Latex-editor_autogen: CMakeFiles/Latex-editor_autogen.dir/build.make
.PHONY : Latex-editor_autogen

# Rule to build all files generated by this target.
CMakeFiles/Latex-editor_autogen.dir/build: Latex-editor_autogen
.PHONY : CMakeFiles/Latex-editor_autogen.dir/build

CMakeFiles/Latex-editor_autogen.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Latex-editor_autogen.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Latex-editor_autogen.dir/clean

CMakeFiles/Latex-editor_autogen.dir/depend:
	cd "/home/pavel/HSE project/main_project_folder/build-Latex-editor-Desktop-Release" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/pavel/HSE project/main_project_folder/Latex-editor" "/home/pavel/HSE project/main_project_folder/Latex-editor" "/home/pavel/HSE project/main_project_folder/build-Latex-editor-Desktop-Release" "/home/pavel/HSE project/main_project_folder/build-Latex-editor-Desktop-Release" "/home/pavel/HSE project/main_project_folder/build-Latex-editor-Desktop-Release/CMakeFiles/Latex-editor_autogen.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/Latex-editor_autogen.dir/depend

