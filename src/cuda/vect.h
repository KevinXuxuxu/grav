//
//  vect.h
//  grav
//
//  Created by XuFangzhou on 17/3/28.
//  Copyright © 2017年 XuFangzhou. All rights reserved.
//

#ifndef vect_h
#define vect_h

#ifdef __CUDACC__
#define CUDA_HOSTDEV __host__ __device__
#include <cuda.h>
#include <cuda_runtime.h>
#else
#define CUDA_HOSTDEV
#endif

#include <cmath>
//#include <pmmintrin.h>

struct Vect
{
    CUDA_HOSTDEV Vect()
    {}
    CUDA_HOSTDEV Vect(float	 _x, float	 _y = 0.0, float	 _z = 0.0)
    :x(_x), y(_y), z(_z)
    {}
    union
    {
        struct
        {
            float	 x, y, z;
        };
        // float	 c[3];
    };
    CUDA_HOSTDEV Vect operator +(Vect b) const
    {
        return Vect(x + b.x, y + b.y, z + b.z);
    }
    CUDA_HOSTDEV  Vect operator +=(Vect b)
    {
        x += b.x;
        y += b.y;
        z += b.z;
        return *this;
    }
    CUDA_HOSTDEV  Vect operator -(Vect b) const
    {
        return Vect(x - b.x, y - b.y, z - b.z);
    }
   CUDA_HOSTDEV  Vect operator *(float	 k) const
    {
        return Vect(x * k, y * k, z * k);
    }
    CUDA_HOSTDEV friend Vect operator *(float	 k, Vect v)
    {
        return Vect(v.x * k, v.y * k, v.z * k);
    }
    CUDA_HOSTDEV Vect operator /(float	 k) const
    {
        return Vect(x / k, y / k, z / k);
    }
    CUDA_HOSTDEV float	 operator &(Vect b) const
    {
        return x * b.x + y * b.y + z * b.z;
    }
    CUDA_HOSTDEV float abs() const
    {
        return sqrt(x * x + y * y + z * z);
    }
    Vect unit() const
    {
        return *this / abs();
    }
};
//float	 dotProduct(Vect a, Vect b)
//{
//    return a.x * b.x + a.y * b.y + a.z * b.z;
//}

#endif /* vect_h */
