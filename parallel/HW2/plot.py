import matplotlib.pyplot as plt
import pandas as pd

data = pd.read_csv("performance.csv")
data = data.groupby(['run', 'x', 'y', 'core']).mean().reset_index()
data_mpi = data[data['run'] == 'mpi']
data_pth = data[data['run'] == 'pth']
data_seq = data[data['run'] == 'seq']
# with pd.option_context('display.max_rows', 152, 'display.max_columns', 10):  # more options can be specified also
#     print(data)
# print(data_mpi[data_mpi['core']==2])

#
# j = 0
# for i in data_mpi['core'].unique():
#     plt.plot('x', 'time/s', data=data_mpi[data_mpi['core'] == i], marker='o', markerfacecolor='C' + str(j),
#              markersize=5, color='C' + str(j), label="MPI n=" + str(i))
#     j += 1
# plt.legend()
# plt.show()

j = 0
for i in data_pth['core'].unique():
    plt.plot('x', 'time/s', data=data_pth[data_pth['core'] == i], marker='o', markerfacecolor='C' + str(j),
             markersize=5, color='C' + str(j), label="Pthread n=" + str(i), linestyle='dashed')
    j += 1
# #
# plt.plot('x', 'time/s', data=data_seq[data_seq['core'] == 1], marker='o', markerfacecolor="black",
#          markersize=5, color='black', label="Sequential ", linestyle='-.')
plt.xlabel('x')
plt.ylabel('time/s')
plt.title("Pthread performance")
plt.legend()
plt.show()
