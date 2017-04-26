//
//  gravity.cpp
//  grav
//
//  Created by XuFangzhou on 17/3/28.
//  Copyright © 2017 XuFangzhou. All rights reserved.
//
#ifndef GRAVITY_CPP
#define GRAVITY_CPP

#include <iostream>
#include <fstream>
#include <iostream>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <sys/time.h>
#include <assert.h>     /* assert */


#include "gravity.hpp"

using namespace std;

int n;
long long T;

// BodyX body[N];
// Vect* cs; //coordinates
// Vect* vs; //velocity
// float* ms; //mass
// float* sizes;
// Vect *dv, *dx;

void input(Vect** cs, Vect** vs, float** ms, float** sizes, Vect **dv, Vect ** dx)
{
    ifstream fin("config.in");
    char xx[N];
    fin>>xx>>xx>>xx>>n;
    fin>>xx>>T>>xx;
    printf("T: %lld\nn: %d\n", T, n);
    *cs = (Vect *) malloc(sizeof(Vect) * n);
    *vs = (Vect *) malloc(sizeof(Vect) * n);
    *ms = (float *) malloc(sizeof(float) * n);
    *sizes = (float *) malloc(sizeof(float) * n);
    *dv = (Vect *) malloc(sizeof(Vect) * n);
    *dx = (Vect *) malloc(sizeof(Vect) * n);

    assert(*cs != NULL && *vs != NULL && *ms != NULL && *sizes != NULL && *dv != NULL && *dx != NULL);
    for(int i=0;i<n;i++)
    {   
        fin>>(*cs)[i].x;
        fin>>(*cs)[i].y>>(*cs)[i].z>>(*vs)[i].x>>(*vs)[i].y>>(*vs)[i].z>>(*ms)[i];
        (*sizes)[i] = 0.2 * pow((*ms)[i], 1.0/3.0);
    }
}



void collide(const Vect &a_c, const &Vect a_v, float a_m, 
    const Vect &b_c, const Vect &b_v, float b_m, 
    Vect &vat, Vect &vbt)
{
    Vect e = a_c - b_c;
    e = e / e.abs();
    float	 A, B;
    A = a_v & e;
    B = b_v & e;
    Vect ar, br;
    ar = a_v - A * e;
    br = b_v - B * e;
    float	 Al, Bl;
    Al = (A * (a_m - b_m) + 2 * b_m * B) / (a_m + b_m);
    Bl = (B * (b_m - a_m) + 2 * a_m * A) / (a_m + b_m);
    vat = ar + Al * e;
    vbt = br + Bl * e;
}

Vect caldv(const Vect &a_c,const  Vect &a_v,const  float a_m,const Vect &b_c, const Vect &b_v) //impulse of a on b
{
    Vect dx = a_c - b_c;
    Vect dv = a_v - b_v;
    float	 r = dx.abs();
    return G * a_m * (
                      dx / (r * r * r) * dt +
                      1.0 / 2.0 * dv / (r * r * r) * dt * dt +
                      3.0 / 2.0 * (dx & dv) * dx / (r * r * r * r * r) * dt * dt
                      );
}
Vect caldx(const Vect &a_c,const  Vect &a_v,const  float a_m,const Vect &b_c, const Vect &b_v) //impulse of a on b
{
    Vect dx = a_c - b_c;
    Vect dv = a_v - b_v;
    float	 r = dx.abs();
    return G * a_m * (
                      1.0 / 2.0 * dx / (r * r * r) * dt * dt +
                      1.0 / 6.0 * dv / (r * r * r) * dt * dt * dt +
                      1.0 / 2.0 * (dx & dv) * dx / (r * r * r * r * r) * dt * dt * dt
                      );
}
void iterate2(Vect* cs, Vect* vs, float* ms, float* sizes, Vect *dv, Vect *dx)
{
    cout << "****************iterate**********************\n";
    struct timeval t_start;
    gettimeofday(&t_start, NULL);

    double	 time_start = (t_start.tv_sec) * 1000 + (t_start.tv_usec) / 1000 ; 
    int i, j;
    /* Cal gravity */
    for (i = 0; i < n; i++)
    {
        dv[i] = Vect(0, 0, 0);
        dx[i] = Vect(0, 0, 0);
        for (j = 0; j < n; j++)
        {
            if(j!=i)
            {
                dv[i] += caldv(cs[i], vs[i], ms[i], cs[j], vs[j]);
                dx[i] += vs[i] * dt + caldx(cs[i], vs[i], ms[i], cs[j], vs[j]);
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
            if (((cs[i] + dx[i]) - (cs[j] + dx[j])).abs() <= sizes[i] + sizes[j])
            {
                collide(cs[i], vs[i], ms[i], cs[j], vs[j], ms[j], vit, vjt);
                vs[i] = vit;
                vs[j] = vjt;
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
    
    printf("calculate gravity: %f ms\n", (time_after_grav - time_start));
    printf("calculate collide: %f ms\n", (time_after_coll - time_after_grav));

    for (i = 0; i < n; i++)
    {
        cs[i] += dx[i];
        vs[i] += dv[i];
    }
}



#endif
