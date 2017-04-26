PROJ = main
CC = g++
NVCC = nvcc -O3


CFLAGS = -c -g -Wall -I/opt/local/include -I$(HOME)/cppunit/include -msse2 -msse3 -O2 -lm
LDFLAGS = -L/opt/local/lib -L$(HOME)/cppunit/lib
LIBS = -lcppunit -ldl
OBJS = $(patsubst %.cpp,%.o,$(wildcard *.cpp))


all: $(PROJ)

$(PROJ): $(OBJS)
		$(CC) $(OMPFLAGS) $(LDFLAGS) $^ -o $@ $(LIBS)

%.o : %.cpp
		$(CC) $(CFLAGS) $< -o $@ $(SIMDFLAGS) $(OMPFLAGS)

%.o : %.cpp %.h
		$(CC) $(CFLAGS) $< -o $@ $(SIMDFLAGS)

clean:
		rm -f $(PROJ) $(OBJS) *.xml
