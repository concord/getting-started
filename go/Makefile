DISTDIR = bin

all: dist

clean:
	rm -rf $(DISTDIR)

env:
	go env

build:
	GOOS=linux GOARCH=amd64 go build -o bin/computation

deploy:
	concord deploy counter_manifest.json
	concord deploy producer_manifest.json

dist: clean build

