include ../makefile

build_chat-service:
	docker compose build chat-service

build_message-service:
	docker compose build message-service

up:
	docker compose up -d chat-service message-service

down:
	docker compose down -v

restart: down up

all: cpp-poco build_chat-service build_message-service restart
