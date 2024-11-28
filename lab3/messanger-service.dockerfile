FROM cpp-poco

ARG BUILD_TYPE=Release

WORKDIR /app
COPY scripts/requirements.txt .
RUN pip install -r requirements.txt
COPY scripts/fill.py .
COPY scripts/run.sh .

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
RUN cp /build/build/messanger-service .
ENTRYPOINT [ "bash", "run.sh" ]
