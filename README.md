# oneAPI Direct Programming
This repository contains a collection of data-parallel programs for evaluating oneAPI direct programming. Each program is written with CUDA, SYCL, and OpenMP target offloading. Intel<sup>®</sup> DPC++ Compatibility Tool (DPCT) can convert a CUDA program to a SYCL program in which memory management migration is implemented using the explicit and restricted Unified Shared Memory extension or the DPCT header files.


# Experiments
We compare the performance of the SYCL, DPCT-generated, and OpenMP implementations of each program. The performance results below were obtained with the [Intel OpenCL intercept layer](https://github.com/intel/opencl-intercept-layer). "total enqueue" indicates the total number of low-level OpenCL enqueue commands called by a SYCL program. These enqueue commands include clEnqueueNDRangeKernel, clEnqueueReadBuffer, and clEnqueueWriteBuffer. The host timing is the total elapsed time of executing OpenCL API functions on a host while the device timing is the total elapsed time of executing OpenCL API functions on a GPU. The Plugin Interface is OpenCL.  
 
## Setup
Software: Intel<sup>®</sup> oneAPI Beta08 Toolkit, Ubuntu 18.04  
Platform 1: Intel<sup>®</sup> Xeon E3-1284L with a Gen8 P6300 integrated GPU  
Platform 2: Intel<sup>®</sup> Xeon E-2176G with a Gen9.5 UHD630 integrated GPU

## Run
A script "run.sh" attempts to run all tests with the OpenCL plugin interface. To run a single test, go to a test directory and type the command "make run".  

## Results on Platform 1
| amgmk | SYCL | DPCT usm | DPCT header | OpenMP |     
| --- | --- | --- | --- | --- |            
| total enqueue | 501 | 506 | 506 | 2010 | 
| host timing(s) | 0.41 | 0.88 | 0.88 | 3.78 | 
| device timing(s) | 0.18 | 0.18 | 0.18 | 0.18 |  


| aobench | SYCL | DPCT usm | DPCT header | OpenMP | 
| --- | --- | --- | --- | --- |
| total enqueue | 20 | 30 | 30 | 85 |
| host timing(s) | 0.58 | 0.92 | 0.95 | 3.71 | 
| device timing(s) | 0.14 | 0.14 | 0.14 | 0.16 |  


| bezier-surface | SYCL | DPCT usm | DPCT header | OpenMP | 
| --- | --- | --- | --- | --- |
| total enqueue | 2 | 3 | 3 | 12 |
| host timing(s) | 1.5 | 1.79 | 1.87 | 4.47 | 
| device timing(s) | 0.7 | 0.71 | 0.72 | 0.75 |  


| bitonic-sort | SYCL | DPCT usm | DPCT header| OpenMP | 
| --- | --- | --- | --- | --- |
| total enqueue | 326 | 327 | 327 | 1957 |
| host timing(s) | 2.21 | 2.56 | 2.67 | 5.85 | 
| device timing(s) | 1.92 | 1.93 | 1.97 | 2.36 |  


| black-scholes | SYCL | DPCT usm | DPCT header | OpenMP | 
| --- | --- | --- | --- | --- |
| total enqueue | 2 | 3 | 3 | 9 |
| host timing(s) | 0.57 | 1.42 | 1.46 | 4.67 | 
| device timing(s) | 0.16 | 0.35 | 0.34 | 0.95 |  


| chi2 | SYCL | DPCT usm | DPCT header | OpenMP | 
| --- | --- | --- | --- | --- |
| total enqueue | 2 | 3 | 3 | 12 |
| host timing(s) | 1.1 | 1.41 | 1.47 | 4.5 |
| device timing(s) | 0.19 | 0.23 | 0.35 | 0.92 |


| clenergy | SYCL | DPCT usm | DPCT header | OpenMP | 
| --- | --- | --- | --- | --- |
| total enqueue | 502 | 502  | 502 | 3011 |
| host timing(s) | 12.3 | 11.6 | 11.8 | 14.9 |
| device timing(s) | 11.8 | 10.8 | 10.9 | 11.2  |


| gamma-correction | SYCL | DPCT usm | DPCT header | OpenMP | 
| --- | --- | --- | --- | --- |
| total enqueue | 2 | 3 | 3 | 9 |
| host timing(s) | 0.27 | 0.66 | 0.69 | 3.56 |
| device timing(ms) | 14 | 27 | 24 |  73 |


| haccmk | SYCL | DPCT usm | DPCT header | OpenMP | 
| --- | --- | --- | --- | --- |
| total enqueue | 4 | 11 | 11 | 21 |
| host timing(s) | 0.21 | 0.72 | 0.6 | 3.49 |
| device timing(ms) | 6.8 | 6.7 | 6.8 | 7.6 |


| heat | SYCL | DPCT usm | DPCT header | OpenMP | 
| --- | --- | --- | --- | --- |
| total enqueue | 1003 | 1003 | 1003 | 10029 |
| host timing(s) | 8.54 | 8.44 | 8.94 | 12.36 |
| device timing(s) | 7.98 | 7.6 | 7.92 | 8.36 |


| heat2d | SYCL | DPCT usm | DPCT header | OpenMP | 
| --- | --- | --- | --- | --- |
| total enqueue | 102 | 102 | 102 | 1107 |
| host timing(s) | 1.99 | 2.33 | 2.3 | 5.5 |
| device timing(s) | 1.65 | 1.68 | 1.63 | 1.89 |


| inversek2j | SYCL | DPCT usm | DPCT header | OpenMP | 
| --- | --- | --- | --- | --- |
| total enqueue | 100001 | 100004 | 100004 | 400008 |
| host timing(s) | 5 | 3.75 | 5.5 | 16.1 |
| device timing(s) | 1.93 | 2.65 | 1.99 | 3.85 |


| iso2dfd | SYCL | DPCT usm | DPCT header | OpenMP | 
| --- | --- | --- | --- | --- |
| total enqueue | 1001 | 1004 | 1004 | 10010 |
| host timing(s) | 2.18 | 2.5 | 2.6 |  5.87 |
| device timing(s) | 1.91 | 1.94 | 1.92 | 2.1 |

| mandelbrot | SYCL | DPCT usm | DPCT header | OpenMP | 
| --- | --- | --- | --- | --- |
| total enqueue | 303 | 404 | 404 | 611 |
| host timing(s) | 0.22 | 0.69 | 0.62 |  3.49 |
| device timing(ms) | 3.29 | 3.65 | 3.58 | 4.25 |


| matrix-mul | SYCL | DPCT usm | DPCT header | OpenMP | 
| --- | --- | --- | --- | --- |
| total enqueue | 2 | 4 | 4 | 13 | 
| host timing(s) | 7 | 9.3 | 9.2 | 13.1 |
| device timing(s) | 6.7 | 8.68 | 8.44 | 9.56 |


| md | SYCL | DPCT usm | DPCT header | OpenMP | 
| --- | --- | --- | --- | --- |
| total enqueue | 1002 | 1002 | 1002 | 8008 |
| host timing(s) | 3.67 | 3.45 | 3.73 | 7.14 |
| device timing(s) | 3.3 | 2.77 | 3.0 | 3.36 |


| md5hash | SYCL | DPCT usm | DPCT header | OpenMP | 
| --- | --- | --- | --- | --- |
| total enqueue | 16 | 28 | 28 | 57 |
| host timing(s) | 3.75 | 3.37 | 3.59 | 6.22 |
| device timing(s) | 2.6 | 2.6 | 2.8 | 2.6 |


| mkl-sgemm | SYCL | DPCT usm | DPCT header | OpenMP | 
| --- | --- | --- | --- | --- |
| total enqueue | 80001 | 120004 | 80004 | 80004 |
| host timing(s) | 6.3 | 8.3 | 6.6 | 4.7 |
| device timing(s) | 2.38 | 2.52 | 2.38 | 2.53 |


| page-rank | SYCL | DPCT usm | DPCT header | OpenMP | 
| --- | --- | --- | --- | --- |
| total enqueue | 6 | 11 | 11 | 30 |
| host timing(s) | 0.75 | 1.25 | 1.19 | 3.99 |
| device timing(s) | 0.23 | 0.31 | 0.31 | 0.36 |


| particle-diffusion | SYCL | DPCT usm | DPCT header | OpenMP | 
| --- | --- | --- | --- | --- |
| total enqueue | 2 | 7 | 7 | 18 | 
| host timing(s) | 1.3 | 1.7 | 1.8 | 4.98 |
| device timing(s) | 0.22 | 0.51 | 0.53 | 1.42 |


| reduction | SYCL | DPCT usm | DPCT header | OpenMP | 
| --- | --- | --- | --- | --- |
| total enqueue | 325 | 326 | 326 | 975 |
| host timing(s) | 1.3 | 1.74 | 1.8 | 4.65 |
| device timing(s) | 1 | 0.95 | 1.0 | 1.13 |


| sph | SYCL | DPCT usm | DPCT header | OpenMP | 
| --- | --- | --- | --- | --- |
| total enqueue | 2002 | 2004 | 2004 |  13512 |
| host timing(s) | 14.6 | 15.2 | 15.4 | 12.2 |
| device timing(s) | 14 | 14.1 | 14.2 | 10.9 |


| stencil | SYCL | DPCT usm | DPCT header | OpenMP |
| --- | --- | --- | --- | --- |
| total enqueue | 2 | 3 | 3 | 10 | 
| host timing(s) | 0.73 | 1.13 | 1.12 | 4.1 |
| device timing(s) | 0.12 | 0.19 | 0.18 | 0.51 |

| xsbench | SYCL | DPCT usm | DPCT header | OpenMP |
| --- | --- | --- | --- | --- |
| total enqueue | 2 | 9 | 9 | 26 |
| host timing(s) | 3.0 | 3.4 | 3.0 | 6.8 |
| device timing(s) | 2.6 | 2.7 | 2.1 | 3.1 |

## Results on Platform 2
| amgmk | SYCL | DPCT usm | DPCT header | OpenMP |     
| --- | --- | --- | --- | --- |           
| total enqueue | 501 | 506 | 506 | 2010 | 
| host timing(s) | 0.59 | 1.04 | 0.95 | 3.87 | 
| device timing(s) | 0.28 | 0.29 | 0.28 | 0.28 |  


| aobench | SYCL | DPCT usm | DPCT header | OpenMP | 
| --- | --- | --- | --- | ---- |
| total enqueue | 20 | 30 | 30 | 85 |
| host timing(s) | 0.7 | 1.04 | 1.02 | 3.92 | 
| device timing(s) | 0.27 | 0.27 | 0.27 | 0.31 |  


| bezier-surface | SYCL | DPCT usm | DPCT header | OpenMP | 
| --- | --- | --- | --- | --- |
| total enqueue | 2 | 3 | 3 | 12 | 
| host timing(s) | 1.94 | 2.1 | 2.2 | 5.37 | 
| device timing(s) | 1.19 | 1.17 | 1.18 | 0.81 |  


| bitonic-sort | SYCL | DPCT usm | DPCT header | OpenMP | 
| --- | --- | --- | --- | --- |
| total enqueue | 326 | 327 | 327 | 1957 |
| host timing(s) | 3.01 | 3.22 | 3.21 | 6.26 | 
| device timing(s) | 2.59 | 2.52 | 2.52 | 2.77 |  


| black-scholes | SYCL | DPCT usm | DPCT header | OpenMP | 
| --- | --- | --- | --- | --- |
| total enqueue | 2 | 3 | 9 |
| host timing(s) | 0.71 | 1.42 | 1.43 | 4.49 | 
| device timing(s) | 0.27 | 0.42 | 0.37 | 0.96 |  


| chi2 | SYCL | DPCT usm | DPCT header | OpenMP | 
| --- | --- | --- | --- | --- |
| total enqueue | 2 | 3 | 3 | 12 |
| host timing(s) | 0.96 | 1.25 | 1.3 | 4.51 |
| device timing(s) | 0.19 | 0.31 | 0.28 | 1.03 |


| clenergy | SYCL | DPCT usm | DPCT header | OpenMP | 
| --- | --- | --- | --- | --- |
| total enqueue | 502 | 502  | 502 | 3011 |
| host timing(s) | 16.7 | 16.4 | 16.8 | 20.1 |
| device timing(s) | 15.99 | 15.7 | 15.93 | 15.99  |


| gamma-correction | SYCL | DPCT usm | DPCT header | OpenMP | 
| --- | --- | --- | --- | --- |
| total enqueue | 2 | 3 | 3 | 9 |
| host timing(s) | 0.33 | 0.65 | 0.70 | 3.41 |
| device timing(ms) | 16 | 26 | 23 | 68 |


| haccmk | SYCL | DPCT usm | DPCT header | OpenMP | 
| --- | --- | --- | --- | --- |
| total enqueue | 4 | 11 | 11 | 21 | 
| host timing(s) | 0.31 | 0.63 | 0.63 | 3.42 |
| device timing(ms) | 5.8 | 5.8 | 5.8 |  6.8 |


| heat | SYCL | DPCT usm | DPCT header | OpenMP | 
| --- | --- | --- | --- | --- |
| total enqueue | 1003 | 1003 | 1003 | 10029 |
| host timing(s) | 9.79 | 9.75 | 10.0 | 16.1 |
| device timing(s) | 9.19 | 8.74 | 9.11 | 12.3 |


| heat2d | SYCL | DPCT usm | DPCT header | OpenMP | 
| --- | --- | --- | --- | --- |
| total enqueue | 102 | 102 | 102 | 1107 |
| host timing(s) | 2.65 | 2.98 | 2.99 | 5.26 |
| device timing(s) | 2.29 | 2.29 | 2.29 | 1.82 |


| inversek2j | SYCL | DPCT usm | DPCT header | OpenMP | 
| --- | --- | --- | --- | --- |
| total enqueue | 100001 | 100004 | 100003 | 400008 |
| host timing(s) | 6.45 | 22 | 6.9 | 50.5 |
| device timing(s) | 3.91 | 4.37 | 3.94 | 6.1  |


| iso2dfd | SYCL | DPCT usm | DPCT header | OpenMP | 
| --- | --- | --- | --- | --- |
| total enqueue | 1001 | 1004 | 1004 |  10010 |
| host timing(s) | 2.77 | 3.29 | 3.15 | 6.42 |
| device timing(s) | 2.42 | 2.45 | 2.45 | 2.75 |


| mandelbrot | SYCL | DPCT usm | DPCT header | OpenMP | 
| --- | --- | --- | --- | --- |
| total enqueue | 303 | 404 | 404 | 611 |
| host timing(s) | 0.34 | 0.72 | 0.74 | 3.48 |
| device timing(ms) | 5.18 | 5.48 | 5.4 | 5.15 |


| matrix-mul | SYCL | DPCT usm | DPCT header | OpenMP | 
| --- | --- | --- | --- | --- |
| total enqueue | 2 | 4 | 4 | 13 |
| host timing(s) | 6.8 | 7.76 | 7.79 | 13.37 |
| device timing(s) | 6.44 | 7.09 | 7.1 | 9.95 |


| md | SYCL | DPCT usm | DPCT header | OpenMP | 
| --- | --- | --- | --- | --- |
| total enqueue | 1002 | 1002 | 1002 | 8008 |
| host timing(s) | 7.52 | 3.2 | 3.1 | 10.85 |
| device timing(s) | 7.17 | 2.42 | 2.41 | 7.17 |


| md5hash | SYCL | DPCT usm | DPCT header | OpenMP | 
| --- | --- | --- | --- | --- |
| total enqueue | 16 | 28 | 28 | 57 | 
| host timing(s) | 6.56 | 6.13 | 6.12 | 8.65 |
| device timing(s) | 5.39 | 5.39 | 5.39 | 5.12 |


| mkl-sgemm | SYCL | DPCT usm | DPCT header | OpenMP | 
| --- | --- | --- | --- | --- |
| total enqueue | 20004 | 60007 | 20007 | 20007 |
| host timing(s) | 1.63 | 14.6 | 19.1 | 6.7 |
| device timing(s) | 0.45 | 4.0 | 0.45 | 0.64 |


| page-rank | SYCL | DPCT usm | DPCT header | OpenMP | 
| --- | --- | --- | --- | --- |
| total enqueue | 6 | 11 | 11 | 30 |
| host timing(s) | 1.22 | 1.67 | 1.60 | 4.3 |
| device timing(s) | 0.71 | 0.77 | 0.74 | 0.82 |


| particle-diffusion | SYCL | DPCT usm | DPCT header | OpenMP | 
| --- | --- | --- | --- | --- |
| total enqueue | 2 | 7 | 7 | 18 | 
| host timing(s) | 1.14 | 1.49 | 1.52 | 4.83 |
| device timing(s) | 0.24 | 0.48 | 0.41 | 1.32 |


| reduction | SYCL | DPCT usm | DPCT header | OpenMP | 
| --- | --- | --- | --- | --- |
| total enqueue | 325 | 326 | 326 | 975 |
| host timing(s) | 1.61 | 1.93 | 1.94 | 4.8 |
| device timing(s) | 1.2 | 1.17 | 1.18 | 1.3 |


| sph | SYCL | DPCT usm | DPCT header | OpenMP | 
| --- | --- | --- | --- | --- |
| total enqueue | 2002 | 2004 | 2004 | 13512 |
| host timing(s) | 21.8 | 22.3 | 22.2 | 24.8 |
| device timing(s) | 21.1 | 21 | 21.1 | 20.4 |


| stencil | SYCL | DPCT usm | DPCT header | OpenMP |
| --- | --- | --- | --- | --- |
| total enqueue | 2 | 3 | 3 | 10 |
| host timing(s) | 0.68 | 0.89 | 0.98 | 4 |
| device timing(s) | 0.09 | 0.14 | 0.13 | 0.48 |

| xsbench | SYCL | DPCT usm | DPCT header | OpenMP |
| --- | --- | --- | --- | --- |
| total enqueue | 2 | 9 | 9 | 26 |
| host timing(s) | 2.49 | 2.7 | 2.8 | 5.8 |
| device timing(s) | 2.1 | 2.0 | 2.0 | 2.3 |

# Reference
### amgmk 
  The relax kernel in the AMGmk benchmark (https://asc.llnl.gov/CORAL-benchmarks/Micro/amgmk-v1.0.tar.gz)
  
### aobench
  A lightweight ambient occlusion renderer (https://code.google.com/archive/p/aobench)

### bezier-surface
  The Bezier surface (https://github.com/chai-benchmarks/chai)

### bitonic-sort
  Bitonic sorting (https://github.com/oneapi-src/oneAPI-samples/tree/master/DirectProgramming/)

### black-scholes
  The Black Scholes simulation (https://github.com/cavazos-lab/FinanceBench)

### chi2
  The Chi-square 2-df test. The input data must be downloaded before running the test. Please see README for the link.

### clenergy
  Direct Coulomb summation kernel (http://www.ks.uiuc.edu/Training/Workshop/GPU_Aug2010/resources/clenergy.tar.gz)

### gamma-correction
  Gamma correction (https://github.com/intel/BaseKit-code-samples)

### haccmk
  The HACC microkernel (https://asc.llnl.gov/CORAL-benchmarks/#haccmk)

## heat
  The heat equation solver (https://github.com/UoB-HPC/heat_sycl)

## heat2d
  Discreet 2D laplacian operation a number of times on a given vector (https://github.com/gpucw/cuda-lapl)

### inversek2j
  The inverse kinematics for 2-joint arm (http://axbench.org/)

### iso2dfd, mandelbrot, particle-diffusion
  The HPCKit code samples (https://github.com/intel/HPCKit-code-samples/)

### matrix-mul
  Single-precision floating-point matrix multiply

### md
  Molecular dynamics function in the SHOC benchmark suite(https://github.com/vetter/shoc/)

### md5hash
  The MD5 Hash function (https://github.com/vetter/shoc/)

### mkl-sgemm
  Single-precision floating-point matrix multiply using Intel<sup>®</sup> Math Kernel Library 

### page-rank
  PageRank (https://github.com/Sable/Ostrich/tree/master/map-reduce/page-rank)

### reduction
  Integer sum reduction (https://github.com/ROCm-Developer-Tools/HIP-Examples/tree/master/reduction)

### xsbench
  The nuclear reactor simulation proxy applications in SYCL  
  (https://github.com/ANL-CESAR/XSBench/tree/master/sycl)

### sph
  The simple n^2 SPH simulation (https://github.com/olcf/SPH_Simple)

### stencil
  1D stencil using shared memory


## Development Team
Authored and maintained by Zheming Jin (https://github.com/zjin-lcf) 

## Acknowledgement
Results presented were obtained using the Chameleon testbed supported by the National Science Foundation and the Intel<sup>®</sup> DevCloud.
