defualt: bashprompt

bashprompt: src/
	mkdir -p build
	g++ -std=c++17 -o build/bash-rb-ps1 src/*.cpp

clean:
	@echo "unimplemented"
