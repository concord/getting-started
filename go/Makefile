all: build 

env:
	go env

build:
	GOOS=linux GOARCH=amd64 go build -v -o ./bin/sentence_splitter.o ./src/splitter/main.go
	GOOS=linux GOARCH=amd64 go build -v -o ./bin/sentence_generator.o ./src/generator/main.go
	GOOS=linux GOARCH=amd64 go build -v -o ./bin/word_counter.o ./src/counter/main.go

dist: clean build
