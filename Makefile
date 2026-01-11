.PHONY: all

all: backend frontend

setup: *
	$(MAKE) -C frontend setup

backend: backend/*
	$(MAKE) -C backend

frontend: frontend/*
	$(MAKE) -C frontend

tests:
	@case "$$OS" in \
	Windows_NT) run_tests.bat ;; \
	*) ./run_tests.bash ;; \
	esac
