//
//  body.cpp
//  grav
//
//  Created by XuFangzhou on 17/3/28.
//  Copyright © 2017年 XuFangzhou. All rights reserved.
//

#include "body.hpp"

Force::Force()
    :dx(0), dv(0), col(false)
{}
Force::Force(Vect _dx, Vect _dv, bool _col = false)
    :dx(_dx), dv(_dv), col(_col)
{/*
    if (isnan(dx.x) || isnan(dv.x))
    {
        dx = 0;
        dv = 0;
        printf("NAN Force!!!\n");
    }*/
}
Force Force::operator +(Force &b) const
{/*
    if (col)
        return *this;
    if (b.col)
        return b;*/
    return Force(dx + b.dx, dv + b.dv, false);
}
Force &Force::operator +=(const Force &b)
{
    //if (col)
    //  return *this;
    dx += b.dx;
    dv += b.dv;
    //if (b.col)
    //  *this = b;
    return *this;
}
void Force::output() const
{
    //if (dxdv)
        printf("Force (%.2lg, %.2lg, %.2lg) (%.2lg, %.2lg, %.2lg) col %d\n", dv.x, dv.y, dv.z, dx.x, dx.y, dx.z, (int)col);
    //else
    //  printf("Force (%.2lf, %.2lf, %.2lf)\n", f.x, f.y, f.z);
}

Body::Body()
{}
Body::Body(Vect _c, Vect _v, float _m)
    :c(_c), v(_v), m(_m)
{
    size = 0.2 * pow(m, 1.0/3.0);
}
Body::Body(Vect _c, Vect _v, float _m, float _size)
    :c(_c), v(_v), m(_m), size(_size)
{}
bool Body::overlap(Body &b)
{
    return (c - b.c).abs() < size + b.size;
}
void Body::apply(Force f)
{
    c += v * dt + f.dx;
    v += f.dv;
}
Body Body::ifApply(Force f)
{
    Body b = *this;
    b.apply(f);
    return b;
}
void Body::output() const
{
    printf("Body m %.4lf (%.2lg, %.2lg, %.2lg) (%.2lg, %.2lg, %.2lg)\n", m, c.x, c.y, c.z, v.x, v.y, v.z);
}