#include <iostream>
#include <vector>
#include <queue>
#include <limits.h>
int bfs(std::vector<std::vector<int>> graph, int vertex)
{
    std::queue<int> bfsQueue;
    bfsQueue.push(vertex);
    std::vector<int> lengthToBegin(graph.size(), -1);
    int currLengthToBegin = 0;
    lengthToBegin[vertex] = 0;
    while (bfsQueue.size() != 0)
    {
        vertex = bfsQueue.front();
        bfsQueue.pop();
        currLengthToBegin = lengthToBegin[vertex];
        for (int i = 0; i < graph[vertex].size(); i++)
        {
            if (lengthToBegin[graph[vertex][i]] >= currLengthToBegin)
                return lengthToBegin[graph[vertex][i]] + currLengthToBegin + 1;
            else
                if (lengthToBegin[graph[vertex][i]] == -1)
                {
                    bfsQueue.push(graph[vertex][i]);
                    lengthToBegin[graph[vertex][i]] = currLengthToBegin + 1;
                }
        }
    }
    return INT_MAX;
}
int searchMinCycle(std::vector<std::vector<int>> graph, int vertices)
{
    int minLength = INT_MAX;
    int length = 0;
    for (int i = 0; i < vertices; i++)
    {
        length = bfs(graph, i);
        if (length < minLength)
            minLength = length;
    }
    if (minLength != INT_MAX)
        return minLength;
    else
        return -1;
}
int main()
{
    int vertices = 0;
    std::cin >> vertices;
    int edges = 0;
    std::cin >> edges;
    std::vector<std::vector<int>> graph(vertices);
    int firstVertex = 0;
    int secondVertex = 0;
    for (int i = 0; i < edges; i++)
    {
        std::cin >> firstVertex;
        std::cin >> secondVertex;
        graph[firstVertex].push_back(secondVertex);
        graph[secondVertex].push_back(firstVertex);
    }
    std::cout << searchMinCycle(graph, vertices);
}


