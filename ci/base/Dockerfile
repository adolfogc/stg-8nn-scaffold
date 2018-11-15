FROM debian:stretch-slim

# Note 0: ARM Embedded toolchains can be found here:
# https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads

# Note 1: '99fixbadproxy' is needed for preventing build fails due to invalid checksums

RUN rm -rf /var/lib/apt/lists/* && \
  echo "Acquire::http::Pipeline-Depth 0;" > /etc/apt/apt.conf.d/99fixbadproxy && \
  echo "Acquire::http::No-Cache true;" >> /etc/apt/apt.conf.d/99fixbadproxy && \
  echo "Acquire::BrokenProxy true;" >> /etc/apt/apt.conf.d/99fixbadproxy && \
  apt-get -y update && \
  apt-get -y --no-install-recommends install apt-utils curl ca-certificates bzip2 gnupg2 && \
  curl -SL https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add - && \
  echo "deb http://apt.llvm.org/stretch/ llvm-toolchain-stretch-6.0 main" >> /etc/apt/sources.list && \
  echo "deb-src http://apt.llvm.org/stretch/ llvm-toolchain-stretch-6.0 main" >> /etc/apt/sources.list && \
  apt-get -y update && \
  apt-get -y --no-install-recommends install \
    cmake make ninja-build git binutils \
    libclang-6.0-dev llvm-6.0-dev \
    clang-6.0 clang-tools-6.0 \
    clang-tidy-6.0 clang-format-6.0 && \
  (cd /tmp && \
  git clone --depth=1 https://github.com/bloodstalker/mutator && \
  cd mutator && \
  git submodule init && \
  git submodule update && \
  make -j2 mutator-lvl0 CXX=clang++-6.0 LLVM_CONF=llvm-config-6.0 BUILD_MODE=COV_NO_CLANG && \
  cp m0/mutator-lvl0 /usr/local/bin) && \
  (cd /tmp && \
  curl -SOL https://developer.arm.com/-/media/Files/downloads/gnu-rm/7-2018q2/gcc-arm-none-eabi-7-2018-q2-update-linux.tar.bz2 && \
  tar xjvf gcc-arm-none-eabi-7-2018-q2-update-linux.tar.bz2 && \
  cp -r /tmp/gcc-arm-none-eabi-7-2018-q2-update/* /usr/local/) && \
  rm -rf /tmp/* && \
  (cd / && \
  curl -SOL https://github.com/krallin/tini/releases/download/v0.18.0/tini-static-amd64 && \
  mv /tini-static-amd64 /tini && \
  chmod u+x /tini)

VOLUME ["/src", "/build"]
ENTRYPOINT ["/tini", "--"]
CMD ["/bin/bash"]
