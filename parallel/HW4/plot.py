import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

# data = pd.read_csv("per.csv")
# data_mpi = pd.read_csv("mpi_p.csv")
# data_ompi = pd.read_csv('ompi_p.csv')

data = pd.read_csv("p_performance.csv")

data = data.groupby(['run', 'length', 'process']).mean().reset_index()
# data_mpi = data_mpi.groupby(['run', 'length', 'process']).mean().reset_index()
# data_ompi = data_ompi.groupby(['run', 'length', 'process']).mean().reset_index()

data_pth = data[data['run'] == 'pth']
data_omp = data[data['run'] == 'omp']
data_seq = data[data['run'] == 'seq']
data_mpi = data[data['run'] == 'mpi']
data_ompi = data[data['run'] == 'ompi']

for i in data_seq['length'].unique():
    if i == 1000:
        noise = 0
    else:
        noise = 0
    data_seq.loc[data_seq['length'] == i, 'time'] = np.mean(data_seq[data_seq['length'] == i]['time']) - noise

# grouped = data_mpi.groupby('length', 'process')


for i in data_mpi['length'].unique():
    data_mpi.loc[data_mpi['length'] == i, 'time'] = data_mpi.loc[data_mpi.loc[data_mpi['length'] == i].index[3]]['time']
    # print(data_mpi.loc[data_mpi.loc[data_mpi['length'] == i].index[3]])

# j = 0
# for i in data_mpi['N'].unique():
#     plt.plot('size', 'time', data=data_mpi[data_mpi['N'] == i], marker='o', markerfacecolor='C' + str(j),
#              markersize=5, color='C' + str(j), label="MPI n=" + str(i))
#     j += 1
# plt.legend()
# plt.show()
# with pd.option_context('display.max_rows', 152, 'display.max_columns', 10):  # more options can be specified also
#     print(data_mpi)
# print(data_ompi)
# print(data_mpi)


# # MPI
# j = 0
# for i in data_mpi['length'].unique():
#         plt.plot('process', 'time', data=data_mpi[data_mpi['length'] == i], marker='o', markerfacecolor='C' + str(j),
#                  markersize=5, color='C' + str(j), label="MPI length =" + str(i))
#         j += 1

# Pthread
j = 0
for i in data_pth['length'].unique():
        plt.plot('process', 'time', data=data_pth[data_pth['length'] == i], marker='o', markerfacecolor='C' + str(j),
                 markersize=5, color='C' + str(j), label="Pth length =" + str(i))
        j += 1

# Seq
j = 0
for i in data_mpi['length'].unique():
    plt.plot('process', 'time', data=data_seq[data_seq['length'] == i], marker='', markerfacecolor='C' + str(j),
             markersize=5, color='C' + str(j), label="Seq length = " + str(i), linestyle='-.')
    j += 1

# j = 0
# for i in data_ompi['length'].unique():
#     plt.plot('process', 'time', data=data_ompi[data_ompi['length'] == i], marker='o', markerfacecolor='C' + str(j),
#              markersize=5, color='C' + str(j), label="Omp+MPI N=" + str(i),linestyle='-.')
#     j += 1
#

# j = 0
# for i in data_omp['length'].unique():
#     if i != 1000:
#         plt.plot('process', 'time', data=data_omp[data_omp['length'] == i], marker='o', markerfacecolor='C' + str(j),
#                  markersize=5, color='C' + str(j), label="OpenMP length=" + str(i), linestyle=':')
#         j += 1

#
plt.xlabel('processors')
plt.ylabel('time/s')
plt.title("Pthread performance")
plt.xlim(0, 17)
plt.legend()
plt.show()
