#include <iostream>
#include <cstdio>
#include<string>

using namespace std;

int main(int argc, char **argv) {
    int N_list[5] = {50, 200, 400, 800, 1000};
    int m_list[14] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 16, 18};
    int walltime = 5;
    string um = "#PBS -N CSC4005\n#PBS -M xinhaih@umich.edu\n#PBS -m abe\n#PBS -l nodes=1:ppn=5,mem=1g,walltime=00:0" +
                to_string(walltime) +
                ":00\n#PBS -A bleu_fluxod\n#PBS -q fluxod\n#PBS -m abe\n#PBS -j oe\n#PBS -V\n#PBS -l qos=flux\n\n";
    for (int i : N_list) {
        for (int j:m_list) {
            FILE *fp = nullptr;
            string name = "./list/ompi_" + to_string(i) + "_n_" + to_string(j) + ".pbs";

            const char *filename = name.c_str();
            fp = fopen(filename, "w+");
            string con = "#!/bin/bash\n#PBS -l nodes=1:ppn=5,mem=1g,walltime=00:0" + to_string(walltime) +
                         ":00\n#PBS -q batch\n#PBS -m abe\n#PBS -j oe\n#PBS -V\n\nmpirun -np 4 ";
            string cc = "mpirun -np 4 /home/xinhaih/HW3/ompi_p -N " + to_string(i) + " -m " + to_string(j);
            const char *content = (um + cc).c_str();
            fputs(content, fp);
        }
    }

    for (int i : N_list) {
        for (int j:m_list) {
            FILE *fp = nullptr;
            string name = "./list/mpi_" + to_string(i) + "_n_" + to_string(j) + ".pbs";
            const char *filename = name.c_str();
            fp = fopen(filename, "w+");
            string con =
                    "#!/bin/bash\n#PBS -l nodes=1:ppn=5,mem=1g,walltime=00:0" + to_string(walltime) +
                    ":00\n#PBS -q batch\n#PBS -m abe\n#PBS -j oe\n#PBS -V\n\nmpirun -np " +
                    to_string(j);

            string cc = "mpirun -np " + to_string(j) + " /home/xinhaih/HW3/mpi_p -N " + to_string(i);
            const char *content = (um + cc).c_str();
            fputs(content, fp);
        }
    }
    for (int i : N_list) {
        FILE *fp = nullptr;
        string name = "./list/seq_" + to_string(i) + "_n_" + ".pbs";
        const char *filename = name.c_str();
        fp = fopen(filename, "w+");
        string con = "#!/bin/bash\n#PBS -l nodes=1:ppn=5,mem=1g,walltime=00:0" + to_string(walltime) +
                     ":00\n#PBS -q batch\n#PBS -m abe\n#PBS -j oe\n#PBS -V\n\n";
        string cc = "/home/xinhaih/HW3/seq_p -N " + to_string(i);
        const char *content = (um + cc).c_str();
        fputs(content, fp);
    }

    for (int i: N_list) {
        for (int j:m_list) {
            FILE *fp = nullptr;
            string name = "./list/pth_" + to_string(i) + "_n_" + to_string(j) + ".pbs";
            const char *filename = name.c_str();
            fp = fopen(filename, "w+");
            string con =
                    "#!/bin/bash\n#PBS -l nodes=1:ppn=5,mem=1g,walltime=00:0" + to_string(walltime) +
                    ":00\n#PBS -q batch\n#PBS -m abe\n#PBS -j oe\n#PBS -V\n\n";
            string cc = " /home/xinhaih/HW3/pth_p -N " + to_string(i) + " n " + to_string(j);
            const char *content = (um + cc).c_str();
            fputs(content, fp);
        }
    }

    for (int i: N_list) {
        for (int j:m_list) {
            FILE *fp = nullptr;
            string name = "./list/omp_" + to_string(i) + "_n_" + to_string(j) + ".pbs";
            const char *filename = name.c_str();
            fp = fopen(filename, "w+");
            string con =
                    "#!/bin/bash\n#PBS -l nodes=1:ppn=5,mem=1g,walltime=00:0" + to_string(walltime) +
                    ":00\n#PBS -q batch\n#PBS -m abe\n#PBS -j oe\n#PBS -V\n\n";
            string cc = " /home/xinhaih/HW3/omp_p -N " + to_string(i) + " n " + to_string(j);
            const char *content = (um + cc).c_str();
            fputs(content, fp);
        }
    }


    return 0;
}
