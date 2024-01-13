#include <omp.h>
#include <queue>
#include <vector>
#include <atomic>
#include <iostream>
#include <ctime>

void parallel_bfs(int start, std::vector<std::vector<int>>& graph) {
    int num_nodes = graph.size();
    std::vector<bool> visited(num_nodes, false);
    std::queue<int> q;
    std::atomic<int> visited_count(0);

    visited[start] = true;
    q.push(start);
    ++visited_count;

    clock_t start_t = clock();

    #pragma omp parallel num_threads(8)
    {
        while (!q.empty()) {
            int node;

            #pragma omp critical
            {
                node = q.front();
                q.pop();
            }

            for (int i = 0; i < num_nodes; ++i) {
                if (graph[node][i] && !visited[i]) {
                    #pragma omp critical
                    {
                        if (!visited[i]) {
                            visited[i] = true;
                            graph[node][i] = 2;
                            q.push(i);
                            ++visited_count;
                            // std::cout << "Visited nodes: " << visited_count << std::endl;
                        }
                    }
                }
            }
        }
    }

    clock_t end_t = clock();
    std::cout << "Time: " << (double)(end_t - start_t) / CLOCKS_PER_SEC << "s" << std::endl;

    for (int i = 0; i < num_nodes; ++i) {
        if (!visited[i]) {
            std::cout << "Node " << i << " not visited" << std::endl;
        }
    }
}

int main() {
    int num_nodes = 10000;
    std::vector<std::vector<int>> graph(num_nodes, std::vector<int>(num_nodes, 1));
    std::cout << "Graph size: " << num_nodes << std::endl;

    parallel_bfs(0, graph);

    return 0;
}