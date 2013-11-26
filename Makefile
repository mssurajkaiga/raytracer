CC = g++
CFLAGS = -I ./eigen/ -I ./EasyBMP/
BMP = EasyBMP/EasyBMP.cpp

all: test

test: test.cpp raytracer.o scene.o lights.o objects.o utility.o
	$(CC) $(CFLAGS) test.cpp raytracer.o scene.o lights.o objects.o utility.o $(BMP) -o test

raytracer.o: raytracer.cpp scene.o
	$(CC) $(CFLAGS) -c raytracer.cpp scene.o

scene.o: scene.cpp core.o lights.o objects.o utility.o
	$(CC) $(CFLAGS) -c scene.cpp core.o lights.o objects.o utility.o

lights.o: lights.cpp
	$(CC) $(CFLAGS) -c lights.cpp

objects.o: objects.cpp core.o
	$(CC) $(CFLAGS) -c objects.cpp core.o

utility.o: utility.cpp core.o
	$(CC) $(CFLAGS) -c utility.cpp core.o

core.o: core.cpp
	$(CC) $(CFLAGS) -c core.cpp

clean:
	rm -rf *.o *.out test output.bmp *.log