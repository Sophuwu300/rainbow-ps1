# Makefile for building the deb package
NAME = bash-rb-ps1
# Versioning
# run `date '+%Y.%m.%d' to set the version`
VERSION = $(shell date '+%Y.%m.%d')

START_DIR = $(shell pwd)
SRC_DIR = ../src
BUILD_DIR = ./build/$(VERSION)

ifeq ($(shell command -v upx), )
UPX = echo "UPX not found, skipping compression"
else
UPX = upx --lzma --best $(NAME)
endif


FLAGS =

defualt: $(SRC_DIR)/ bash-rb-ps1.1
	mkdir -p $(BUILD_DIR)
	cp -r $(SRC_DIR) $(BUILD_DIR)
	sed -e 's/{{ Version }}/$(VERSION)/g' bash-rb-ps1.1 > ./$(BUILD_DIR)/bash-rb-ps1.1
	VERSION=$(VERSION) TARGET="amd64" ./zig-cpp.sh
	VERSION=$(VERSION) TARGET="arm64" ./zig-cpp.sh

clean:
	@echo "unimplemented"
