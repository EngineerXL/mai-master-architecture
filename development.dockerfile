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
    openssl

# Install Poco
RUN git clone -b main https://github.com/pocoproject/poco.git
WORKDIR /poco
RUN mkdir cmake-build && \
    cd cmake-build && \
    cmake .. && \
    cmake --build . --config Release && \
    cmake --build . --target install -j `nproc`
RUN ldconfig && cd .. && rm -rf poco

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
