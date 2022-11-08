build:
	g++ -std=c++17 -Wall ./src/*.cpp ./src/Physics/*.cpp ./src/Inputs/*.cpp -lm -lSDL2 -lSDL2_image -lSDL2_gfx -o engine

run:
	./engine

clean:
	rm engine

