CC = g++
CFLAGS = -pthread `pkg-config --cflags opencv4`
LIBS = `pkg-config --libs opencv4`
DEPS =
OBJ = apply_filter.o papply_filter.o

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

apply_filter: apply_filter.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

papply_filter: papply_filter.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

test: apply_filter papply_filter
	./apply_filter Images/street.jpg Filters/Edge_Mask_2.txt Output/output_serial.jpg 1
	./papply_filter Images/street.jpg Filters/Edge_Mask_2.txt Output/output_parallel.jpg 4

clean:
	rm -f *.o *~ apply_filter papply_filter Output/*.jpg