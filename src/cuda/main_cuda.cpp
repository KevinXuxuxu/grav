//
//  main.cpp
//  grav
//
//  Created by XuFangzhou on 17/3/28.
//  Copyright © 2017 XuFangzhou. All rights reserved.
//

#include <iostream>


// #include <GLUT/glut.h>
// #include <OpenGL/gl.h>
//#include <GL/glut.h>

#include <cstdio>

#include <stdlib.h>

#include "gravity.hpp"

const int SamplingTimer = 1;
const int samplingrate = 40; //in milliseconds
const int iterationnum = 10;
const float	 PI = 3.14159265358979323846264338327950288419716939937510;
Vect* cs; //coordinates
Vect* vs; //velocity
float* ms; //mass
float* sizes;
Vect *dv, *dx;


int main(int argc, const char * argv[]) {
    input(&cs, &vs, &ms, &sizes, &dv, &dx);
    int n = 100000;
    while(n>0) {
        iterate_cuda(cs, vs, ms, sizes, dv, dx);
        n--;
    }
    // Destory rescource
	if(cs != NULL) free(cs);
    if(vs != NULL) free(vs);
    if(ms != NULL) free(ms);
    if(sizes != NULL) free(sizes);
    if(dv != NULL) free(dv);
    if(dx != NULL) free(dx);
    return 0;
}


