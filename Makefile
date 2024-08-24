defualt: bashprompt

bashprompt: src/
	mkdir -p build
	g++ -std=c++17 -o build/bashprompt src/*.cpp

clean:
	@echo "unimplemented"
