.PHONY: all

all: backend frontend

setup:
	npx electron-rebuild

backend:
	$(MAKE) -C backend

frontend:
	$(MAKE) -C frontend