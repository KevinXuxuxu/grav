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

const float	 G = 1.73205;
const int N = 16000;
const float OctreeThreshold = 0.05;

extern int n;
extern long long T;

extern Body *body;
extern Force *force;

extern void input(Body** body, Force** force);
extern void collide(const Body &a, const Body &b, Vect &vat, Vect &vbt);
extern Vect caldv(Body b, Body a); //impulse of a on b
extern Vect caldx(Body b, Body a); //impulse of a on b
extern Force calForce2(const Body &a, Body b);
extern void iterateOctree(int iteration);

#endif /* gravity_hpp */
