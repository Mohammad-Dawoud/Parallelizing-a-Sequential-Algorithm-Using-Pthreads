CC = g++
CFLAGS = -pthread `pkg-config --cflags opencv4`
LIBS = `pkg-config --libs opencv4`
DEPS =
OBJ = sequential_filter_apply.o parallel_filter_apply.o

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

sequential_filter_apply: sequential_filter_apply.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

parallel_filter_apply: parallel_filter_apply.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

all: sequential_filter_apply parallel_filter_apply

clean:
	rm -f *.o sequential_filter_apply parallel_filter_apply

.PHONY: clean all

test: sequential_filter_apply parallel_filter_apply
	./sequential_filter_apply Images/street.jpg Filters/Edge_Mask_2.txt Output/output_serial.jpg
	./parallel_filter_apply Images/street.jpg Filters/Edge_Mask_2.txt Output/output_parallel.jpg 4