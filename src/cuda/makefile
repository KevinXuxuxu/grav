PROJ = main
CC = g++
NVCC = nvcc -O3


CFLAGS = -c -g -Wall -I/opt/local/include -I$(HOME)/cppunit/include -msse3 -O3 -lm
LDFLAGS = -L/opt/local/lib -L$(HOME)/cppunit/lib
LIBS = -lcppunit -ldl

OBJS = gravity_cuda.o main_cuda.o

all: $(PROJ)

$(PROJ): $(OBJS) $(TEST_CASES)
	$(NVCC) $(LDFLAGS) $^ -o $@ $(LIBS)

%.o : %.cu
	$(NVCC) -c $< -o $@

%.o : %.cpp
	$(CC) $(CFLAGS) $< -o $@ 

clean:
	rm -f $(PROJ) $(OBJS) $(TEST_CASES) *.xml
