FROM cpp-poco

ARG BUILD_TYPE=Release

WORKDIR /app
COPY lab2/scripts/requirements.txt .
RUN pip install -r requirements.txt
COPY lab2/scripts/fill.py .
COPY lab2/scripts/run.sh .

WORKDIR /build
COPY common common
WORKDIR /build/service
COPY lab2 .

RUN if [ "${BUILD_TYPE}" = "Debug" ] ; \
    then \
    cmake -DCMAKE_BUILD_TYPE=Debug -B=build . ; \
    else \
    cmake -DCMAKE_BUILD_TYPE=Release -B=build . ; \
    fi
RUN cmake --build build -j `nproc`

WORKDIR /app
RUN cp /build/service/build/user-service .
ENTRYPOINT [ "bash", "run.sh" ]
