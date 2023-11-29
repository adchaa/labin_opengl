LIBS = -lglut -lGLU -lGL -lm
build: main.cpp
	gcc -Wall  main.cpp -O3 -o main $(LIBS)
run: main.cpp
	gcc -Wall -o main main.cpp $(LIBS) && ./main
clean: main