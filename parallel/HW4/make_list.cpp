#include <iostream>
#include <cstdio>
#include<string>
#include <fstream>

using namespace std;

int main(int argc, char **argv) {
    int N_list[6] = {50, 100, 200, 400, 600, 1000};
    int m_list[14] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 14, 16};
    int walltime = 5;
    int local = 1;
    string um = "#PBS -N CSC4005\n#PBS -M xinhaih@umich.edu\n#PBS -m abe\n#PBS -l nodes=1:ppn=5,mem=1g,walltime=00:0" +
                to_string(walltime) +
                ":00\n#PBS -A bleu_fluxod\n#PBS -q fluxod\n#PBS -m abe\n#PBS -j oe\n#PBS -V\n#PBS -l qos=flux\n\n";


    string con = "#!/bin/bash\n#PBS -l nodes=1:ppn=5,mem=4g,walltime=00:0" + to_string(walltime) +
                 ":00\n#PBS -q batch\n#PBS -m abe\n#PBS -j oe\n#PBS -V\n\n";
    string local_name = "/Users/renly/OneDrive - CUHK-Shenzhen/CUHKSZ/2019-2020 T1/CSC4005/parallel/HW4";

    // ompi
    for (int i : N_list) {
        for (int j:m_list) {
            ofstream fs;
            string name = "/ompi_list/ompi_" + to_string(i) + "_n_" + to_string(j) + ".pbs";
            if (local == 1)
                name = local_name + name;
            else name = "." + name;

            fs.open(name, ios::out);
            if (!fs) {
                cout << "file create error";
                return 0;
            }
            string cc = "mpirun -np 4  -f /home/mpi_config2 /code/116010073/HW4/ompi_p -l " + to_string(i) + " -n " +
                        to_string(j) + " -w";
            fs << con + cc;
            fs.close();

        }
    }

    //mpi
    for (int i : N_list) {
        for (int j:m_list) {
            ofstream fs;
            string name = "/mpi_list/mpi_" + to_string(i) + "_n_" + to_string(j) + ".pbs";
            if (local == 1)
                name = local_name + name;
            else name = "." + name;

            fs.open(name, ios::out);
            if (!fs) {
                cout << "file create error";
                return 0;
            }

            string cc =
                    "mpirun -np " + to_string(j) + " -f /home/mpi_config /code/116010073/HW4/mpi_p -l " + to_string(i) +
                    " -w";
            fs << con + cc;
            fs.close();

        }
    }


    //pth
    for (int i : N_list) {
        for (int j:m_list) {
            ofstream fs;
            string name = "/pth_list/pth_" + to_string(i) + "_n_" + to_string(j) + ".pbs";
            if (local == 1)
                name = local_name + name;
            else name = "." + name;
            fs.open(name, ios::out);
            if (!fs) {
                cout << "file create error";
                return 0;
            }

            string cc = "/code/116010073/HW4/pth_p -l " + to_string(i) + " -N " + to_string(j) + " -w";
            fs << con + cc;
            fs.close();

        }
    }

    //omp
    for (int i : N_list) {
        for (int j:m_list) {
            ofstream fs;
            string name = "/omp_list/omp_" + to_string(i) + "_n_" + to_string(j) + ".pbs";
            if (local == 1)
                name = local_name + name;
            else name = "." + name;
            fs.open(name, ios::out);
            if (!fs) {
                cout << "file create error";
                return 0;
            }

            string cc = "/code/116010073/HW4/pth_p -l " + to_string(i) + " -n " + to_string(j) + " -w";
            fs << con + cc;
            fs.close();

        }
    }

    //seq
    for (int i : N_list) {

        ofstream fs;
        string name = "/seq_list/seq_" + to_string(i) + "_n_1.pbs";
        if (local == 1)
            name = local_name + name;
        else name = "." + name;
        fs.open(name, ios::out);
        if (!fs) {
            cout << "file create error";
            return 0;
        }

        string cc = "/code/116010073/HW4/seq_p -l " + to_string(i) + " -w";
        fs << con + cc;
        fs.close();


    }


    return 0;


}
