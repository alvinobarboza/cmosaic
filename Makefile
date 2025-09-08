# Default target
.PHONY: run
run: build
	./bin/mosaic

# Target for building the project
.PHONY: build
build: 
ifeq ($(OS),Windows_NT)
	gcc -std=c99 -Wall -Wextra -o bin/mosaic.exe ./all.c
else
	gcc ./all.c -std=c99 -Wall -Wextra -Iinclude/raylib/include/ -Linclude/raylib/lib/ -lraylib -lm -lpthread -ldl -lrt -lX11 -o bin/mosaic 
endif

# Target for building the project
.PHONY: release
release: 
ifeq ($(OS),Windows_NT)
	gcc -std=c99 -O2 -Wall -Wextra -o bin/mosaic.exe ./all.c
else
	gcc ./all.c -std=c99 -O2 -Wall -Wextra -Iinclude/raylib/include/ -Linclude/raylib/lib/ -lraylib -lm -lpthread -ldl -lrt -lX11 -o bin/mosaic 
endif

# Target for cleaning up
.PHONY: clean
clean:
ifeq ($(OS),Windows_NT)
	del bin/mosaic.exe
else
	rm ./bin/mosaic
endif