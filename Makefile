build:
	g++ -std=c++17 -Wall ./src/*.cpp ./src/Physics/*.cpp ./src/Inputs/*.cpp ./src/Scenes/*.cpp ./src/Graphics/*.cpp ./src/UI/*.cpp -lm -lSDL2 -lSDL2_image -lSDL2_gfx -lSDL2_ttf -o engine

run:
	./engine

clean:
	rm engine

