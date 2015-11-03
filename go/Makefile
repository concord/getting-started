APPNAME = lightrail
DISTDIR = bin

all: dist

clean:
	rm -rf $(DISTDIR)

env:
	go env

build:
	GOOS=linux GOARCH=amd64 go build -o bin/computation

dist: clean build

