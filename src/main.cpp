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

int main(int argc, const char * argv[]) {
    input(cs, vs, ms, sizes);
    int n = 100000;
    while(n>0) {
        iterate2(cs, vs, ms, sizes);
        n--;
    }
    
    return 0;
}


