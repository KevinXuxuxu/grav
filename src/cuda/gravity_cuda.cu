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
#include <cuda.h>
#include <cuda_runtime.h>

#include "gravity.hpp"

using namespace std;
#define NUM_THREADS_PER_BLOCK 1024

int n;
long long T;

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



void collide(const Vect &a_c, const Vect &a_v, float a_m, 
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

/* impulse of a on b */
__host__ __device__
Vect caldv(const Vect &b_c,const  Vect &b_v,const Vect &a_c, const Vect &a_v, const  float a_m)
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

/* impulse of a on b */
__host__ __device__
Vect caldx(const Vect &b_c,const  Vect &b_v,const Vect &a_c, const Vect &a_v, const  float a_m) 
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


//     for (j = 0; j < n; j++)
//     {
//         if(j!=i)
//         {
//             dv[i] += caldv(cs[i], vs[i], cs[j], vs[j], ms[j]);
//             dx[i] += vs[i] * dt + caldx(cs[i], vs[i], cs[j], vs[j], ms[j]);
//         }
//     }
// }
__global__
void
init_d_kernel(Vect * d,int n) {
    int index = blockDim.x * blockIdx.x + threadIdx.x;
    if(index < n) {
        d[index].x = 0.0f;
        d[index].y = 0.0f;
        d[index].z = 0.0f;
    }
}


__global__
void
cal_gravity_kernel(Vect * cs_d,Vect * vs_d,float* ms_d,float* sizes_d,Vect * dv_d,Vect *  dx_d,int n,int GRID_SIZE, int i) {
    int index = blockDim.x * blockIdx.x + threadIdx.x;

    if(index < n && i != index) {
        dv_d[i] += caldv(cs_d[i], vs_d[i], cs_d[index], vs_d[index], ms_d[index]);
        dx_d[i] += vs_d[i] * dt + caldx(cs_d[i], vs_d[i], cs_d[index], vs_d[index], ms_d[index]);
    }
}

void iterate_cuda(Vect* cs, Vect* vs, float* ms, float* sizes, Vect *dv, Vect *dx) {
    printf("****************iterate CUDA**********************\n");
    struct timeval t_start;
    gettimeofday(&t_start, NULL);

    double   time_start = (t_start.tv_sec) * 1000 + (t_start.tv_usec) / 1000 ; 
    int i, j;
    /* Cal gravity */

    Vect *cs_d, *vs_d, *dv_d, *dx_d;
    float *ms_d, *sizes_d;
    int vects_size = n * sizeof(Vect), floats_size = n * sizeof(float);

    /* copy to device memory */
    cudaMalloc((void**) &cs_d, vects_size);
    cudaMalloc((void**) &vs_d, vects_size);
    cudaMalloc((void**) &ms_d, floats_size);
    cudaMalloc((void**) &sizes_d, floats_size);
    cudaMemcpy(cs_d, cs, vects_size, cudaMemcpyHostToDevice);
    cudaMemcpy(vs_d, vs, vects_size, cudaMemcpyHostToDevice);
    cudaMemcpy(ms_d, ms, floats_size, cudaMemcpyHostToDevice);
    cudaMemcpy(sizes_d, sizes, floats_size, cudaMemcpyHostToDevice);
    // allocate result matrix on device
    cudaMalloc((void**) &dv_d, floats_size);
    cudaMalloc((void**) &dx_d, floats_size);

    // dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);
    // int GRID_SIZE = (int)ceil((double)n / (double)BLOCK_SIZE);
    // dim3 dimGrid(GRID_SIZE, GRID_SIZE);
    int GRID_SIZE = (int)ceil((double)n / (double)NUM_THREADS_PER_BLOCK);

    init_d_kernel<<<GRID_SIZE, NUM_THREADS_PER_BLOCK>>>(dx_d, n);
    init_d_kernel<<<GRID_SIZE, NUM_THREADS_PER_BLOCK>>>(dv_d, n);

    for(int i = 0;i < n;i ++) {
        cal_gravity_kernel<<<GRID_SIZE, NUM_THREADS_PER_BLOCK>>>(cs_d, vs_d, ms_d, sizes_d, dv_d, dx_d, n, GRID_SIZE, i);   
    }
    // cal_gravity_kernel<<<dimGrid, dimBlock>>>(cs_d, vs_d, ms_d, sizes_d, dv_d, dx_d, n, GRID_SIZE);
    cudaMemcpy(dv, dv_d, floats_size, cudaMemcpyDeviceToHost);
    cudaMemcpy(dx, dx_d, floats_size, cudaMemcpyDeviceToHost);

    cudaFree(cs_d);
    cudaFree(vs_d);
    cudaFree(ms_d);
    cudaFree(sizes_d);
    cudaFree(dx_d);
    cudaFree(dv_d);

    /* End of Cal gravity */

    struct timeval t_after_grav;
    gettimeofday(&t_after_grav, NULL);

    double   time_after_grav = (t_after_grav.tv_sec) * 1000 + (t_after_grav.tv_usec) / 1000 ; 
    
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

    double   time_after_coll = (t_after_coll.tv_sec) * 1000 + (t_after_coll.tv_usec) / 1000 ; 
    
    printf("calculate gravity: %f ms\n", (time_after_grav - time_start));
    printf("calculate collide: %f ms\n", (time_after_coll - time_after_grav));

    for (i = 0; i < n; i++)
    {
        cs[i] += dx[i];
        vs[i] += dv[i];
    }

    
}

void iterate2(Vect* cs, Vect* vs, float* ms, float* sizes, Vect *dv, Vect *dx)
{
    printf("****************iterate**********************\n");
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
                dv[i] += caldv(cs[i], vs[i], cs[j], vs[j], ms[j]);
                dx[i] += vs[i] * dt + caldx(cs[i], vs[i], cs[j], vs[j], ms[j]);
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
