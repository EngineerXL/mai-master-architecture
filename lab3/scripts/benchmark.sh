#!/bin/bash
echo "Performing GET /chat benchmark"
wrk -t12 -c12 -d30s -s ./get_chat.lua http://chat-service:8080
echo "Performing GET /message/chat benchmark"
# todo thread safe client ?
wrk -t1 -c1 -d30s -s ./get_chat_message.lua http://message-service:8080
