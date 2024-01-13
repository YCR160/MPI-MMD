# MPI-MMD
MPI multi-chip multi-core demo

## Single chip single-core & multi-core

```bash
# single-core
g++ -O2 ./single_chip.cpp && ./a.out

# multi-core
g++ -O2 -fopenmp ./single_chip.cpp && ./a.out
```

## Multi chip

You must setup your ssh access and prepare host file (`my_hostfile`) before running the script.

```bash
./run.sh
```
