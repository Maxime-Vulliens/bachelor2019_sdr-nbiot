# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_SOURCE_DIR = /home/gnuradio/Desktop/gnubloc/gr-nbiot

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/gnuradio/Desktop/gnubloc/gr-nbiot/build

# Utility rule file for nbiot_swig_swig_doc.

# Include the progress variables for this target.
include swig/CMakeFiles/nbiot_swig_swig_doc.dir/progress.make

swig/CMakeFiles/nbiot_swig_swig_doc: swig/nbiot_swig_doc.i


swig/nbiot_swig_doc.i: swig/nbiot_swig_doc_swig_docs/xml/index.xml
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/gnuradio/Desktop/gnubloc/gr-nbiot/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating python docstrings for nbiot_swig_doc"
	cd /home/gnuradio/Desktop/gnubloc/gr-nbiot/docs/doxygen && /usr/bin/python2 -B /home/gnuradio/Desktop/gnubloc/gr-nbiot/docs/doxygen/swig_doc.py /home/gnuradio/Desktop/gnubloc/gr-nbiot/build/swig/nbiot_swig_doc_swig_docs/xml /home/gnuradio/Desktop/gnubloc/gr-nbiot/build/swig/nbiot_swig_doc.i

swig/nbiot_swig_doc_swig_docs/xml/index.xml: swig/_nbiot_swig_doc_tag
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/gnuradio/Desktop/gnubloc/gr-nbiot/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Generating doxygen xml for nbiot_swig_doc docs"
	cd /home/gnuradio/Desktop/gnubloc/gr-nbiot/build/swig && ./_nbiot_swig_doc_tag
	cd /home/gnuradio/Desktop/gnubloc/gr-nbiot/build/swig && /usr/bin/doxygen /home/gnuradio/Desktop/gnubloc/gr-nbiot/build/swig/nbiot_swig_doc_swig_docs/Doxyfile

nbiot_swig_swig_doc: swig/CMakeFiles/nbiot_swig_swig_doc
nbiot_swig_swig_doc: swig/nbiot_swig_doc.i
nbiot_swig_swig_doc: swig/nbiot_swig_doc_swig_docs/xml/index.xml
nbiot_swig_swig_doc: swig/CMakeFiles/nbiot_swig_swig_doc.dir/build.make

.PHONY : nbiot_swig_swig_doc

# Rule to build all files generated by this target.
swig/CMakeFiles/nbiot_swig_swig_doc.dir/build: nbiot_swig_swig_doc

.PHONY : swig/CMakeFiles/nbiot_swig_swig_doc.dir/build

swig/CMakeFiles/nbiot_swig_swig_doc.dir/clean:
	cd /home/gnuradio/Desktop/gnubloc/gr-nbiot/build/swig && $(CMAKE_COMMAND) -P CMakeFiles/nbiot_swig_swig_doc.dir/cmake_clean.cmake
.PHONY : swig/CMakeFiles/nbiot_swig_swig_doc.dir/clean

swig/CMakeFiles/nbiot_swig_swig_doc.dir/depend:
	cd /home/gnuradio/Desktop/gnubloc/gr-nbiot/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/gnuradio/Desktop/gnubloc/gr-nbiot /home/gnuradio/Desktop/gnubloc/gr-nbiot/swig /home/gnuradio/Desktop/gnubloc/gr-nbiot/build /home/gnuradio/Desktop/gnubloc/gr-nbiot/build/swig /home/gnuradio/Desktop/gnubloc/gr-nbiot/build/swig/CMakeFiles/nbiot_swig_swig_doc.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : swig/CMakeFiles/nbiot_swig_swig_doc.dir/depend

