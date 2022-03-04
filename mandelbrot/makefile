out: main.o 
	g++ main.o -o out

main.o: main.cpp mandelbrot.h
	g++ -c main.cpp

clean:
	del /Q /S *.o *.exe