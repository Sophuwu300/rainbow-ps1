
build:
	cmake -B build && cmake --build build ||  g++ --std=c++17 -o build/bashprompt src/prompt.cpp

clean: build/
	@echo Cleaning up.
	@rm -rf build
	@echo Cleaned up successfully.