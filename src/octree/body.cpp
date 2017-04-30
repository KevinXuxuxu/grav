//
//  body.cpp
//  grav
//
//  Created by XuFangzhou on 17/3/28.
//  Copyright © 2017年 XuFangzhou. All rights reserved.
//

#include "body.hpp"

Force::Force()
{}
Force::Force(Vect _f)
:f(_f), dxdv(false)
{}
Force::Force(Vect _dx, Vect _dv)
:dx(_dx), dv(_dv), dxdv(true)
{}
Force Force::operator +(Force b) const
{
    if (dxdv)
        return *this;
    if (b.dxdv)
        return b;
    return Force(f + b.f);
}
Force Force::operator +=(Force b)
{
    if (dxdv)
        return *this;
    f += b.f;
    if (b.dxdv)
        *this = b;
    return *this;
}
Force Force::operator *(float	 k) const
{
    return Force(f * k);
}
Force operator *(float	 k, Force f)
{
    return Force(f.f * k);
}
Force Force::operator /(float	 k) const
{
    return Force(f / k);
}

BodyX::BodyX()
{}
BodyX::BodyX(Vect _c, Vect _v, float	 _m)
:c(_c), v(_v), m(_m)
{}
void BodyX::apply(Force f)
{
    if (f.dxdv)
    {
        c += f.dx;
        v += f.dv;
        //printf("collision force applied\n");
    }
    else
    {
        c += v * dt + 0.5 * f.f / m * dt * dt;
        v += f.f * dt / m;
    }
}
BodyX BodyX::ifApply(Force f)
{
    BodyX b = *this;
    b.apply(f);
    return b;
}
