SHELL := bash

.PHONY: all
all: run paths

.PHONY: run
run:
	@rm -f stderr.log stdout.log && \
	./cross-build.sh 1> stdout.log 2> stderr.log

.PHONY: test
test:
	@./cross-build.sh --target pal-test0

.PHONY: json
json:
	cat stdout.log | grep ^{ | jq "." |less

.PHONY: paths
paths:
	cat stdout.log |grep ^{| jq '.message?.payload? | "\(.source) \(.destination)"'|sort -nr |uniq