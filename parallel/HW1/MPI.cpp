#include "mpi.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <unistd.h>


using namespace std;

void printArray(int a[], int len) {
    for (int i = 0; i < len; i++) {
        cout << a[i] << "\t";
    }
    cout << endl;
}

int main(int argc, char *argv[]) {
    /* start MPI */
    MPI_Init(&argc, &argv);
    /* timing */
    clock_t start, end;
    start = clock();
    int rank,   /* rank ID  */
            size;/* number of ranks */
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//    printf("hello this is rank: %d. %d",rank,size);
    /* Default setting of m, MAX_NUM */
    int seed = 219;
    int m = 20;
    int MAX_NUM = 1000;
    for (int i = 0; i < argc; i++) {
        if (string(argv[i]) == "-m") {
            m = atoi(argv[i + 1]);
        }
        if (string(argv[i]) == "-MAX_NUM") {
            MAX_NUM = atoi(argv[i + 1]);
        }
    }
    int n = size;

    int length = m / n + 1;
    int s_length = length - 1;
    int s_index = m % n;  /* s_index is the index of the first short sub-array;
                         * n - s_index is the number of short sub-array;
                         * s_index is the number of long sub-array */

    int v[m];
    int l_sub_v[length];
    int s_sub_v[s_length];

    /* generate a array of random number for each nodes. */
    srand((unsigned) time(NULL));
    for (int i = 0; i < m; i++) {
        v[i] = int(rand() % (MAX_NUM) + 1);
    }

    /* Distribute numbers to sub-array */
    if (rank < s_index) {
        for (int i = rank * length; i < (rank + 1) * length; i++) {
            l_sub_v[i - rank * length] = v[i];
        }


    } else {
        int l_total = s_index * length;
        for (int i = l_total + (rank - s_index) * s_length; i < l_total + (rank - s_index + 1) * s_length; i++) {
            s_sub_v[i - l_total - (rank - s_index) * s_length] = v[i];
        }
    }
    /* start for loop */
    int round = 0;
    int sorted = false;
    int boundary_sort;
    while (!sorted) {
        sorted = true;
        boundary_sort = true;
        // swap in each node
        if (round % 2 == 0) {
            if (rank < s_index) {
                for (int j = 1; j <= length - 2; j += 2) {
                    if (l_sub_v[j] > l_sub_v[j + 1]) {
                        swap(l_sub_v[j], l_sub_v[j + 1]);
                        sorted = false;
                    }
                }
            } else {
                for (int j = 1; j <= s_length - 2; j += 2) {
                    if (s_sub_v[j] > s_sub_v[j + 1]) {
                        swap(s_sub_v[j], s_sub_v[j + 1]);
                        sorted = false;
                    }
                }
            }

        } else {
            if (rank < s_index) {
                for (int j = 0; j <= length - 2; j += 2) {
                    if (l_sub_v[j] > l_sub_v[j + 1]) {
                        swap(l_sub_v[j], l_sub_v[j + 1]);
                        sorted = false;
                    }
                }
            } else {
                for (int j = 0; j <= s_length - 2; j += 2) {
                    if (s_sub_v[j] > s_sub_v[j + 1]) {
                        swap(s_sub_v[j], s_sub_v[j + 1]);
                        sorted = false;
                    }
                }
            }
        }

        /* swap at the boundary */
        /* Send the last number of each node. For the last node, it receives -1 */
        int r_boundary = -1, l_boundary = -1;
        if (rank != 0) {
            if (rank < s_index) {
                int rc = MPI_Send(&l_sub_v[0], 1, MPI_INT, rank - 1, round, MPI_COMM_WORLD);
                if (rc != MPI_SUCCESS) {
                    printf("Send failure for %d to %d at round %d\n", rank, rank - 1, round);
                }
            } else {
                int rc = MPI_Send(&s_sub_v[0], 1, MPI_INT, rank - 1, round, MPI_COMM_WORLD);
                if (rc != MPI_SUCCESS) {
                    printf("Send failure for %d to %d at round %d\n", rank, rank - 1, round);
                }
            }
        }

        MPI_Status status;
        /* For all nodes except for the last one, they should conduct boundary check */
        if (rank != size - 1) {
            int rc = MPI_Recv(&r_boundary, 1, MPI_INT, rank + 1, round, MPI_COMM_WORLD, &status);
            if (rc != MPI_SUCCESS) {
                printf("Receive failure for %d\n", rank);
            }

            if (rank < s_index) {
                if (l_sub_v[length - 1] > r_boundary) {
                    swap(l_sub_v[length - 1], r_boundary);
                    boundary_sort = false;
                }

            } else if (rank >= s_index) {
                if (s_sub_v[s_length - 1] > r_boundary) {
                    swap(s_sub_v[s_length - 1], r_boundary);
                    boundary_sort = false;
                }
            }
            /* We need to transport last number of this node to the next node */
            int bd = MPI_Send(&r_boundary, 1, MPI_INT, rank + 1, round, MPI_COMM_WORLD);
            if (bd != MPI_SUCCESS) {
                printf("Send failure for %d to %d at round %d\n", rank, rank + 1, round);
            }
        }
        MPI_Status status2;
        if (rank != 0) {
            int bd = MPI_Recv(&l_boundary, 1, MPI_INT, rank - 1, round, MPI_COMM_WORLD, &status2);
            if (bd != MPI_SUCCESS) {
                printf("Receive failure for %d from %d at round %d\n", rank, rank - 1, round);
            }
            /* Swap boundary back to the array */
            if (rank < s_index) swap(l_sub_v[0], l_boundary);
            else swap(s_sub_v[0], l_boundary);

            sorted = sorted && boundary_sort;
        }
        /* gather all sorted flag into node 0, if there is no swap operation in this node, sorted is true for this node */
        int all_sort[n];
        MPI_Gather(&sorted, 1, MPI_INT, &all_sort, 1, MPI_INT, 0, MPI_COMM_WORLD);
        int stop_sort = true;
        for (int i = 0; i < size; i++) if (!all_sort[i]) stop_sort = false;
        for (int i = 0; i < size; i++) all_sort[i] = stop_sort;
        MPI_Scatter(&all_sort, 1, MPI_INT, &sorted, 1, MPI_INT, 0, MPI_COMM_WORLD);
        /* If all numbers are ordered in each node, stop the loop */
        round++;
    }


    if (rank >= s_index) {
        for (int i = 0; i < length - 1; i++) l_sub_v[i] = s_sub_v[i];
        l_sub_v[length - 1] = -1;
    }
    /* Gather */
    int sortedArray[n * length];
    int receive_counts[n];
    int receive_displacements[n];
    for (int i = 0; i < s_index; i++) {
        receive_counts[i] = length;
        receive_displacements[i] = i * length;
    }
    for (int i = s_index; i < n; i++) {
        receive_counts[i] = length;
        receive_displacements[i] = (s_index) * length + (i - s_index) * s_length;
    }

    int ac = MPI_Gatherv(&l_sub_v, length, MPI_INT, &sortedArray, receive_counts, receive_displacements, MPI_INT, 0,
                         MPI_COMM_WORLD);


    if (rank == 0) {
        printf("Name: Xinhai Hou\nStudent ID:116010073\nAssignment 1: Odd-Even Transposition Sort, MPI.\n\n");
    }
    MPI_Finalize();
    end = clock();
    double endtime = (double) (end - start) / CLOCKS_PER_SEC;
    cout << "Total time: " << endtime << "s for node: " << rank << endl;
    sleep(1);
    if (rank == 0) {
        printf("\n");
        printf("Random array is:\n");
        printArray(v, m);
        printf("Sorted array is:\n");
        printArray(sortedArray, m);
    }
    return 0;
}

//#include "mpi.h"
//#include <stdio.h>
//#include <iostream>
//#include <unistd.h>
//
//using namespace std;
//
//void printArray(bool a[], int len) {
//    for (int i = 0; i < len; i++) {
//        cout << a[i] << "\t";
//    }
//    cout << endl;
//}
//
//int main(int argc, char *argv[]) {
//    bool buffer=true;
//    bool result[4]={true,false,false,true};
//    bool a=false;
//    int rank, size;
//
//    MPI_Init(&argc, &argv);
//    MPI_Comm_size(MPI_COMM_WORLD, &size);
//    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//    printf("hello %d",rank);
//
////    buffer = rank != 2;
//
////    for (int i = 0; i < size; i++) {
////        if (rank == i) {
////            cout << buffer << '\t' << rank << endl;
////        } else sleep(1);
////
////    }
//
////    MPI_Scatter(&result, 1, MPI_CXX_BOOL, &buffer, 1, MPI_CXX_BOOL, 0, MPI_COMM_WORLD);
////    if (rank == 0) {
////
////        printArray(result, 4);
//////        fflush(stdout);
////    }
//    cout<<"MPI node "<< rank<<'\t'<<buffer<<endl;
//    MPI_Finalize();
//    return 0;
//}
//
// Created by Renly on 9/10/2019.
//
