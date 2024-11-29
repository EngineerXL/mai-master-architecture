#!/bin/bash
export TEST_N_USERS=100000
export TEST_N_CHATS=1000
export TEST_N_MESSAGES=100
docker compose up test-lab3
docker compose down -v
