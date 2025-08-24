# Default target
.PHONY: run
run: build
	./mosaic

# Target for building the project
.PHONY: build
build: 
ifeq ($(OS),Windows_NT)
	gcc -std=c99 -Wall -o mosaic.exe ./main.c
else
	gcc -std=c99 -Wall -o mosaic ./main.c
endif

# Target for cleaning up
.PHONY: clean
clean:
ifeq ($(OS),Windows_NT)
	del mosaic.exe
else
	rm ./mosaic
endif