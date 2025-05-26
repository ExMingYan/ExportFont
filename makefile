CXX ?= g++
CXXFLAGS := -std=gnu++17 -Wall -Wextra -Ofast

ifeq ($(OS), Windows_NT)
EXE = .exe
LDFLAGS := $(shell pkg-config --static --libs sdl2 SDL2_image SDL2_ttf) -mconsole
else
EXE =
LDFLAGS := -lSDL2 -lSDL2_image -lSDL2_ttf
endif

# Source files
SOURCES = $(wildcard *.cpp)

TARGET = ExportFont$(EXE)

.PHONY: all clean

all: $(TARGET)

clean:
	$(RM) $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)
