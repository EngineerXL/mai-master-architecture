FROM ubuntu:24.04

RUN DEBIAN_FRONTEND=noninteractive
RUN apt-get update
RUN apt-get install -y \
    build-essential \
    cmake \
    git \
    g++ \
    make \
    libmysqlclient-dev \
    libpq-dev \
    libssl-dev \
    openssl

# Install Poco
RUN git clone -b main https://github.com/pocoproject/poco.git
RUN cd poco && \
    mkdir cmake-build && \
    cd cmake-build && \
    cmake .. && \
    cmake --build . --config Release -j `nproc`

# Utilities
RUN apt-get install -y \
    clang-format
