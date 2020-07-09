#include <iostream>
#include <ctime>

using namespace std;

void printArray(int a[], int len) {
    for (int i = 0; i < len; i++) {
        cout << a[i] << "\t";
    }
    cout << endl;
}

bool sorted_check(int a[], int len) {
    for (int i = 0; i < len - 1; i++) {
        if (a[i] > a[i + 1]) {
            return false;
        }
    }
    return true;
}

void odd_even_sort(int len, int a[]) {
    int i = 0;
    bool sorted = false;
    while (!sorted) {
        if (i % 2 == 0) {
            for (int j = 1; j <= len - 2; j += 2) {
                if (a[j] > a[j + 1]) swap(a[j], a[j + 1]);
            }
        } else {
            for (int j = 0; j <= len - 2; j += 2) {
                if (a[j] > a[j + 1]) swap(a[j], a[j + 1]);
            }
        }
        i++;
        if (sorted_check(a, len)) sorted = true;
    }

}

int main(int argc, char *argv[]) {
    clock_t start, end;
    start = clock();
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
    int *v;
    v = new int[m];
    srand((unsigned) time(NULL));
    for (int i = 0; i < m; i++) {
        v[i] = int(rand() % (MAX_NUM) + 1);
    }
    printf("Name: Xinhai Hou\nStudent ID:116010073\nAssignment 1: Odd-Even Transposition Sort, Sequential.\n\n");
//    printf("Random array is: \n");
    odd_even_sort(m, v);
    end = clock();
    double endtime = (double) (end - start) / CLOCKS_PER_SEC;
    cout << "Total time: " << endtime << "s" << endl << endl;
    printf("Sorted array is: \n");
    printArray(v, m);
    return 0;


}
//
// Created by Renly on 3/10/2019.
//

