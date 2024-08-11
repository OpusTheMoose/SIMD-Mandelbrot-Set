CFLAGS = -std=c++20 -Ofast -fopenmp -pthread -march=native
LDFLAGS = -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lsfml-graphics -lsfml-window -lsfml-system 

Fractal: main.cpp
	g++ $(CFLAGS) -o Fractal main.cpp GLRender.cpp fractalV2.cpp glad.c $(LDFLAGS)

.PHONY: test clean
test: Fractal
	  ./Fractal
clean:
	rm -f Fractal
