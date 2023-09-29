# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.24

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.24.3/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.24.3/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/richardchen/work/wasm.contracts/ans.contracts

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/richardchen/work/wasm.contracts/ans.contracts/build

# Utility rule file for contracts_unit_tests.

# Include any custom commands dependencies for this target.
include CMakeFiles/contracts_unit_tests.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/contracts_unit_tests.dir/progress.make

CMakeFiles/contracts_unit_tests: CMakeFiles/contracts_unit_tests-complete

CMakeFiles/contracts_unit_tests-complete: contracts_unit_tests-prefix/src/contracts_unit_tests-stamp/contracts_unit_tests-install
CMakeFiles/contracts_unit_tests-complete: contracts_unit_tests-prefix/src/contracts_unit_tests-stamp/contracts_unit_tests-mkdir
CMakeFiles/contracts_unit_tests-complete: contracts_unit_tests-prefix/src/contracts_unit_tests-stamp/contracts_unit_tests-download
CMakeFiles/contracts_unit_tests-complete: contracts_unit_tests-prefix/src/contracts_unit_tests-stamp/contracts_unit_tests-update
CMakeFiles/contracts_unit_tests-complete: contracts_unit_tests-prefix/src/contracts_unit_tests-stamp/contracts_unit_tests-patch
CMakeFiles/contracts_unit_tests-complete: contracts_unit_tests-prefix/src/contracts_unit_tests-stamp/contracts_unit_tests-configure
CMakeFiles/contracts_unit_tests-complete: contracts_unit_tests-prefix/src/contracts_unit_tests-stamp/contracts_unit_tests-build
CMakeFiles/contracts_unit_tests-complete: contracts_unit_tests-prefix/src/contracts_unit_tests-stamp/contracts_unit_tests-install
CMakeFiles/contracts_unit_tests-complete: contracts_unit_tests-prefix/src/contracts_unit_tests-stamp/contracts_unit_tests-test
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/richardchen/work/wasm.contracts/ans.contracts/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Completed 'contracts_unit_tests'"
	/usr/local/Cellar/cmake/3.24.3/bin/cmake -E make_directory /Users/richardchen/work/wasm.contracts/ans.contracts/build/CMakeFiles
	/usr/local/Cellar/cmake/3.24.3/bin/cmake -E touch /Users/richardchen/work/wasm.contracts/ans.contracts/build/CMakeFiles/contracts_unit_tests-complete
	/usr/local/Cellar/cmake/3.24.3/bin/cmake -E touch /Users/richardchen/work/wasm.contracts/ans.contracts/build/contracts_unit_tests-prefix/src/contracts_unit_tests-stamp/contracts_unit_tests-done

contracts_unit_tests-prefix/src/contracts_unit_tests-stamp/contracts_unit_tests-build: contracts_unit_tests-prefix/src/contracts_unit_tests-stamp/contracts_unit_tests-configure
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/richardchen/work/wasm.contracts/ans.contracts/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Performing build step for 'contracts_unit_tests'"
	cd /Users/richardchen/work/wasm.contracts/ans.contracts/build/tests && $(MAKE)

contracts_unit_tests-prefix/src/contracts_unit_tests-stamp/contracts_unit_tests-configure: contracts_unit_tests-prefix/tmp/contracts_unit_tests-cfgcmd.txt
contracts_unit_tests-prefix/src/contracts_unit_tests-stamp/contracts_unit_tests-configure: contracts_unit_tests-prefix/src/contracts_unit_tests-stamp/contracts_unit_tests-patch
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/richardchen/work/wasm.contracts/ans.contracts/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Performing configure step for 'contracts_unit_tests'"
	cd /Users/richardchen/work/wasm.contracts/ans.contracts/build/tests && /usr/local/Cellar/cmake/3.24.3/bin/cmake -DCMAKE_BUILD_TYPE= -DCMAKE_PREFIX_PATH= -DCMAKE_FRAMEWORK_PATH= -DCMAKE_MODULE_PATH=/usr/local/amax.cdt/lib/cmake/amax.cdt -DAMAX_ROOT= -DLLVM_DIR= -DBOOST_ROOT= "-GUnix Makefiles" /Users/richardchen/work/wasm.contracts/ans.contracts/tests
	cd /Users/richardchen/work/wasm.contracts/ans.contracts/build/tests && /usr/local/Cellar/cmake/3.24.3/bin/cmake -E touch /Users/richardchen/work/wasm.contracts/ans.contracts/build/contracts_unit_tests-prefix/src/contracts_unit_tests-stamp/contracts_unit_tests-configure

contracts_unit_tests-prefix/src/contracts_unit_tests-stamp/contracts_unit_tests-download: contracts_unit_tests-prefix/src/contracts_unit_tests-stamp/contracts_unit_tests-source_dirinfo.txt
contracts_unit_tests-prefix/src/contracts_unit_tests-stamp/contracts_unit_tests-download: contracts_unit_tests-prefix/src/contracts_unit_tests-stamp/contracts_unit_tests-mkdir
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/richardchen/work/wasm.contracts/ans.contracts/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "No download step for 'contracts_unit_tests'"
	/usr/local/Cellar/cmake/3.24.3/bin/cmake -E echo_append
	/usr/local/Cellar/cmake/3.24.3/bin/cmake -E touch /Users/richardchen/work/wasm.contracts/ans.contracts/build/contracts_unit_tests-prefix/src/contracts_unit_tests-stamp/contracts_unit_tests-download

contracts_unit_tests-prefix/src/contracts_unit_tests-stamp/contracts_unit_tests-install: contracts_unit_tests-prefix/src/contracts_unit_tests-stamp/contracts_unit_tests-build
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/richardchen/work/wasm.contracts/ans.contracts/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "No install step for 'contracts_unit_tests'"
	cd /Users/richardchen/work/wasm.contracts/ans.contracts/build/tests && /usr/local/Cellar/cmake/3.24.3/bin/cmake -E echo_append
	cd /Users/richardchen/work/wasm.contracts/ans.contracts/build/tests && /usr/local/Cellar/cmake/3.24.3/bin/cmake -E touch /Users/richardchen/work/wasm.contracts/ans.contracts/build/contracts_unit_tests-prefix/src/contracts_unit_tests-stamp/contracts_unit_tests-install

contracts_unit_tests-prefix/src/contracts_unit_tests-stamp/contracts_unit_tests-mkdir:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/richardchen/work/wasm.contracts/ans.contracts/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Creating directories for 'contracts_unit_tests'"
	/usr/local/Cellar/cmake/3.24.3/bin/cmake -Dcfgdir= -P /Users/richardchen/work/wasm.contracts/ans.contracts/build/contracts_unit_tests-prefix/tmp/contracts_unit_tests-mkdirs.cmake
	/usr/local/Cellar/cmake/3.24.3/bin/cmake -E touch /Users/richardchen/work/wasm.contracts/ans.contracts/build/contracts_unit_tests-prefix/src/contracts_unit_tests-stamp/contracts_unit_tests-mkdir

contracts_unit_tests-prefix/src/contracts_unit_tests-stamp/contracts_unit_tests-patch: contracts_unit_tests-prefix/src/contracts_unit_tests-stamp/contracts_unit_tests-update
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/richardchen/work/wasm.contracts/ans.contracts/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "No patch step for 'contracts_unit_tests'"
	/usr/local/Cellar/cmake/3.24.3/bin/cmake -E echo_append
	/usr/local/Cellar/cmake/3.24.3/bin/cmake -E touch /Users/richardchen/work/wasm.contracts/ans.contracts/build/contracts_unit_tests-prefix/src/contracts_unit_tests-stamp/contracts_unit_tests-patch

contracts_unit_tests-prefix/src/contracts_unit_tests-stamp/contracts_unit_tests-test: contracts_unit_tests-prefix/src/contracts_unit_tests-stamp/contracts_unit_tests-install
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/richardchen/work/wasm.contracts/ans.contracts/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "No test step for 'contracts_unit_tests'"
	cd /Users/richardchen/work/wasm.contracts/ans.contracts/build/tests && /usr/local/Cellar/cmake/3.24.3/bin/cmake -E echo_append
	cd /Users/richardchen/work/wasm.contracts/ans.contracts/build/tests && /usr/local/Cellar/cmake/3.24.3/bin/cmake -E touch /Users/richardchen/work/wasm.contracts/ans.contracts/build/contracts_unit_tests-prefix/src/contracts_unit_tests-stamp/contracts_unit_tests-test

contracts_unit_tests-prefix/src/contracts_unit_tests-stamp/contracts_unit_tests-update: contracts_unit_tests-prefix/src/contracts_unit_tests-stamp/contracts_unit_tests-download
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/richardchen/work/wasm.contracts/ans.contracts/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "No update step for 'contracts_unit_tests'"
	/usr/local/Cellar/cmake/3.24.3/bin/cmake -E echo_append
	/usr/local/Cellar/cmake/3.24.3/bin/cmake -E touch /Users/richardchen/work/wasm.contracts/ans.contracts/build/contracts_unit_tests-prefix/src/contracts_unit_tests-stamp/contracts_unit_tests-update

contracts_unit_tests: CMakeFiles/contracts_unit_tests
contracts_unit_tests: CMakeFiles/contracts_unit_tests-complete
contracts_unit_tests: contracts_unit_tests-prefix/src/contracts_unit_tests-stamp/contracts_unit_tests-build
contracts_unit_tests: contracts_unit_tests-prefix/src/contracts_unit_tests-stamp/contracts_unit_tests-configure
contracts_unit_tests: contracts_unit_tests-prefix/src/contracts_unit_tests-stamp/contracts_unit_tests-download
contracts_unit_tests: contracts_unit_tests-prefix/src/contracts_unit_tests-stamp/contracts_unit_tests-install
contracts_unit_tests: contracts_unit_tests-prefix/src/contracts_unit_tests-stamp/contracts_unit_tests-mkdir
contracts_unit_tests: contracts_unit_tests-prefix/src/contracts_unit_tests-stamp/contracts_unit_tests-patch
contracts_unit_tests: contracts_unit_tests-prefix/src/contracts_unit_tests-stamp/contracts_unit_tests-test
contracts_unit_tests: contracts_unit_tests-prefix/src/contracts_unit_tests-stamp/contracts_unit_tests-update
contracts_unit_tests: CMakeFiles/contracts_unit_tests.dir/build.make
.PHONY : contracts_unit_tests

# Rule to build all files generated by this target.
CMakeFiles/contracts_unit_tests.dir/build: contracts_unit_tests
.PHONY : CMakeFiles/contracts_unit_tests.dir/build

CMakeFiles/contracts_unit_tests.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/contracts_unit_tests.dir/cmake_clean.cmake
.PHONY : CMakeFiles/contracts_unit_tests.dir/clean

CMakeFiles/contracts_unit_tests.dir/depend:
	cd /Users/richardchen/work/wasm.contracts/ans.contracts/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/richardchen/work/wasm.contracts/ans.contracts /Users/richardchen/work/wasm.contracts/ans.contracts /Users/richardchen/work/wasm.contracts/ans.contracts/build /Users/richardchen/work/wasm.contracts/ans.contracts/build /Users/richardchen/work/wasm.contracts/ans.contracts/build/CMakeFiles/contracts_unit_tests.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/contracts_unit_tests.dir/depend

