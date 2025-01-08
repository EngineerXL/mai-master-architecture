FROM cpp-poco

ARG BUILD_TYPE=Release

WORKDIR /app
COPY lab3/scripts/requirements.txt .
RUN pip install -r requirements.txt
COPY lab3/scripts/fill.py .
COPY lab3/scripts/run_chat-service.sh run.sh

WORKDIR /build
COPY common common
WORKDIR /build/service
COPY lab3 .

RUN if [ "${BUILD_TYPE}" = "Debug" ] ; \
    then \
    cmake -DCMAKE_BUILD_TYPE=Debug -B=build . ; \
    else \
    cmake -DCMAKE_BUILD_TYPE=Release -B=build . ; \
    fi
RUN cmake --build build --target chat-service -j `nproc`

WORKDIR /app
RUN cp /build/service/build/chat-service/chat-service .
ENTRYPOINT [ "bash", "run.sh" ]
