#!/bin/bash
export TEST_N_USERS=100000
docker compose up test-lab2
docker compose down -v
