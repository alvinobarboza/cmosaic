# Default target
.PHONY: run
run: build
	./bin/mosaic

# Target for building the project
.PHONY: build
build: 
ifeq ($(OS),Windows_NT)
	gcc -std=c99 -Wall -o bin/mosaic.exe ./main.c
else
	gcc -std=c99 -Wall -o bin/mosaic ./main.c
endif

# Target for building the project
.PHONY: release
release: 
ifeq ($(OS),Windows_NT)
	gcc -std=c99 -O2 -Wall -o bin/mosaic.exe ./main.c
else
	gcc -std=c99 -O2 -Wall -o bin/mosaic ./main.c
endif

# Target for cleaning up
.PHONY: clean
clean:
ifeq ($(OS),Windows_NT)
	del bin/mosaic.exe
else
	rm ./bin/mosaic
endif