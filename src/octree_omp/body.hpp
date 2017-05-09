//
//  body.hpp
//  grav
//
//  Created by XuFangzhou on 17/3/28.
//  Copyright © 2017年 XuFangzhou. All rights reserved.
//

#ifndef body_hpp
#define body_hpp

#include <stdio.h>

#include "vect.h"

const float	 dt = 0.005;

struct Force
{
    Vect dx, dv;
    bool col;
    Force();
    Force(Vect _dx, Vect _dv, bool _col);
    Force operator +(Force &b) const;
    Force &operator +=(const Force &);
    void output() const;
};
struct Body
{
    Vect c;     //coordinates
    Vect v;     //velocity
    float m;    //mass
    float size;
    Body();
    Body(Vect _c, Vect _v, float _m);
    Body(Vect _c, Vect _v, float _m, float _size);
    bool overlap(Body &b);
    void apply(Force f);
    Body ifApply(Force f);
    void output() const;
};

#endif
