out = ./gl.out
deps = -lGL -lGLEW -lglfw -lassimp -Wpedantic -pedantic-errors

clean:
	rm $(out)

build:
	g++ src/main.cpp src/utils/shader_tools.cpp $(deps) -o $(out)

run:
	make build
	$(out)