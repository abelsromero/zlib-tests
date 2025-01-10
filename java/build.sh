#!/usr/bin/env bash

set -euo pipefail

readonly MUSL_VERSION="1.2.5"
readonly ZLIB_VERSION="1.3.1"

readonly CC_ENV="cc_env"
readonly MUSL_HOME="$PWD/$CC_ENV/musl-toolchain"

clean() {
  rm -rf "$CC_ENV"
}

prepare_env() {
  mkdir -p "$CC_ENV"
}

install_musl() {
  curl -O "https://musl.libc.org/releases/musl-${MUSL_VERSION}.tar.gz"

 # Build musl from source
  tar -xzvf "musl-${MUSL_VERSION}.tar.gz"
  pushd "musl-${MUSL_VERSION}"
    ./configure --prefix=$MUSL_HOME --static
    # The next operation may require privileged access to system resources, so use sudo
    make -j $(nproc) && make install
  popd

  # Install a symlink for use by native-image
  ln -s "${MUSL_HOME}/bin/musl-gcc" "${MUSL_HOME}/bin/x86_64-linux-musl-gcc"

  rm "musl-${MUSL_VERSION}.tar.gz"
  rm -rf "musl-${MUSL_VERSION}"
}

install_zlib() {
  curl -O "https://zlib.net/fossils/zlib-${ZLIB_VERSION}.tar.gz"

  # Build zlib with musl from source and install into the MUSL_HOME directory
  tar -xzvf "zlib-${ZLIB_VERSION}.tar.gz"
  pushd "zlib-${ZLIB_VERSION}"
    CC=musl-gcc ./configure --prefix=$MUSL_HOME --static
    make -j $(nproc) && make install
  popd

  rm "zlib-${ZLIB_VERSION}.tar.gz"
  rm -rf "zlib-${ZLIB_VERSION}"
}

build_java_static() {
  javac EnvMap.java
  native-image --static --libc=musl EnvMap
}

main() {
  clean
  PATH="$MUSL_HOME/bin:$PATH"
  (
    prepare_env
    cd "$CC_ENV"
    install_musl
    install_zlib
  )
  build_java_static
}

main