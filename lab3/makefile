include ../makefile

messanger-service:
	docker compose build messanger-service

up:
	docker compose up -d messanger-service

down:
	docker compose down -v

restart: down up

all: cpp-poco messanger-service restart
