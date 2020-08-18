#define DPCT_USM_LEVEL_NONE
#include <CL/sycl.hpp>
#include <dpct/dpct.hpp>
#include <cassert>
#include <cfloat>
#include <list>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include "MD.h"

using namespace std;


template <class T, class posVecType>
inline T distance(const posVecType* position, const int i, const int j);

template <class T>
inline void insertInOrder(std::list<T>& currDist, std::list<int>& currList,
        const int j, const T distIJ, const int maxNeighbors);

template <class T, class posVecType>
inline int buildNeighborList(const int nAtom, const posVecType* position,
        int* neighborList);

template <class T>
inline int populateNeighborList(std::list<T>& currDist,
        std::list<int>& currList, const int j, const int nAtom,
        int* neighborList);


// ****************************************************************************
// Function: checkResults
//
// Purpose:
//   Check device results against cpu results -- this is the CPU equivalent of
//
// Arguments:
//      d_force:   forces calculated on the device
//      position:  positions of atoms
//      neighList: atom neighbor list
//      nAtom:     number of atoms
// Returns:  true if results match, false otherwise
//
// Programmer: Kyle Spafford
// Creation: July 26, 2010
//
// Modifications:
//
// ****************************************************************************
template <class T, class forceVecType, class posVecType>
bool checkResults(forceVecType* d_force, posVecType *position,
                  int *neighList, int nAtom)
{
    for (int i = 0; i < nAtom; i++)
    {
        posVecType ipos = position[i];
        forceVecType f = {0.f, 0.f, 0.f, 0.f};
        int j = 0;
        while (j < maxNeighbors)
        {
            int jidx = neighList[j*nAtom + i];
            posVecType jpos = position[jidx];
            // Calculate distance
                              T delx = ipos.x() - jpos.x();
                              T dely = ipos.y() - jpos.y();
                              T delz = ipos.z() - jpos.z();
            T r2inv = delx*delx + dely*dely + delz*delz;

            // If distance is less than cutoff, calculate force
            if (r2inv < cutsq) {

                r2inv = 1.0f/r2inv;
                T r6inv = r2inv * r2inv * r2inv;
                T force = r2inv*r6inv*(lj1*r6inv - lj2);

                                        f.x() += delx * force;
                                        f.y() += dely * force;
                                        f.z() += delz * force;
            }
            j++;
        }
        // Check the results
                    T diffx = (d_force[i].x() - f.x()) / d_force[i].x();
                    T diffy = (d_force[i].y() - f.y()) / d_force[i].y();
                    T diffz = (d_force[i].z() - f.z()) / d_force[i].z();
                    T err = sqrt(diffx * diffx) + sqrt(diffy * diffy) +
                            sqrt(diffz * diffz);
        if (err > (3.0 * EPSILON))
        {
            cout << "Test Failed, idx: " << i << " diff: " << err << "\n";
                              cout << "f.x: " << f.x()
                                   << " df.x: " << d_force[i].x() << "\n";
                              cout << "f.y: " << f.y()
                                   << " df.y: " << d_force[i].y() << "\n";
                              cout << "f.z: " << f.z()
                                   << " df.z: " << d_force[i].z() << "\n";
            cout << "Test FAILED\n";
            return false;
        }
    }
    cout << "Test Passed\n";
    return true;
}


void md ( const POSVECTYPE* position, FORCEVECTYPE* force, const int* neighborList, 
          const int nAtom, const int maxNeighbors, 
          const FPTYPE lj1_t, const FPTYPE lj2_t, const FPTYPE cutsq_t ,
          sycl::nd_item<3> item_ct1)
{
          const uint idx =
              item_ct1.get_local_range().get(2) * item_ct1.get_group(2) +
              item_ct1.get_local_id(2);
          if (idx >= nAtom) return;

          POSVECTYPE ipos = position[idx];
          FORCEVECTYPE f = {0.0f, 0.0f, 0.0f, 0.0f};

          int j = 0;
          while (j < maxNeighbors)
          {
              int jidx = neighborList[j*nAtom + idx];

              // Uncoalesced read
              POSVECTYPE jpos = position[jidx];

              // Calculate distance
                    FPTYPE delx = ipos.x() - jpos.x();
                    FPTYPE dely = ipos.y() - jpos.y();
                    FPTYPE delz = ipos.z() - jpos.z();
              FPTYPE r2inv = delx*delx + dely*dely + delz*delz;

              // If distance is less than cutoff, calculate force
              if (r2inv < cutsq_t)
              {
                  r2inv = (FPTYPE)1.0 / r2inv;
                  FPTYPE r6inv = r2inv * r2inv * r2inv;
                  FPTYPE forceC = r2inv * r6inv * (lj1_t * r6inv - lj2_t);

                              f.x() += delx * forceC;
                              f.y() += dely * forceC;
                              f.z() += delz * forceC;
              }
              j++;
          }
          // store the results
          force[idx] = f;
}



int main(int argc, char** argv)
{
          dpct::device_ext &dev_ct1 = dpct::get_current_device();
          sycl::queue &q_ct1 = dev_ct1.default_queue();
    if (argc != 3) {
      printf("usage: %s <class size> <iteration>", argv[0]);
      return 1;
    }
    
    // Problem Parameters
    int sizeClass = atoi(argv[1]);
    int iteration = atoi(argv[2]);
    const int probSizes[] = { 12288, 24576, 36864, 73728 };
    assert(sizeClass >= 0 && sizeClass < 4);
    assert(iteration >= 0);

    int nAtom = probSizes[sizeClass];

    // Allocate problem data on host
    POSVECTYPE*   position;
    FORCEVECTYPE* h_force;
    int* neighborList;
    size_t localSize  = 256;
    size_t globalSize = nAtom;

    position = (POSVECTYPE*) malloc(nAtom * sizeof(POSVECTYPE));
    h_force = (FORCEVECTYPE*) malloc(nAtom * sizeof(FORCEVECTYPE));
    neighborList = (int*) malloc(maxNeighbors * nAtom * sizeof(int));

    cout << "Initializing test problem (this can take several "
            "minutes for large problems).\n                   ";

    // Seed random number generator
    srand48(8650341L);

    // Initialize positions -- random distribution in cubic domain
    for (int i = 0; i < nAtom; i++)
    {
                    position[i].x() = (drand48() * domainEdge);
                    position[i].y() = (drand48() * domainEdge);
                    position[i].z() = (drand48() * domainEdge);
    }


    cout << "Finished.\n";
    int totalPairs = buildNeighborList<FPTYPE, POSVECTYPE>(nAtom, position, neighborList);
    cout << totalPairs << " of " << nAtom*maxNeighbors <<
            " pairs within cutoff distance = " <<
            100.0 * ((double)totalPairs / (nAtom*maxNeighbors)) << " %\n";

    // see MD.h
    FPTYPE lj1_t   = (FPTYPE) lj1;
    FPTYPE lj2_t   = (FPTYPE) lj2;
    FPTYPE cutsq_t = (FPTYPE) cutsq;

    POSVECTYPE*   d_position;
    FORCEVECTYPE* d_force;
    int* d_neighborList;
          dpct::dpct_malloc((void **)&d_force, nAtom * sizeof(FORCEVECTYPE));
          dpct::dpct_malloc((void **)&d_position, nAtom * sizeof(POSVECTYPE));
          dpct::dpct_malloc((void **)&d_neighborList,
                            nAtom * maxNeighbors * sizeof(int));

          {
                    dpct::buffer_t d_position_buf_ct0 =
                        dpct::get_buffer(d_position);
                    dpct::buffer_t d_force_buf_ct1 = dpct::get_buffer(d_force);
                    dpct::buffer_t d_neighborList_buf_ct2 =
                        dpct::get_buffer(d_neighborList);
                    q_ct1.submit([&](sycl::handler &cgh) {
                              auto d_position_acc_ct0 =
                                  d_position_buf_ct0.get_access<
                                      sycl::access::mode::read_write>(cgh);
                              auto d_force_acc_ct1 = d_force_buf_ct1.get_access<
                                  sycl::access::mode::read_write>(cgh);
                              auto d_neighborList_acc_ct2 =
                                  d_neighborList_buf_ct2.get_access<
                                      sycl::access::mode::read_write>(cgh);

                              auto maxNeighbors_ct4 = maxNeighbors;

                              cgh.parallel_for(
                                  sycl::nd_range<3>(
                                      sycl::range<3>(
                                          1, 1,
                                          (globalSize + localSize - 1) /
                                              localSize) *
                                          sycl::range<3>(1, 1, localSize),
                                      sycl::range<3>(1, 1, localSize)),
                                  [=](sycl::nd_item<3> item_ct1) {
                                            md((const sycl::float4
                                                    *)(&d_position_acc_ct0[0]),
                                               (sycl::float4
                                                    *)(&d_force_acc_ct1[0]),
                                               (const int
                                                    *)(&d_neighborList_acc_ct2
                                                           [0]),
                                               nAtom, maxNeighbors_ct4, lj1_t,
                                               lj2_t, cutsq_t, item_ct1);
                                  });
                    });
          }

          dpct::dpct_memcpy(h_force, d_force, nAtom * sizeof(FORCEVECTYPE),
                            dpct::device_to_host);

    cout << "Performing Correctness Check (may take several minutes)\n";

    if (!checkResults<FPTYPE, FORCEVECTYPE, POSVECTYPE>(h_force, position,
            neighborList, nAtom))
    {
        return -1;
    }

    for (int j = 0; j < iteration; j++)
    {
        //Launch Kernels
                    dpct::buffer_t d_position_buf_ct0 =
                        dpct::get_buffer(d_position);
                    dpct::buffer_t d_force_buf_ct1 = dpct::get_buffer(d_force);
                    dpct::buffer_t d_neighborList_buf_ct2 =
                        dpct::get_buffer(d_neighborList);
                    q_ct1.submit([&](sycl::handler &cgh) {
                              auto d_position_acc_ct0 =
                                  d_position_buf_ct0.get_access<
                                      sycl::access::mode::read_write>(cgh);
                              auto d_force_acc_ct1 = d_force_buf_ct1.get_access<
                                  sycl::access::mode::read_write>(cgh);
                              auto d_neighborList_acc_ct2 =
                                  d_neighborList_buf_ct2.get_access<
                                      sycl::access::mode::read_write>(cgh);

                              auto maxNeighbors_ct4 = maxNeighbors;

                              cgh.parallel_for(
                                  sycl::nd_range<3>(
                                      sycl::range<3>(
                                          1, 1,
                                          (globalSize + localSize - 1) /
                                              localSize) *
                                          sycl::range<3>(1, 1, localSize),
                                      sycl::range<3>(1, 1, localSize)),
                                  [=](sycl::nd_item<3> item_ct1) {
                                            md((const sycl::float4
                                                    *)(&d_position_acc_ct0[0]),
                                               (sycl::float4
                                                    *)(&d_force_acc_ct1[0]),
                                               (const int
                                                    *)(&d_neighborList_acc_ct2
                                                           [0]),
                                               nAtom, maxNeighbors_ct4, lj1_t,
                                               lj2_t, cutsq_t, item_ct1);
                                  });
                    });
    }
          dev_ct1.queues_wait_and_throw();

          dpct::dpct_free(d_position);
          dpct::dpct_free(d_force);
          dpct::dpct_free(d_neighborList);

    free(position);
    free(h_force);
    free(neighborList);

    return 0;
}

// ********************************************************
// Function: distance
//
// Purpose:
//   Calculates distance squared between two atoms
//
// Arguments:
//   position: atom position information
//   i, j: indexes of the two atoms
//
// Returns:  the computed distance
//
// Programmer: Kyle Spafford
// Creation: July 26, 2010
//
// Modifications:
//
// ********************************************************
template <class T, class posVecType>
inline T distance(const posVecType* position, const int i, const int j)
{
    posVecType ipos = position[i];
    posVecType jpos = position[j];
          T delx = ipos.x() - jpos.x();
          T dely = ipos.y() - jpos.y();
          T delz = ipos.z() - jpos.z();
    T r2inv = delx * delx + dely * dely + delz * delz;
    return r2inv;
}

// ********************************************************
// Function: insertInOrder
//
// Purpose:
//   Adds atom j to current neighbor list and distance list
//   if it's distance is low enough.
//
// Arguments:
//   currDist: distance between current atom and each of its neighbors in the
//             current list, sorted in ascending order
//   currList: neighbor list for current atom, sorted by distance in asc. order
//   j:        atom to insert into neighbor list
//   distIJ:   distance between current atom and atom J
//   maxNeighbors: max length of neighbor list
//
// Returns:  nothing
//
// Programmer: Kyle Spafford
// Creation: July 26, 2010
//
// Modifications:
//
// ********************************************************
template <class T>
inline void insertInOrder(list<T>& currDist, list<int>& currList,
        const int j, const T distIJ, const int maxNeighbors)
{

    typename list<T>::iterator   it;
    typename list<int>::iterator it2;

    it2 = currList.begin();

    T currMax = currDist.back();

    if (distIJ > currMax) return;

    for (it=currDist.begin(); it!=currDist.end(); it++)
    {
        if (distIJ < (*it))
        {
            // Insert into appropriate place in list
            currDist.insert(it,distIJ);
            currList.insert(it2, j);

            // Trim end of list
            currList.resize(maxNeighbors);
            currDist.resize(maxNeighbors);
            return;
        }
        it2++;
    }
}
// ********************************************************
// Function: buildNeighborList
//
// Purpose:
//   Builds the neighbor list structure for all atoms for GPU coalesced reads
//   and counts the number of pairs within the cutoff distance, so
//   the benchmark gets an accurate FLOPS count
//
// Arguments:
//   nAtom:    total number of atoms
//   position: pointer to the atom's position information
//   neighborList: pointer to neighbor list data structure
//
// Returns:  number of pairs of atoms within cutoff distance
//
// Programmer: Kyle Spafford
// Creation: July 26, 2010
//
// Modifications:
//
// ********************************************************
template <class T, class posVecType>
inline int buildNeighborList(const int nAtom, const posVecType* position,
        int* neighborList)
{
    int totalPairs = 0;
    // Build Neighbor List
    // Find the nearest N atoms to each other atom, where N = maxNeighbors
    for (int i = 0; i < nAtom; i++)
    {
        // Current neighbor list for atom i, initialized to -1
        list<int>   currList(maxNeighbors, -1);
        // Distance to those neighbors.  We're populating this with the
        // closest neighbors, so initialize to FLT_MAX
        list<T> currDist(maxNeighbors, FLT_MAX);

        for (int j = 0; j < nAtom; j++)
        {
            if (i == j) continue; // An atom cannot be its own neighbor

            // Calculate distance and insert in order into the current lists
            T distIJ = distance<T, posVecType>(position, i, j);
            insertInOrder<T>(currDist, currList, j, distIJ, maxNeighbors);
        }
        // We should now have the closest maxNeighbors neighbors and their
        // distances to atom i. Populate the neighbor list data structure
        // for GPU coalesced reads.
        // The populate method returns how many of the maxNeighbors closest
        // neighbors are within the cutoff distance.  This will be used to
        // calculate GFLOPS later.
        totalPairs += populateNeighborList<T>(currDist, currList, i, nAtom,
                neighborList);
    }
    return totalPairs;
}


// ********************************************************
// Function: populateNeighborList
//
// Purpose:
//   Populates the neighbor list structure for a *single* atom for
//   GPU coalesced reads and counts the number of pairs within the cutoff
//   distance, (for current atom) so the benchmark gets an accurate FLOPS count
//
// Arguments:
//   currDist: distance between current atom and each of its maxNeighbors
//             neighbors
//   currList: current list of neighbors
//   i:        current atom
//   nAtom:    total number of atoms
//   neighborList: pointer to neighbor list data structure
//
// Returns:  number of pairs of atoms within cutoff distance
//
// Programmer: Kyle Spafford
// Creation: July 26, 2010
//
// Modifications:
//
// ********************************************************
template <class T>
inline int populateNeighborList(list<T>& currDist,
        list<int>& currList, const int i, const int nAtom,
        int* neighborList)
{
    int idx = 0;
    int validPairs = 0; // Pairs of atoms closer together than the cutoff

    // Iterate across distance and neighbor list
    typename list<T>::iterator distanceIter = currDist.begin();
    for (list<int>::iterator neighborIter = currList.begin();
            neighborIter != currList.end(); neighborIter++)
    {
        // Populate packed neighbor list
        neighborList[(idx * nAtom) + i] = *neighborIter;

        // If the distance is less than cutoff, increment valid counter
        if (*distanceIter < cutsq)
            validPairs++;

        // Increment idx and distance iterator
        idx++;
        distanceIter++;
    }
    return validPairs;
}