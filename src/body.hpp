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

const double dt = 0.005;

struct Force
{
    Vect f;
    Vect dx, dv;
    bool dxdv;
    Force();
    Force(Vect _f);
    Force(Vect _dx, Vect _dv);
    Force operator +(Force b) const;
    Force operator +=(Force);
    Force operator *(double k) const;
    friend Force operator *(double k, Force v);
    Force operator /(double k) const;
};
struct BodyX
{
    Vect c;//coordinates
    Vect v;//velocity
    double m; //mass
    double size;
    BodyX();
    BodyX(Vect _c, Vect _v, double _m);
    void apply(Force f);
    BodyX ifApply(Force f);
};
#endif /* body_hpp */
