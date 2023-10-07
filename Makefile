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

ifeq ($(shell command -v strip), )
STRIPCMD = echo Strip not found. Binary size may be larger than expected.
else
STRIPCMD = strip --strip-all build/bashprompt
endif
ifeq ($(shell command -v upx), )
UPXCMD = echo UPX not found. Binary size may be larger than expected.
else
UPXCMD = upx --best build/bashprompt > /dev/null
endif

build: main.cpp $(CMAKELISTS)
	@echo Building with $(COMPMSG).
	@sleep 2
	@mkdir -p build
	@$(COMPCMD)
	@echo Compiled successfully.
	@echo Program is located at build/bashprompt.
	@echo "make small" to reduce binary size. Requires strip and/or upx.
	@echo "sudo make install" to install the program into /usr/local/bin.
	@echo And "make clean" to remove the build directory.

clean: build/
	@echo Cleaning up.
	@rm -rf build
	@echo Cleaned up successfully.

small: build/bashprompt
	@cp build/bashprompt build/bashprompt.bak
	@echo Stripping binary.
	@$(STRIPCMD)
	@$(UPXCMD)
	@echo Old Size: && du -sh build/bashprompt.bak
	@echo New size: && du -sh build/bashprompt


install: build/bashprompt
	@echo Installing to /usr/local/bin.
	@sudo cp build/bashprompt /usr/local/bin/bashprompt
	@echo Installed successfully.
	@echo Run "bashprompt" to use the program.
