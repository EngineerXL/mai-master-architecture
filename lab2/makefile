include ../makefile

build_user-service:
	docker compose build user-service

up:
	docker compose up -d user-service

down:
	docker compose down -v

restart: down up

all: cpp-poco build_user-service restart
