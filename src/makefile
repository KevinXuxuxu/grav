MAINOBJ = gravity.o body.o main_nogl.o
OPT = -g -O2 -fopenmp
CPC = mpic++

main: $(MAINOBJ)
	c++ $(MAINOBJ) -lGL -lglut -o main $(OPT)
	
main.o: main_nogl.cpp
	c++ main_nogl.cpp -c $(OPT)

gravity.o: gravity.cpp
	c++ gravity.cpp -c $(OPT)

body.o: body.cpp
	c++ body.cpp -c $(OPT)

#vect.o: vect.h
#	c++ vect.h -c $(OPT)

clean:
	-rm $(MAINOBJ)
