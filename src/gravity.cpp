//
//  gravity.cpp
//  grav
//
//  Created by XuFangzhou on 17/3/28.
//  Copyright © 2017年 XuFangzhou. All rights reserved.
//
#ifndef GRAVITY_CPP
#define GRAVITY_CPP

#include <iostream>
#include <fstream>
#include <iostream>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <algorithm>
#include <sys/time.h>


#include "gravity.hpp"

//#include "vect.h"
////#include "octtree.h"
//#include "body.hpp"
//#include "body.cpp"

using namespace std;

#define NUM_THREADS 8

int n;
long long T;

BodyX body[N];

void input(BodyX *a)
{
    ifstream fin("config.in");
    char xx[N];
    fin>>xx>>xx>>xx>>n;
    fin>>xx>>T>>xx;
    printf("T: %lld\nn: %d", T, n);
    for(int i=0;i<n;i++)
    {
        fin>>a[i].c.x>>a[i].c.y>>a[i].c.z>>a[i].v.x>>a[i].v.y>>a[i].v.z>>a[i].m;
        a[i].size = 0.2 * pow(a[i].m, 1.0/3.0);
    }
}
Force calForce(const BodyX &a, BodyX b)
{
    Force f;
    float	 r2 = (a.c - b.c) & (a.c - b.c);
    float	 r = (a.c - b.c).abs();
    if (r == 0.0)
        return Force(0);
    f = (b.c - a.c) / r * (G * a.m * b.m / r2);
    //printf("Force (%.2lf, %.2lf, %.2lf)\n", f.x, f.y, f.z);
    return f;
}
Vect dv[N], dx[N];

void collide(const BodyX &a, const BodyX &b, Vect &vat, Vect &vbt)
{
    Vect e = a.c - b.c;
    e = e / e.abs();
    float	 A, B;
    A = a.v & e;
    B = b.v & e;
    Vect ar, br;
    ar = a.v - A * e;
    br = b.v - B * e;
    float	 Al, Bl;
    Al = (A * (a.m - b.m) + 2 * b.m * B) / (a.m + b.m);
    Bl = (B * (b.m - a.m) + 2 * a.m * A) / (a.m + b.m);
    vat = ar + Al * e;
    vbt = br + Bl * e;
}

Vect caldv(BodyX b, BodyX a) //impulse of a on b
{
    Vect dx = a.c - b.c;
    Vect dv = a.v - b.v;
    float	 r = dx.abs();
    return G * a.m * (
                      dx / (r * r * r) * dt +
                      1.0 / 2.0 * dv / (r * r * r) * dt * dt +
                      3.0 / 2.0 * (dx & dv) * dx / (r * r * r * r * r) * dt * dt
                      );
}
Vect caldx(BodyX b, BodyX a) //impulse of a on b
{
    Vect dx = a.c - b.c;
    Vect dv = a.v - b.v;
    float	 r = dx.abs();
    return G * a.m * (
                      1.0 / 2.0 * dx / (r * r * r) * dt * dt +
                      1.0 / 6.0 * dv / (r * r * r) * dt * dt * dt +
                      1.0 / 2.0 * (dx & dv) * dx / (r * r * r * r * r) * dt * dt * dt
                      );
}
void iterate2(BodyX *a)
{
    cout << "****************iterate**********************\n";
    struct timeval t_start;
    gettimeofday(&t_start, NULL);

    double	 time_start = (t_start.tv_sec) * 1000 + (t_start.tv_usec) / 1000 ; 

    int i, j;
 /* Cal gravity */
    #pragma omp parallel num_threads(NUM_THREADS)
    for (i = 0; i < n; i++)
    {
        dv[i] = Vect(0, 0, 0);
        dx[i] = Vect(0, 0, 0);
        for (j = 0; j < n; j++)
        {
            if(j!=i)
            {
                dv[i] += caldv(a[i], a[j]);
                dx[i] += a[i].v * dt + caldx(a[i], a[j]);
            }
        }
    }
    /* End of Cal gravity */

    struct timeval t_after_grav;
    gettimeofday(&t_after_grav, NULL);

    double	 time_after_grav = (t_after_grav.tv_sec) * 1000 + (t_after_grav.tv_usec) / 1000 ; 
    
    /* Cal collide */
    
    Vect vit, vjt;
    for (i = 0; i < n; i++)
        for (j = i + 1; j < n; j++)
        {
            if (((a[i].c + dx[i]) - (a[j].c + dx[j])).abs() <= a[i].size + a[j].size)
            {
                collide(a[i], a[j], vit, vjt);
                a[i].v = vit;
                a[j].v = vjt;
                dx[i] = Vect(0.0);
                dx[j] = Vect(0.0);
                dv[i] = Vect(0.0);
                dv[j] = Vect(0.0);
            }
        }
     /* End of Cal collide */
    struct timeval t_after_coll;
    gettimeofday(&t_after_coll, NULL);

    double	 time_after_coll = (t_after_coll.tv_sec) * 1000 + (t_after_coll.tv_usec) / 1000 ; 
    
    cout << "calculate gravity: " << (time_after_grav - time_start) << "ms\n";
    cout << "calculate collide: " << (time_after_coll - time_after_grav) << "ms\n";

    for (i = 0; i < n; i++)
    {
        a[i].c += dx[i];
        a[i].v += dv[i];
    }
}
//
//Octtree octtree;
//Force force[N];
//void iterateOcttree(BodyX *body)
//{
//    int i;
//    //(printf("iteration start\n");)
//    octtree.build(body, n, OcttreeThreshold);
//    //(printf("build complete\n");)
//    for (i = 0; i < n; i++)
//        force[i] = octtree.query(body[i]);
//    //(printf("queries complete\n");)
//    octtree.destroy();
//    //(printf("destroy complete\n");)
//    for (i = 0; i < n; i++)
//    {
//        body[i].apply(force[i]);
//    }
//    //printf("apply complete\n");
//}

#endif
