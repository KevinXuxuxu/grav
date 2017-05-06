//
//  main.cpp
//  grav
//
//  Created by XuFangzhou on 17/3/28.
//  Copyright © 2017年 XuFangzhou. All rights reserved.
//

#include <iostream>


// #include <GLUT/glut.h>
// #include <OpenGL/gl.h>
//#include <GL/glut.h>

#include <cstdio>

#include "gravity.hpp"

const int SamplingTimer = 1;
const int samplingrate = 40; //in milliseconds
const int iterationnum = 10;
const float	 PI = 3.14159265358979323846264338327950288419716939937510;
// Vect* cs; //coordinates
// Vect* vs; //velocity
// float* ms; //mass
// float* sizes;
// Vect *dv, *dx;

Body* body;
Force* force;

int main(int argc, const char * argv[]) {
    // input(&cs, &vs, &ms, &sizes, &dv, &dx);
    input(&body, &force);
    int n = 4;
    while(n>0) {
        // iterate2(cs, vs, ms, sizes, dv, dx);
        iterateOctree(n);
        n--;
    }
    // Destory rescource
	// if(cs != NULL) free(cs);
 //    if(vs != NULL) free(vs);
 //    if(ms != NULL) free(ms);
 //    if(sizes != NULL) free(sizes);
 //    if(dv != NULL) free(dv);
 //    if(dx != NULL) free(dx);

    if(body != NULL) delete[] body;
    if(force != NULL) delete [] force;

    return 0;
}