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
CMAKE_SOURCE_DIR = /home/andrew/programming/graphics

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/andrew/programming/graphics/build

# Include any dependencies generated for this target.
include CMakeFiles/p1.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/p1.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/p1.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/p1.dir/flags.make

CMakeFiles/p1.dir/misc05_picking/p1_source.cpp.o: CMakeFiles/p1.dir/flags.make
CMakeFiles/p1.dir/misc05_picking/p1_source.cpp.o: ../misc05_picking/p1_source.cpp
CMakeFiles/p1.dir/misc05_picking/p1_source.cpp.o: CMakeFiles/p1.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/andrew/programming/graphics/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/p1.dir/misc05_picking/p1_source.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/p1.dir/misc05_picking/p1_source.cpp.o -MF CMakeFiles/p1.dir/misc05_picking/p1_source.cpp.o.d -o CMakeFiles/p1.dir/misc05_picking/p1_source.cpp.o -c /home/andrew/programming/graphics/misc05_picking/p1_source.cpp

CMakeFiles/p1.dir/misc05_picking/p1_source.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/p1.dir/misc05_picking/p1_source.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/andrew/programming/graphics/misc05_picking/p1_source.cpp > CMakeFiles/p1.dir/misc05_picking/p1_source.cpp.i

CMakeFiles/p1.dir/misc05_picking/p1_source.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/p1.dir/misc05_picking/p1_source.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/andrew/programming/graphics/misc05_picking/p1_source.cpp -o CMakeFiles/p1.dir/misc05_picking/p1_source.cpp.s

CMakeFiles/p1.dir/common/shader.cpp.o: CMakeFiles/p1.dir/flags.make
CMakeFiles/p1.dir/common/shader.cpp.o: ../common/shader.cpp
CMakeFiles/p1.dir/common/shader.cpp.o: CMakeFiles/p1.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/andrew/programming/graphics/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/p1.dir/common/shader.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/p1.dir/common/shader.cpp.o -MF CMakeFiles/p1.dir/common/shader.cpp.o.d -o CMakeFiles/p1.dir/common/shader.cpp.o -c /home/andrew/programming/graphics/common/shader.cpp

CMakeFiles/p1.dir/common/shader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/p1.dir/common/shader.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/andrew/programming/graphics/common/shader.cpp > CMakeFiles/p1.dir/common/shader.cpp.i

CMakeFiles/p1.dir/common/shader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/p1.dir/common/shader.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/andrew/programming/graphics/common/shader.cpp -o CMakeFiles/p1.dir/common/shader.cpp.s

CMakeFiles/p1.dir/common/controls.cpp.o: CMakeFiles/p1.dir/flags.make
CMakeFiles/p1.dir/common/controls.cpp.o: ../common/controls.cpp
CMakeFiles/p1.dir/common/controls.cpp.o: CMakeFiles/p1.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/andrew/programming/graphics/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/p1.dir/common/controls.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/p1.dir/common/controls.cpp.o -MF CMakeFiles/p1.dir/common/controls.cpp.o.d -o CMakeFiles/p1.dir/common/controls.cpp.o -c /home/andrew/programming/graphics/common/controls.cpp

CMakeFiles/p1.dir/common/controls.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/p1.dir/common/controls.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/andrew/programming/graphics/common/controls.cpp > CMakeFiles/p1.dir/common/controls.cpp.i

CMakeFiles/p1.dir/common/controls.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/p1.dir/common/controls.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/andrew/programming/graphics/common/controls.cpp -o CMakeFiles/p1.dir/common/controls.cpp.s

CMakeFiles/p1.dir/common/texture.cpp.o: CMakeFiles/p1.dir/flags.make
CMakeFiles/p1.dir/common/texture.cpp.o: ../common/texture.cpp
CMakeFiles/p1.dir/common/texture.cpp.o: CMakeFiles/p1.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/andrew/programming/graphics/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/p1.dir/common/texture.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/p1.dir/common/texture.cpp.o -MF CMakeFiles/p1.dir/common/texture.cpp.o.d -o CMakeFiles/p1.dir/common/texture.cpp.o -c /home/andrew/programming/graphics/common/texture.cpp

CMakeFiles/p1.dir/common/texture.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/p1.dir/common/texture.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/andrew/programming/graphics/common/texture.cpp > CMakeFiles/p1.dir/common/texture.cpp.i

CMakeFiles/p1.dir/common/texture.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/p1.dir/common/texture.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/andrew/programming/graphics/common/texture.cpp -o CMakeFiles/p1.dir/common/texture.cpp.s

CMakeFiles/p1.dir/common/objloader.cpp.o: CMakeFiles/p1.dir/flags.make
CMakeFiles/p1.dir/common/objloader.cpp.o: ../common/objloader.cpp
CMakeFiles/p1.dir/common/objloader.cpp.o: CMakeFiles/p1.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/andrew/programming/graphics/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/p1.dir/common/objloader.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/p1.dir/common/objloader.cpp.o -MF CMakeFiles/p1.dir/common/objloader.cpp.o.d -o CMakeFiles/p1.dir/common/objloader.cpp.o -c /home/andrew/programming/graphics/common/objloader.cpp

CMakeFiles/p1.dir/common/objloader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/p1.dir/common/objloader.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/andrew/programming/graphics/common/objloader.cpp > CMakeFiles/p1.dir/common/objloader.cpp.i

CMakeFiles/p1.dir/common/objloader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/p1.dir/common/objloader.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/andrew/programming/graphics/common/objloader.cpp -o CMakeFiles/p1.dir/common/objloader.cpp.s

CMakeFiles/p1.dir/common/vboindexer.cpp.o: CMakeFiles/p1.dir/flags.make
CMakeFiles/p1.dir/common/vboindexer.cpp.o: ../common/vboindexer.cpp
CMakeFiles/p1.dir/common/vboindexer.cpp.o: CMakeFiles/p1.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/andrew/programming/graphics/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/p1.dir/common/vboindexer.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/p1.dir/common/vboindexer.cpp.o -MF CMakeFiles/p1.dir/common/vboindexer.cpp.o.d -o CMakeFiles/p1.dir/common/vboindexer.cpp.o -c /home/andrew/programming/graphics/common/vboindexer.cpp

CMakeFiles/p1.dir/common/vboindexer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/p1.dir/common/vboindexer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/andrew/programming/graphics/common/vboindexer.cpp > CMakeFiles/p1.dir/common/vboindexer.cpp.i

CMakeFiles/p1.dir/common/vboindexer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/p1.dir/common/vboindexer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/andrew/programming/graphics/common/vboindexer.cpp -o CMakeFiles/p1.dir/common/vboindexer.cpp.s

# Object files for target p1
p1_OBJECTS = \
"CMakeFiles/p1.dir/misc05_picking/p1_source.cpp.o" \
"CMakeFiles/p1.dir/common/shader.cpp.o" \
"CMakeFiles/p1.dir/common/controls.cpp.o" \
"CMakeFiles/p1.dir/common/texture.cpp.o" \
"CMakeFiles/p1.dir/common/objloader.cpp.o" \
"CMakeFiles/p1.dir/common/vboindexer.cpp.o"

# External object files for target p1
p1_EXTERNAL_OBJECTS =

p1: CMakeFiles/p1.dir/misc05_picking/p1_source.cpp.o
p1: CMakeFiles/p1.dir/common/shader.cpp.o
p1: CMakeFiles/p1.dir/common/controls.cpp.o
p1: CMakeFiles/p1.dir/common/texture.cpp.o
p1: CMakeFiles/p1.dir/common/objloader.cpp.o
p1: CMakeFiles/p1.dir/common/vboindexer.cpp.o
p1: CMakeFiles/p1.dir/build.make
p1: /usr/lib/x86_64-linux-gnu/libGL.so
p1: /usr/lib/x86_64-linux-gnu/libGLU.so
p1: external/glfw-3.1.2/src/libglfw3.a
p1: external/libGLEW_1130.a
p1: external/libANTTWEAKBAR_116_OGLCORE_GLFW.a
p1: /usr/lib/x86_64-linux-gnu/librt.a
p1: /usr/lib/x86_64-linux-gnu/libm.so
p1: /usr/lib/x86_64-linux-gnu/libX11.so
p1: /usr/lib/x86_64-linux-gnu/libXrandr.so
p1: /usr/lib/x86_64-linux-gnu/libXinerama.so
p1: /usr/lib/x86_64-linux-gnu/libXi.so
p1: /usr/lib/x86_64-linux-gnu/libXcursor.so
p1: /usr/lib/x86_64-linux-gnu/librt.a
p1: /usr/lib/x86_64-linux-gnu/libm.so
p1: /usr/lib/x86_64-linux-gnu/libX11.so
p1: /usr/lib/x86_64-linux-gnu/libXrandr.so
p1: /usr/lib/x86_64-linux-gnu/libXinerama.so
p1: /usr/lib/x86_64-linux-gnu/libXi.so
p1: /usr/lib/x86_64-linux-gnu/libXcursor.so
p1: /usr/lib/x86_64-linux-gnu/libGL.so
p1: /usr/lib/x86_64-linux-gnu/libGLU.so
p1: CMakeFiles/p1.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/andrew/programming/graphics/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX executable p1"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/p1.dir/link.txt --verbose=$(VERBOSE)
	/usr/bin/cmake -E copy /home/andrew/programming/graphics/build/./p1 /home/andrew/programming/graphics/misc05_picking/

# Rule to build all files generated by this target.
CMakeFiles/p1.dir/build: p1
.PHONY : CMakeFiles/p1.dir/build

CMakeFiles/p1.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/p1.dir/cmake_clean.cmake
.PHONY : CMakeFiles/p1.dir/clean

CMakeFiles/p1.dir/depend:
	cd /home/andrew/programming/graphics/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/andrew/programming/graphics /home/andrew/programming/graphics /home/andrew/programming/graphics/build /home/andrew/programming/graphics/build /home/andrew/programming/graphics/build/CMakeFiles/p1.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/p1.dir/depend

