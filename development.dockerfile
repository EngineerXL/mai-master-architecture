FROM ubuntu:22.04

# Install Dependencies
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
    openssl \
    wget \
    zip

# Install Poco
RUN wget https://github.com/pocoproject/poco/archive/refs/tags/poco-1.13.2-release.zip && \
    unzip poco-1.13.2-release.zip
WORKDIR /poco-poco-1.13.2-release
RUN mkdir cmake-build && \
    ls && \
    cd cmake-build && \
    cmake .. && \
    cmake --build . --config Release && \
    cmake --build . --target install -j `nproc`
RUN ldconfig && cd .. && rm -rf poco-poco-1.13.2-release poco-1.13.2-release.zip

# Install Python and Debug Utilities
RUN apt-get update
RUN apt-get install -y \
    curl \
    gdb \
    libpq-dev \
    python3 \
    python3-pip \
    clang-format \
    htop \
    tree \
    vim
RUN apt-get clean
