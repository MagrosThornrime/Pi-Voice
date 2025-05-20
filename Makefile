.PHONY: all

all: backend frontend

setup: *
	$(MAKE) -C frontend setup

backend: backend/*
	$(MAKE) -C backend

frontend: frontend/*
	$(MAKE) -C frontend