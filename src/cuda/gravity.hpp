//
//  gravity.hpp
//  grav
//
//  Created by XuFangzhou on 17/3/28.
//  Copyright © 2017年 XuFangzhou. All rights reserved.
//

#ifndef gravity_hpp
#define gravity_hpp

#include <stdio.h>

#include "vect.h"

const float	 G = 1.73205;
const int N = 100000;
const float	 OcttreeThreshold = 0.1;

extern int n;
extern long long T;
const float	 dt = 0.005;

// extern BodyX body[N];

extern Vect* cs; //coordinates
extern Vect* vs; //velocity
extern float* ms; //mass
extern float* sizes;
extern Vect *dv, *dx;

extern void input(Vect** cs, Vect** vs, float** ms, float** sizes, Vect **dv, Vect ** dx);


extern void iterate_cuda(Vect* cs, Vect* vs, float* ms, float* sizes,  Vect *dv, Vect *dx);
//extern void iterateOcttree(BodyX *body);

//#include "gravity.cpp"
#endif /* gravity_hpp */
