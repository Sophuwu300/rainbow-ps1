build:
	mkdir -p build
	g++ -std=c++17 -o build/bashprompt src/prompt.cpp

run:
	./build/bashprompt