build: src/*.cpp
	mkdir -p build
	g++ -std=c++17 -o build/bashprompt src/*.cpp
run:
	./build/bashprompt
clean:
	@echo "unimplemented"