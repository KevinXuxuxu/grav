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
#include "octree.hpp"

using namespace std;

int n;
long long T;

Octree octree;

void input(Body** body, Force** force)
{
    ifstream fin("config.in");
    char xx[N];
    fin>>xx>>xx>>xx>>n;
    fin>>xx>>T>>xx;
    printf("T: %lld\nn: %d\n", T, n);

    *body = new Body[n];
    *force = new Force[n];

    assert(*body != NULL && *force != NULL);

    for(int i=0;i<n;i++)
    {
        fin>>(*body)[i].c.x>>(*body)[i].c.y>>(*body)[i].c.z>>(*body)[i].v.x>>(*body)[i].v.y>>(*body)[i].v.z>>(*body)[i].m;
        (*body)[i].size = 0.2 * pow((*body)[i].m, 1.0/3.0);
    }
}

void collide(const Body &a, const Body &b, Vect &vat, Vect &vbt)
{
    Vect e = a.c - b.c;
    e = e / e.abs();
    float A, B;
    A = a.v & e;
    B = b.v & e;
    Vect ar, br;
    ar = a.v - A * e;
    br = b.v - B * e;
    float Al, Bl;
    Al = (A * (a.m - b.m) + 2 * b.m * B) / (a.m + b.m);
    Bl = (B * (b.m - a.m) + 2 * a.m * A) / (a.m + b.m);
    vat = ar + Al * e;
    vbt = br + Bl * e;
}

Vect caldv(Body b, Body a) //impulse of a on b
{
    Vect dx = a.c - b.c;
    Vect dv = a.v - b.v;
    float r = dx.abs();
    return G * a.m * (
        dx / (r * r * r) * dt +
        1.0 / 2.0 * dv / (r * r * r) * dt * dt +
        3.0 / 2.0 * (dx & dv) * dx / (r * r * r * r * r) * dt * dt
        );
}
Vect caldx(Body b, Body a) //impulse of a on b
{
    Vect dx = a.c - b.c;
    Vect dv = a.v - b.v;
    float r = dx.abs();
    return G * a.m * (
        1.0 / 2.0 * dx / (r * r * r) * dt * dt +
        1.0 / 6.0 * dv / (r * r * r) * dt * dt * dt +
        1.0 / 2.0 * (dx & dv) * dx / (r * r * r * r * r) * dt * dt * dt
        );
}
Force calForce2(const Body &a, Body b)
{
    if ((a.c - b.c).abs() == 0.0)
        return Force(0, 0, false);
    Force f = Force(caldx(a, b), caldv(a, b), false);
    if (isnan(f.dx.x) || isnan(f.dv.x))
    {
        printf("NAN Force!!!\n");
        a.output();
        b.output();
        while (1);
    }
    return Force(caldx(a, b), caldv(a, b), false);
}

void iterateOctree(int iteration) {
    printf("****************iterate**********************\n");
    struct timeval t_start;
    gettimeofday(&t_start, NULL);

    double   time_start = (t_start.tv_sec) * 1000 + (t_start.tv_usec) / 1000 ; 

    //build the octree
    octree.build(body, n, OctreeThreshold);

    for (int i = 0; i < n; i++)
    {
        force[i] = octree.query(body[i]);
        // printf("force[%d] = ", i);
        // force[i].output();
    }

    struct timeval t_after;
    gettimeofday(&t_after, NULL);

    double   time_after = (t_after.tv_sec) * 1000 + (t_after.tv_usec) / 1000 ; 

    printf("calculate total: %f ms\n", (time_after - time_start));

    for (int i = 0; i < n; i++)
    {
        body[i].apply(force[i]);
    }

    // destroy the octree
    octree.destroy();

    if (iteration == 1)
    {
        octree.destroyAux();
    }
}


#endif
