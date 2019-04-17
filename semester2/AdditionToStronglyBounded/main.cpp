/* Дополнение до сильносвязного.
 * Дан ориентированный граф.
 * Определите, какое минимальное число ребер необходимо добавить,
 * чтобы граф стал сильносвязным. В графе возможны петли.*/


#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::vector;
using std::max;

void timeDfs(const vector<vector<int>>& graph, vector<int>& visited,  vector<int>& times, int vertex){
    for (int i = 0; i < graph[vertex].size(); i++)
        if(visited[graph[vertex][i]] == -1){
            visited[graph[vertex][i]] = visited[vertex];
            timeDfs(graph, visited, times, graph[vertex][i]);
        }
    times.push_back(vertex);
}

void dfs(const vector<vector<int>>& graph, vector<int>& visited, int vertex){
    for (int i = 0; i < graph[vertex].size(); i++)
        if(visited[graph[vertex][i]] == -1){
            visited[graph[vertex][i]] = visited[vertex];
            dfs(graph, visited, graph[vertex][i]);
        }
}

int  calcComponents(const vector<vector<int>>& graph, const vector<vector<int>>& invertedGraph, vector<int>& visited){
    vector<int> times;
    visited.assign(graph.size(), -1);
    for (int i = 0; i < graph.size(); i++)
        if(visited[i] == -1) {
            visited[i] = 0;
            timeDfs(invertedGraph, visited, times, i);
        }
    visited.assign(graph.size(), -1);
    int numberOfComponents = 0;
    for (int i = times.size() - 1; i >= 0; i--)
        if(visited[times[i]] == -1){
            visited[times[i]] = numberOfComponents;
            numberOfComponents++;
            dfs(graph, visited, times[i]);
        }
    return numberOfComponents;
}

int calcMissingEdges(const vector<vector<int>>& graph, const vector<vector<int>>& invertedGraph){
    vector<int> components;
    int numberOfComponents = 0;
    numberOfComponents = calcComponents(graph, invertedGraph, components);
    int sink = 0;
    int source = 0;
    vector<int> compsWithOutEdge(numberOfComponents, 0);
    for(int i = 0; i < graph.size(); i++)
        for(int j = 0; j < graph[i].size(); j++)
            if(components[graph[i][j]] != components[i] && compsWithOutEdge[components[i]] == 0)
            {
                source++;
                compsWithOutEdge[components[i]] = 1;
            }
    source = numberOfComponents - source;
    compsWithOutEdge.assign(numberOfComponents, 0);
    for(int i = 0; i < invertedGraph.size(); i++)
        for(int j = 0; j < invertedGraph[i].size(); j++)
            if(components[invertedGraph[i][j]] != components[i] && compsWithOutEdge[components[i]] == 0)
            {
                sink++;
                compsWithOutEdge[components[i]] = 1;
            }
    sink = numberOfComponents - sink;
    if (numberOfComponents == 1)
        return (0);
    else
        return(max(source, sink));
}

int main() {
    int verticesNumber = 0;
    cin >> verticesNumber;
    int edgesNumber = 0;
    cin >> edgesNumber;
    vector<vector<int>> graph(verticesNumber);
    vector<vector<int>> invertedGraph(verticesNumber);
    for (int i = 0; i < edgesNumber; i ++)
    {
        int first = 0;
        int second = 0;
        cin >> first;
        cin >> second;
        first--;
        second--;
        graph[first].push_back(second);
        invertedGraph[second].push_back(first);
    }
    cout << calcMissingEdges(graph, invertedGraph);
}


