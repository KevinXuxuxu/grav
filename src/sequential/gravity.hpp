//
//  gravity.hpp
//  grav
//
//  Created by XuFangzhou on 17/3/28.
//  Copyright © 2017 XuFangzhou. All rights reserved.
//

#ifndef gravity_hpp
#define gravity_hpp

#include <stdio.h>

#include "vect.h"
#include "body.hpp"

const float	 G = 1.73205;
const int N = 100000;
const float	 OcttreeThreshold = 0.1;

extern int n;
extern long long T;

// extern BodyX body[N];

extern Vect* cs; //coordinates
extern Vect* vs; //velocity
extern float* ms; //mass
extern float* sizes;
extern Vect *dv, *dx;

extern void input(Vect** cs, Vect** vs, float** ms, float** sizes, Vect **dv, Vect ** dx);
extern Vect caldv(const Vect &b_c,const  Vect &b_v,const Vect &a_c, const Vect &a_v, const  float a_m); //impulse of a on b
extern Vect caldx(const Vect &b_c,const  Vect &b_v,const Vect &a_c, const Vect &a_v, const  float a_m); //impulse of a on b
extern void collide(const Vect &a_c, const Vect &a_v, float a_m, 
    const Vect &b_c, const Vect &b_v, float b_m, 
    Vect &vat, Vect &vbt);
extern void iterate2(Vect* cs, Vect* vs, float* ms, float* sizes,  Vect *dv, Vect *dx);
//extern void iterateOcttree(BodyX *body);

//#include "gravity.cpp"
#endif /* gravity_hpp */
