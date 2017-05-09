Multi-body Gravity System Simulation
> 18645 term project
> Fangzhou Xu (fangzhox) / Yuhao Xu (yuhaox1) / Shuang Chen (shuangc)

### Overview

Multi-body gravity system is a major problem in the field of Mechanics and Astrophysics. Simulations of this kind of system requires tracking multiple physical values (position, velocity, mass) of each body and calculating gravity interactions between each pair of bodies. Using basic mechanics principles, we can calculate the displacement of each body at each frame.

### code structure

All source codes are in the `src` folder. In the folder, each version of code (with different optimization method) is contained in the correspondent folder.

### Compile and run

- Data generate: use the Python program `data_gen.py` in each folder to generate random input data for test run.
    - for `octree` and `octree_omp`: pass parameter n into python program
    ```
    python data_gen.py <n>
    ```
    this will generate `8000 * n` number of bodies in `config.in` file.
    - for other versions: pass parameter n into python program
    ```
    python data_gen.py <n>
    ```
    this will generate `1000 * n` number of bodies in `config.in` file.

- Compile and run:
    - compile code by running the flowing shell command in the source code folder:
    ```
    make clean
    make
    ```
    this will generate an executable file `main` which will read input from `config.in` and run the simulation.
    ```
    ./main
    ```