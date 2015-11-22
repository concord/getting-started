APPNAME = computation.out

all: build 

env:
	go env

build:
	GOOS=linux GOARCH=amd64 go build -o $(APPNAME) 

deploy:
	concord deploy counter_manifest.json
	concord deploy producer_manifest.json

dist: clean build

