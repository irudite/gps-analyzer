CXX := clang++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic

TINYXML_DIR := $(shell brew --prefix tinyxml2)
CPPFLAGS := -I"$(TINYXML_DIR)/include"
LDFLAGS := -L"$(TINYXML_DIR)/lib"
LDLIBS := -ltinyxml2

gps-analyzer: main.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $< $(LDFLAGS) $(LDLIBS) -o $@

.PHONY: run
run: gps-analyzer
	./gps-analyzer run.gpx
