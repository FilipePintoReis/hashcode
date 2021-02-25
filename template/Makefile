# Python run commands
PYCODE := code.py
PYRUN := python3

# Compiler, gcc or clang
COMPILER := gcc
CPP_STANDARD := c++17

# Compiler command
CC := g++
CODE := ./code.cpp
EXE := ./hack
LIBS := -lfmt
HPP_FILES := lib.hpp
INPUT_FILES := $(shell find input/ -type f)

include cpp-flags.make
CXXFLAGS += $(LIBS)

# * Modes

.PHONY: perfm debug clean submit
.PHONY: run fast each fasteach valgrind

all: debug
	@true

# Compile for performance
perfm: MODE := performance
perfm: CXXFLAGS += $(OPTIM)
perfm: $(EXE)

# Compile for debugging
debug: MODE := debug
debug: CXXFLAGS += $(DEBUG)
debug: $(EXE)

# Main compilation rule
$(EXE): $(CODE) $(HPP_FILES)
	@echo "CC (${COMPILER} $(MODE))  $@"
	@$(CXX) $(CODE) $(CXXFLAGS) -o $@

# Run python on one file
py python:
	@read -e -p "Input file: " file ; $(PYRUN) $(PYCODE) $$file

# Run python on each file
pyeach pythoneach:
	@for file in $(INPUT_FILES); do $(PYRUN) $(PYCODE) $$file ; done

# Compile for debugging and run on current file
run: debug
	@read -e -p "Input file: " file ; $(EXE) $$file

# Compile for performance and run
fast: perfm
	@read -e -p "Input file: " file ; $(EXE) $$file

# Compile for debugging and run each input
each: debug
	@for file in $(INPUT_FILES); do $(EXE) $$file ; done

# Compile for debugging and run each input
fasteach: perfm
	@for file in $(INPUT_FILES); do $(EXE) $$file ; done

# Build in debug mode and run under valgrind
valgrind: debug
	@clear
	@read -e -p "Input file: " file ; valgrind $(EXE) $$file

clean:
	@rm -f $(EXE) core vgcore.* *.log

submit:
	zip -r submission.zip code.cpp lib.hpp Makefile cpp-flags.make README.md code.py
