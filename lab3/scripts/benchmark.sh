#!/bin/bash
echo "Performing GET /chat benchmark"
wrk -t12 -c12 -d30s -s ./get_chat.lua http://messanger-service:8080
