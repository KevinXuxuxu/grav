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


__host__ __device__
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


__global__
void
init_d_kernel(Vect * d,int n) {
    int index = blockDim.x * blockIdx.x + threadIdx.x;
    if(index < n) {
        d[index] = Vect(0.0f, 0.0f, 0.0f);
    }
}


__global__
void
cal_gravity_kernel(Vect * cs_d,Vect * vs_d,float* ms_d,float* sizes_d,Vect * dv_d,Vect *  dx_d,int n,int GRID_SIZE, int i) {
    int index = blockDim.x * blockIdx.x + threadIdx.x;
    __shared__ Vect dv_s, dx_s, cs_d_i, vs_d_i;
    __shared__ float dt_s;
    dv_s = Vect(0.0f, 0.0f, 0.0f);
    dx_s = Vect(0.0f, 0.0f, 0.0f);
    cs_d_i = cs_d[i];
    vs_d_i = vs_d[i];
    dt_s = dt;
    __syncthreads(); // 
    if(index < n && i != index) {
        dv_s += caldv(cs_d_i, vs_d_i, cs_d[index], vs_d[index], ms_d[index]);
        dx_s += vs_d_i * dt_s + caldx(cs_d_i, vs_d_i, cs_d[index], vs_d[index], ms_d[index]);
    }

    __syncthreads();
    dv_d[i] = dv_s;
    dx_d[i] = dx_s;
}


__global__
void 
cal_collide_kernel(Vect * cs_d,Vect * vs_d,float* ms_d,float* sizes_d,Vect * dv_d,Vect *  dx_d,int n,int GRID_SIZE, int i) {
    int index = blockDim.x * blockIdx.x + threadIdx.x;
    if(index > i && index < n) {
        if(((cs_d[i] + dx_d[i]) - (cs_d[index] + dx_d[index])).abs() <= sizes_d[i] + sizes_d[index]) {
            Vect vit, vjt;
            collide(cs_d[i], vs_d[i], ms_d[i], cs_d[index], vs_d[index], ms_d[index], vit, vjt);
            vs_d[i] = vit;
            vs_d[index] = vjt;
            dx_d[i] = Vect(0.0);
            dx_d[index] = Vect(0.0);
            dv_d[i] = Vect(0.0);
            dv_d[index] = Vect(0.0);

        }
    }

}


void iterate_cuda(Vect* cs, Vect* vs, float* ms, float* sizes, Vect *dv, Vect *dx) {
    printf("****************iterate CUDA**********************\n");
    struct timeval t_start;
    gettimeofday(&t_start, NULL);

    double   time_start = (t_start.tv_sec) * 1000 + (t_start.tv_usec) / 1000 ; 
    int i;
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

    
    int GRID_SIZE = (int)ceil((double)n / (double)NUM_THREADS_PER_BLOCK);

    for(int i = 0;i < n;i ++) {
        cal_gravity_kernel<<<GRID_SIZE, NUM_THREADS_PER_BLOCK>>>(cs_d, vs_d, ms_d, sizes_d, dv_d, dx_d, n, GRID_SIZE, i);   
    }

    /* End of Cal gravity */

    /* Cal collide */
    
    for (i = 0; i < n; i++) {
        cal_collide_kernel<<<GRID_SIZE, NUM_THREADS_PER_BLOCK>>>(cs_d, vs_d, ms_d, sizes_d, dv_d, dx_d, n, GRID_SIZE, i); 

    }

    struct timeval t_after_grav;
    gettimeofday(&t_after_grav, NULL);

    double   time_after_grav = (t_after_grav.tv_sec) * 1000 + (t_after_grav.tv_usec) / 1000 ; 
    
     /* End of Cal collide */
    // cal_gravity_kernel<<<dimGrid, dimBlock>>>(cs_d, vs_d, ms_d, sizes_d, dv_d, dx_d, n, GRID_SIZE);
    cudaMemcpy(dv, dv_d, floats_size, cudaMemcpyDeviceToHost);
    cudaMemcpy(dx, dx_d, floats_size, cudaMemcpyDeviceToHost);
    cudaMemcpy(vs, vs_d, vects_size, cudaMemcpyDeviceToHost);

    cudaFree(cs_d);
    cudaFree(vs_d);
    cudaFree(ms_d);
    cudaFree(sizes_d);
    cudaFree(dx_d);
    cudaFree(dv_d);

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
#endif
