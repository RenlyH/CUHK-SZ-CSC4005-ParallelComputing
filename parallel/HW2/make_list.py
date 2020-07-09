import os

print(os.getcwd())

x_list = [200, 400, 1600, 3200, 4800, 6400, 8000, 12800, 14800, 17920, 20000, 24000]
y_list = x_list
m_list = [2, 4, 8, 16, 32, 64]
WallTime = 5

for i in x_list:
    for j in m_list:
        # print(j / i)
        f = open(
            "./list/mpi_" + str(
                i) + "_" + str(i) + "_m_" + str(j) + ".pbs", 'w')
        f.write(
            "#!/bin/bash\n#PBS -l nodes=1:ppn=5,mem=1g,walltime=00:02:00\n#PBS -q batch\n#PBS -m abe\n#PBS -j oe\n#PBS "
            "-V\n\nmpiexec -n " + str(
                j) + " -f /home/mpi_config /code/116010073/HW2/mpi.out -x " + str(i) + " -y " + str(i))

for i in x_list:
    for j in m_list:
        # print(j / i)
        f = open(
            "./list/pth_" + str(
                i) + "_" + str(i) + "_m_" + str(j) + ".pbs", 'w')
        f.write(
            "#!/bin/bash\n#PBS -l nodes=1:ppn=5,mem=1g,walltime=00:02:00\n#PBS -q batch\n#PBS -m abe\n#PBS -j oe\n#PBS "
            "-V\n\n/code/116010073/HW2/pth.out -x " + str(i) + " -y " + str(
                i) + " -m " + str(j))

for j in x_list:
    f = open(
        "./list/seq_" + str(
            j) + "_" + str(j) + ".pbs", 'w')
    f.write(
        "#!/bin/bash\n#PBS -l nodes=1:ppn=5,mem=1g,walltime=00:02:00\n#PBS -q batch\n#PBS -m abe\n#PBS -j oe\n#PBS "
        "-V\n\n/code/116010073/HW2/seq.out -x " + str(j) + " -y " + str(j))
