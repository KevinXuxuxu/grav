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
#include "body.hpp"

const double G = 1.73205;
const int N = 100;
const double OcttreeThreshold = 0.1;

extern int n;
extern long long T;

extern BodyX body[N];

//int n;
//long long T;
//
//BodyX body[N];


extern void input(BodyX *);
extern Force calForce(const BodyX &a, BodyX b);
extern void iterate(BodyX *a);
extern Vect caldv(BodyX b, BodyX a); //impulse of a on b
extern Vect caldx(BodyX b, BodyX a); //impulse of a on b
extern void collide(const BodyX &a, const BodyX &b, Vect &vat, Vect &vbt);
extern void iterate2(BodyX *a);
//extern void iterateOcttree(BodyX *body);

//#include "gravity.cpp"
#endif /* gravity_hpp */
