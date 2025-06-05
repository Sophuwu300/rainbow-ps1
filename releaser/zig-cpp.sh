#!/bin/bash
command -v upx
doUpx="$?"

set -e
command -v zig
[[ "$TARGET" == "amd64" || "$TARGET" == "arm64" ]] || exit 1
[[ "$VERSION" == "$(date +%Y.%m.%d)" ]] || exit 1
START_DIR=$(pwd)
BUILD_DIR="$START_DIR/build/$VERSION"
SRC_DIR="$BUILD_DIR/src"
BUILD_DIR="$BUILD_DIR/$TARGET"
BIN_OUT="$BUILD_DIR/bash-rb-ps1"

mkdir -p $BUILD_DIR

ZIGTAR="x86_64-linux-musl"
if [[ $TARGET == "arm64" ]]; then
  ZIGTAR="aarch64-linux-musl"
elif [[ $TARGET == "amd64" ]]; then
  ZIGTAR="x86_64-linux-musl"
else
  echo "Unsupported target: $TARGET"
  exit 1
fi

sed -e "s/{{ Version }}/$VERSION/g" "$START_DIR/nfpm.yaml" | sed -e "s/{{ Target }}/$TARGET/g" > "$BUILD_DIR/nfpm.yaml"
cd "$BUILD_DIR"
zig c++ -target $ZIGTAR -std=c++17 -o "$BIN_OUT" $SRC_DIR/*.cpp
if [[ $doUpx == "0" ]]; then
  upx --best --lzma "$BIN_OUT"
fi
nfpm package --packager deb --config nfpm.yaml