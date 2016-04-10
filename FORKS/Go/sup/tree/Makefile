.PHONY: all build dist test install clean tools deps update-deps

all:
	@echo "build         - Build sup"
	@echo "dist          - Build sup distribution binaries"
	@echo "test          - Run tests"
	@echo "install       - Install binary"
	@echo "clean         - Clean up"
	@echo ""
	@echo "tools         - Install tools"
	@echo "vendor-list   - List vendor package tree"
	@echo "vendor-update - Update vendored packages"

build:
	@mkdir -p ./bin
	@rm -f ./bin/*
	go build -o ./bin/sup ./cmd/sup

dist:
	@mkdir -p ./bin
	@rm -f ./bin/*
	GOOS=darwin GOARCH=amd64 go build -o ./bin/sup-darwin64 ./cmd/sup
	GOOS=linux GOARCH=amd64 go build -o ./bin/sup-linux64 ./cmd/sup
	tar -czf ./bin/sup-linux64.tar.gz ./bin/sup-linux64
	tar -czf ./bin/sup-darwin64.tar.gz ./bin/sup-darwin64

test:
	go test ./...

install:
	go install ./cmd/sup

clean:
	@rm -rf ./bin

tools:
	go get -u github.com/kardianos/govendor

vendor-list:
	@govendor list

vendor-update:
	@govendor update +external
