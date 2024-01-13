// mpicc -o multi_chip multi_chip.cpp
// mpirun -np 4 -hostfile hostfile ./multi_chip
#include <mpi.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
# include <time.h>

#define NUM_NODES 10000

void parallel_bfs(int start, bool** graph) {
    bool visited[NUM_NODES];
    for (int i = 0; i < NUM_NODES; ++i) {
        visited[i] = false;
    }

    visited[start] = true;

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    clock_t start_t = clock();

    for (int i = world_rank; i < NUM_NODES; i += world_size) {
        if (graph[start][i] && !visited[i]) {
            visited[i] = true;
        }
    }

    bool global_visited[NUM_NODES];
    MPI_Allreduce(visited, global_visited, NUM_NODES, MPI_C_BOOL, MPI_LOR, MPI_COMM_WORLD);

    for (int i = 0; i < NUM_NODES; ++i) {
        visited[i] = global_visited[i];
    }

    clock_t end_t = clock();
    if (world_rank == 0) {
        printf("Time: %fs\n", (double)(end_t - start_t) / CLOCKS_PER_SEC);
    }

    for (int i = 0; i < NUM_NODES; ++i) {
        if (!visited[i]) {
            printf("Node %d not visited\n", i);
        }
    }
}

int main(int argc, char** argv) {
    MPI_Init(NULL, NULL);

    printf("NUM_NODES: %d\n", NUM_NODES);
    bool **graph = (bool **)malloc(NUM_NODES * sizeof(bool *));
    for (int i = 0; i < NUM_NODES; i++) {
        graph[i] = (bool *)malloc(NUM_NODES * sizeof(bool));
    }
    for (int i = 0; i < NUM_NODES; ++i) {
        for (int j = 0; j < NUM_NODES; ++j) {
            graph[i][j] = 1;
        }
    }

    parallel_bfs(0, graph);

    for (int i = 0; i < NUM_NODES; i++) {
        free(graph[i]);
    }
    free(graph);

    MPI_Finalize();
    return 0;
}