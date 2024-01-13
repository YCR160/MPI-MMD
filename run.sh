mpicc -o multi_chip multi_chip.cpp
scp multi_chip root@192.168.114.2:~/mpi_code
scp multi_chip root@192.168.114.3:~/mpi_code
scp multi_chip root@192.168.114.4:~/mpi_code
mpirun -np 4 ./multi_chip
mpirun -np 4 -hostfile my_hostfile ./multi_chip