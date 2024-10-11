include ../makefile

user-service:
	docker compose build user-service

up:
	docker compose up user-service

down:
	docker compose down -v

restart: down up

all: cpp-poco user-service restart
