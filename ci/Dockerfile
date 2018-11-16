FROM adolfogc/stg-8nn-scaffold

# Temporary (remove after base image is updated)
RUN apt-get install cppcheck

ADD ./test-cppcheck.sh /scripts/test-cppcheck.sh
ADD ./build-gcc.sh /scripts/build-gcc.sh
