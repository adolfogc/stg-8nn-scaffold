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
  echo "deb http://apt.llvm.org/stretch/ llvm-toolchain-stretch-7 main" >> /etc/apt/sources.list && \
  echo "deb-src http://apt.llvm.org/stretch/ llvm-toolchain-stretch-7 main" >> /etc/apt/sources.list && \
  apt-get -y update && \
  apt-get -y --no-install-recommends install \
    cmake make ninja-build git binutils g++ libpcre3-dev \
    clang-7 lldb-7 lld-7 clang-format-7 && \
  (cd /tmp && \
  curl -SOL https://github.com/danmar/cppcheck/archive/1.85.tar.gz && \
  tar xzvf 1.85.tar.gz && \
  cd cppcheck-1.85 && \
  make SRCDIR=build CFGDIR=/usr/share/cppcheck/ HAVE_RULES=yes CXXFLAGS="-O2 -DNDEBUG -Wall -Wno-sign-compare -Wno-unused-function" -j2 && \
  make install CFGDIR=/usr/share/cppcheck/) && \
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
