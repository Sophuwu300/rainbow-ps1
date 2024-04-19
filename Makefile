ifeq ($(shell command -v cmake), )
COMPCMD = g++ --std=c++17 -o build/bashprompt main.cpp
COMPMSG = g++
else
ifeq ($(shell command -v ninja), )
COMPCMD = cmake -S . -B build && cmake --build build
COMPMSG = cmake
else
COMPCMD = cmake -GNinja -S . -B build && cmake --build build
COMPMSG = cmake and ninja
endif
CMAKELISTS = CMakeLists.txt
endif

build: main.cpp $(CMAKELISTS)
	@$(COMPCMD)

clean: build/
	@echo Cleaning up.
	@rm -rf build
	@echo Cleaned up successfully.