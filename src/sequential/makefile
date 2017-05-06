PROJ = main
CC = g++

CFLAGS = -c -g -Wall -I/opt/local/include
LDFLAGS = -L/opt/local/lib
LIBS = -lcppunit -ldl
OBJS = $(patsubst %.cpp,%.o,$(wildcard *.cpp))
SIMDFLAGS = -msse2 -msse3 -O2 -lm

all: $(PROJ)

$(PROJ): $(OBJS)
		$(CC) $(OMPFLAGS) $(LDFLAGS) $^ -o $@ $(LIBS)

%.o : %.cpp
		$(CC) $(CFLAGS) $< -o $@ $(SIMDFLAGS)

%.o : %.cpp %.h
		$(CC) $(CFLAGS) $< -o $@ $(SIMDFLAGS)

clean:
		rm -f $(PROJ) $(OBJS) *.xml