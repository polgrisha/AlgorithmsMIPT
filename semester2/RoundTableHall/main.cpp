/*Единственный способ попасть в Зал Круглых Столов – пройти через Колонный Коридор. 
Стены Коридора изображаются на карте прямыми линиями, которые параллельны осиOY системы координат. 
Вход в Коридор находится снизу, а выход из Коридора в Зал – сверху. 
В Коридоре есть цилиндрические (на карте круглые) Колонны одинакового радиуса R. 
Разработайте алгоритм, который по информации о размерах Коридора, и размещения Колонн определяет диаметр наибольшего из Круглых Столов, 
который можно пронести через такой Коридор, сохраняя поверхность Стола горизонтальной.*/

#include <iostream>
#include <vector>
#include <math.h>
#include <time.h>
#include <iomanip>

using std::cin;
using std::cout;
using std::vector;
using std::fixed;
using std::setprecision;

struct Point
{
    double x;
    double y;
};

void dfs (const vector<vector<double>> &graph, vector<bool> &visited, double vertex)
{
    double adjacVertex = 0;
    visited[vertex] = true;
    for (double i = 0; i < graph[vertex].size(); i++)
    {
        adjacVertex = graph[vertex][i];
        if (visited[adjacVertex] == false)
        {
            dfs (graph, visited, adjacVertex);
        }
    }
}

double calcLength (const Point &first, const Point &second)
{
    return sqrt((first.x - second.x) * (first.x - second.x) + (first.y - second.y) * (first.y - second.y));
}

bool ifTableFits (vector<Point> &coulumns, double firstWall, double secondWall, double coulumnsR, double table)
{
    double coulumnsNumber = coulumns.size();
    firstWall -= coulumnsR;
    secondWall += coulumnsR;
    table += 2 * coulumnsR;
    double i = 0;
    Point coulumn;
    coulumn.x = firstWall;
    coulumn.y = 0;
    coulumns.push_back(coulumn);
    coulumn.x = secondWall;
    coulumns.push_back(coulumn);
    vector<vector<double>> graph(coulumns.size());
    int j = 0;

    for (i = 0; i < coulumns.size() - 2; i++)
        for (j = 0; j < coulumns.size() - 2; j++)
        {
            if (calcLength(coulumns[i], coulumns[j]) < table)
                graph[i].push_back(j);
        }
    for (i = 0; i < coulumns.size() - 2; i++)
    {
        if (coulumns[i].x - firstWall < table )
        {
            graph[i].push_back(coulumns.size() - 2);
            graph[coulumns.size() - 2].push_back(i);
        }
        if (secondWall - coulumns[i].x < table )
        {
            graph[i].push_back(coulumns.size() - 1);
            graph[coulumns.size() - 1].push_back(i);
        }
    }
    vector<bool> visited(coulumns.size(), false);
    dfs (graph, visited, coulumns.size() - 2);
    if (visited[coulumns.size() - 1])
        return(false);
    else
        return(true);

}

double searchMaxTable (vector<Point> &coulumns, double firstWall, double secondWall, double coulumnsR)
{
    double leftNumber = 0;
    double rightNumber = secondWall - firstWall;
    double tableCandidate = 0;
    while (rightNumber - leftNumber > 0.0001)
    {
    	tableCandidate = (leftNumber + rightNumber) / 2;
        if (ifTableFits(coulumns, firstWall, secondWall, coulumnsR, tableCandidate))
            leftNumber = tableCandidate;
        else
            rightNumber = tableCandidate;
    }
    return tableCandidate;
}

int main()
{
    double firstWall = 0;
    cin >> firstWall;
    double secondWall = 0;
    cin >> secondWall;
    double coulumnsR = 0;
    cin >> coulumnsR;
    double coulumnsNumber = 0;
    cin >> coulumnsNumber;
    vector<Point> coulumns;
    for ( int i = 0; i < coulumnsNumber; i++)
    {
    	Point coulumn;
        cin >> coulumn.x;
        cin >> coulumn.y;
        coulumns.push_back(coulumn);
    }
    double resolution = searchMaxTable(coulumns, firstWall, secondWall, coulumnsR);
    cout << setprecision(3) << fixed << round(resolution * 1000) / 1000;
}



