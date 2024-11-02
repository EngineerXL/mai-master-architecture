#!/bin/bash
export TEST_N_USERS=100000
docker compose up -d user-service

echo "Waiting for database filling with test data"
while : ; do
    is_init=`docker logs user-service | grep "Database is successfully filled"`
    sleep 1
    [[ ! -z $is_init ]] && break
done

echo "Performing GET /user benchmark"
wrk -t12 -c12 -d30s -s ./scripts/get.lua http://127.0.0.1:8080

echo "Performing GET /user/search/name benchmark"
wrk -t12 -c12 -d30s -s ./scripts/search-name.lua http://127.0.0.1:8080

docker compose down -v
