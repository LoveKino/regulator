THIS_FILE := $(lastword $(MAKEFILE_LIST))

build:
	@echo $@
	g++ -std=c++11 ./lib/**.cc ./app/**.cc -I./lib -o ./bin/regulator
test: 
	@echo $@
	./test/index.sh

.PHONY: test
