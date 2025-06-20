CC        := g++
PKG_CFLAGS := $(shell pkg-config --cflags opencv4)
PKG_LIBS   := $(shell pkg-config --libs opencv4)

CFLAGS    := -fopenmp $(PKG_CFLAGS)
LDFLAGS   := -fopenmp $(PKG_LIBS)

DEPS      :=
OBJ       := parallel_filter_openmp.o sequential_filter_apply.o

%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

parallel_filter_openmp: parallel_filter_openmp.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

parallel_filter_pthreads: parallel_filter_pthreads.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

sequential_filter_apply: sequential_filter_apply.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

all:  parallel_filter_openmp parallel_filter_pthreads sequential_filter_apply 

clean:
	rm -f $(OBJ)  parallel_filter_openmp parallel_filter_pthreads sequential_filter_apply

.PHONY: clean all

test: parallel_filter_openmp parallel_filter_pthreads sequential_filter_apply
	./sequential_filter_apply Images/Cosmos.jpg Filters/Edge_Mask_2.txt Output/output_serial.jpg
	./parallel_filter_openmp Images/Cosmos.jpg Filters/Edge_Mask_2.txt Output/output_parallel.jpg 6
	./parallel_filter_pthreads Images/Cosmos.jpg Filters/Edge_Mask_2.txt Output/output_parallel.jpg 6
