FROM ubuntu:24.04 AS build

ARG BUILD_TYPE=Release

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
    cmake --build . --config Release

WORKDIR /build
COPY . .

RUN if [ "${BUILD_TYPE}" = "Debug" ] ; \
    then \
    cmake -DCMAKE_BUILD_TYPE=Debug -B=build . ; \
    else \
    cmake -DCMAKE_BUILD_TYPE=Release -B=build . ; \
    fi
RUN cmake --build build -j `nproc`

FROM ubuntu:24.04

WORKDIR /app

RUN apt-get update
RUN apt-get install -y gdb

COPY --from=build /build/build/user-service .

ENTRYPOINT [ "./user-service" ]
