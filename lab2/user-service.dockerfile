FROM cpp-poco

ARG BUILD_TYPE=Release

# Install Python and Debug Utilities
RUN apt-get update
RUN apt-get install -y curl gdb libpq-dev python3 python3-pip
RUN apt-get clean

WORKDIR /app
COPY scripts/fill.py .
COPY scripts/requirements.txt .
COPY scripts/run.sh .
RUN pip install -r requirements.txt

WORKDIR /build
COPY . .

RUN if [ "${BUILD_TYPE}" = "Debug" ] ; \
    then \
    cmake -DCMAKE_BUILD_TYPE=Debug -B=build . ; \
    else \
    cmake -DCMAKE_BUILD_TYPE=Release -B=build . ; \
    fi
RUN cmake --build build -j `nproc`

WORKDIR /app
RUN cp /build/build/user-service .
ENTRYPOINT [ "bash", "run.sh" ]
