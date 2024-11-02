# Detect OS
ifeq ($(OS),Windows_NT)
    OS_TYPE := Windows
else
    OS_TYPE := $(shell uname -s)
endif

# Python configuration
PYTHON_VERSION := $(shell python3 -c 'import sys; print(f"{sys.version_info.major}.{sys.version_info.minor}")')
PYTHON_PREFIX := $(shell python3-config --prefix)

# OS-specific settings
ifeq ($(OS_TYPE),Darwin)  # macOS
    ifeq ($(shell test -d /opt/homebrew && echo yes),yes)
        # M1/M2 Mac
        PYTHON_PATH := /opt/homebrew/opt/python@$(PYTHON_VERSION)
    else
        # Intel Mac
        PYTHON_PATH := /usr/local/opt/python@$(PYTHON_VERSION)
    endif
    PYTHON_INCLUDE := $(PYTHON_PATH)/Frameworks/Python.framework/Headers
    PYTHON_LIB := $(PYTHON_PATH)/Frameworks/Python.framework/Versions/$(PYTHON_VERSION)/lib
    EXTRA_FLAGS := -undefined dynamic_lookup
else ifeq ($(OS_TYPE),Linux)
    PYTHON_INCLUDE := $(PYTHON_PREFIX)/include/python$(PYTHON_VERSION)
    PYTHON_LIB := $(PYTHON_PREFIX)/lib
    EXTRA_FLAGS :=
else  # Windows
    PYTHON_INCLUDE := $(PYTHON_PREFIX)/include
    PYTHON_LIB := $(PYTHON_PREFIX)/libs
    EXTRA_FLAGS :=
endif

# Compiler settings
CXX := g++
CC := gcc
CXXFLAGS := -std=c++20 -I$(PYTHON_INCLUDE)
CFLAGS := -I$(PYTHON_INCLUDE)
LDFLAGS := -L$(PYTHON_LIB) -lpython$(PYTHON_VERSION) $(EXTRA_FLAGS)

# Target executable
TARGET := program
ifeq ($(OS_TYPE),Windows)
    TARGET := $(TARGET).exe
endif

# Source files
SOURCES := main.cpp simplex_wrapper.c
OBJECTS := $(SOURCES:.cpp=.o)
OBJECTS := $(OBJECTS:.c=.o)

# Python source
PY_SOURCE := simplex.py
PY_COMPILED := __pycache__/simplex.cpython-$(subst .,,$(PYTHON_VERSION)).pyc

# Default target
all: clean $(TARGET)

# Link
$(TARGET): $(OBJECTS) $(PY_COMPILED)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

# Compile C++
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile C
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile Python
$(PY_COMPILED): $(PY_SOURCE)
	python3 -m py_compile $(PY_SOURCE)

# Clean
clean:
	rm -f $(TARGET) $(OBJECTS)
	rm -rf __pycache__

# Show configuration
config:
	@echo "OS Type: $(OS_TYPE)"
	@echo "Python Version: $(PYTHON_VERSION)"
	@echo "Python Prefix: $(PYTHON_PREFIX)"
	@echo "Python Include: $(PYTHON_INCLUDE)"
	@echo "Python Library: $(PYTHON_LIB)"
	@echo "Extra Flags: $(EXTRA_FLAGS)"

.PHONY: all clean config