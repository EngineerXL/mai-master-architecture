FROM ubuntu:22.04

RUN apt-get update
RUN apt-get install -y curl wrk
RUN apt-get clean

WORKDIR /app
ENTRYPOINT [ "bash", "benchmark.sh" ]
