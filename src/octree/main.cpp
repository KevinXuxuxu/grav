//
//  main.cpp
//  grav
//
//  Created by XuFangzhou on 17/3/28.
//  Copyright © 2017年 XuFangzhou. All rights reserved.
//

#include <iostream>
#include <cstdio>
#include "gravity.hpp"

const int SamplingTimer = 1;
const int samplingrate = 40; //in milliseconds
const int iterationnum = 10;
const float	 PI = 3.14159265358979323846264338327950288419716939937510;

Body* body;
Force* force;

int main(int argc, const char * argv[]) {

    input(&body, &force);
    int n = 4;
    while(n>0) {
        iterateOctree(n);
        n--;
    }

    // Destory rescource
    if(body != NULL) delete[] body;
    if(force != NULL) delete [] force;

    return 0;
}
