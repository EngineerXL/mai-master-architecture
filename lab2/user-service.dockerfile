FROM cpp-poco AS build

ARG BUILD_TYPE=Release

WORKDIR /build
COPY . .

RUN if [ "${BUILD_TYPE}" = "Debug" ] ; \
    then \
    cmake -DCMAKE_BUILD_TYPE=Debug -B=build . ; \
    else \
    cmake -DCMAKE_BUILD_TYPE=Release -B=build . ; \
    fi
RUN cmake --build build -j `nproc`

FROM ubuntu:22.04

RUN apt-get update
RUN apt-get install -y gdb libpq-dev python3 python3-pip
RUN apt-get clean

WORKDIR /app
COPY scripts/ .
RUN pip install -r requirements.txt
COPY --from=build /build/build/user-service .
ENTRYPOINT [ "bash", "run.sh" ]
