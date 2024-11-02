#!/bin/bash
echo "Performing GET /user benchmark"
wrk -t12 -c12 -d30s -s ./get.lua http://user-service:8080

echo "Performing GET /user/search/name benchmark"
wrk -t12 -c12 -d30s -s ./search-name.lua http://user-service:8080
