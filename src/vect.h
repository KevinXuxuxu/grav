//
//  vect.h
//  grav
//
//  Created by XuFangzhou on 17/3/28.
//  Copyright © 2017年 XuFangzhou. All rights reserved.
//

#ifndef vect_h
#define vect_h


#include <cmath>
#include <pmmintrin.h>

struct Vect
{
    Vect()
    {}
    Vect(double _x, double _y = 0.0, double _z = 0.0)
    :x(_x), y(_y), z(_z)
    {}
    union
    {
        struct
        {
            double x, y, z;
        };
        double c[3];
    };
    Vect operator +(Vect b) const
    {
        return Vect(x + b.x, y + b.y, z + b.z);
    }
    Vect operator +=(Vect b)
    {
        x += b.x;
        y += b.y;
        z += b.z;
        return *this;
    }
    Vect operator -(Vect b) const
    {
        return Vect(x - b.x, y - b.y, z - b.z);
    }
    Vect operator *(double k) const
    {
        return Vect(x * k, y * k, z * k);
    }
    friend Vect operator *(double k, Vect v)
    {
        return Vect(v.x * k, v.y * k, v.z * k);
    }
    Vect operator /(double k) const
    {
        return Vect(x / k, y / k, z / k);
    }
    double operator &(Vect b) const
    {
        return x * b.x + y * b.y + z * b.z;
    }
    double abs() const
    {
        return sqrt(x * x + y * y + z * z);
    }
    Vect unit() const
    {
        return *this / abs();
    }
};
//double dotProduct(Vect a, Vect b)
//{
//    return a.x * b.x + a.y * b.y + a.z * b.z;
//}

#endif /* vect_h */
