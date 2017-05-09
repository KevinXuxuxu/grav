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
    Vect(float	 _x, float	 _y = 0.0, float	 _z = 0.0)
    :x(_x), y(_y), z(_z)
    {}
    union
    {
        struct
        {
            float	 x, y, z;
        };
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
    Vect operator *(float	 k) const
    {
        return Vect(x * k, y * k, z * k);
    }
    friend Vect operator *(float	 k, Vect v)
    {
        return Vect(v.x * k, v.y * k, v.z * k);
    }
    Vect operator /(float	 k) const
    {
        return Vect(x / k, y / k, z / k);
    }
    float	 operator &(Vect b) const
    {
        return x * b.x + y * b.y + z * b.z;
    }
    float	 abs() const
    {
        return sqrt(x * x + y * y + z * z);
    }
    Vect unit() const
    {
        return *this / abs();
    }
};

#endif /* vect_h */
