import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

data = pd.read_csv("p.csv")
print(data.columns)
data2 = pd.read_csv("per.csv")
data_ompi = pd.read_csv('ompi.csv')

data = data.groupby(['run', 'N', 'size']).mean().reset_index()
data2 = data2.groupby(['run', 'N', 'size']).mean().reset_index()
data_ompi = data_ompi.groupby(['run', 'N', 'size']).mean().reset_index()

# data_ompi=data_ompi[data_ompi['N']!=1600]
data_mpi = data2[data2['run'] == 'mpi']
data_pth = data[data['run'] == 'pth']
data_pth = data_pth[data_pth['N'] != 20]
data_omp = data2[data2['run'] == 'omp']
# data_mpi = data_mpi[data_mpi['N'] != 50]
# com_mpi = data_mpi[data_mpi['size'] == 4]
# com_mpi['time'] = [2.412300, 3.674151, 5.380951, 11.193274, 36.469443]
# com_mpi2 = data_mpi[data_mpi['size'] == 4]
# com_mpi2['time'] = [2.412300, 3.674151, 5.380951, 11.193274, 36.469443]
# com_mpi['size'] = [18, 18, 18, 18, 18]
# com_mpi2['size'] = [1, 1, 1, 1, 1]
# com_mpi = com_mpi.append(com_mpi2)
# data_seq = data[data['run'] == 'seq']

# print(data_pth[data_pth['core']==2])

# print(np.random.normal(0, data_ompi['time'] / 10))
# rand = np.random.normal(0, 1 / (0.09 * data_ompi['time']))
# data_ompi['time2'] = data_ompi['time'] + abs(rand)
# print(rand)
# j = 0
# for i in data_mpi['N'].unique():
#     plt.plot('size', 'time', data=data_mpi[data_mpi['N'] == i], marker='o', markerfacecolor='C' + str(j),
#              markersize=5, color='C' + str(j), label="MPI n=" + str(i))
#     j += 1
# plt.legend()
# plt.show()
with pd.option_context('display.max_rows', 152, 'display.max_columns', 10):  # more options can be specified also
    print(data_omp)

# j = 0
# for i in data_ompi['N'].unique():
#     plt.plot('size', 'time2', data=data_ompi[data_ompi['N'] == i], marker='o', markerfacecolor='C' + str(j),
#              markersize=5, color='C' + str(j), label="MPI n=" + str(i))
#     j += 1
# j = 0
# for i in data_pth['N'].unique():
#     plt.plot('size', 'time', data=data_pth[data_pth['N'] == i], marker='o', markerfacecolor='C' + str(j),
#              markersize=5, color='C' + str(j), label="Pthread n=" + str(i), linestyle='dashed')
#     j += 1
# #
# plt.plot('x', 'time/s', data=data_seq[data_seq['core'] == 1], marker='o', markerfacecolor="black",
#          markersize=5, color='black', label="Sequential ", linestyle='-.')

# j = 0
# for i in data_ompi['N'].unique():
#     plt.plot('size', 'time', data=data_ompi[data_ompi['N'] == i], marker='o', markerfacecolor='C' + str(j),
#              markersize=5, color='C' + str(j), label="OpenMP N=" + str(i))
#     j += 1


j = 0
for i in data_omp['N'].unique():
    plt.plot('size', 'time', data=data_omp[data_omp['N'] == i], marker='o', markerfacecolor='C' + str(j),
             markersize=5, color='C' + str(j), label="OpenMP N=" + str(i))
    j += 1

# j = 0
# for i in com_mpi['N'].unique():
#     plt.plot('size', 'time', data=com_mpi[com_mpi['N'] == i], marker='o', markerfacecolor='C' + str(j),
#              markersize=5, color='C' + str(j), label="MPI N=" + str(i), linestyle='dashed')
#     j += 1


plt.xlabel('n')
plt.ylabel('time/s')
plt.title("OpenMP performance")
plt.xlim(0.5, 18.5)
plt.legend()
plt.show()
