#include <iostream>
#include <vector>

bool findSink ( std::vector<std::vector<int>> matrix )
{
    int sinkCandidate = 0;
    for ( int i=1; i < matrix.size(); i++)
    {
        if ( matrix[sinkCandidate][i] == 1 && matrix[i][sinkCandidate] == 1)
        {
            sinkCandidate = i;
            i++;
        }
        if( matrix[sinkCandidate][i] == 1 && matrix[i][sinkCandidate] == 0)
            sinkCandidate = i;
    }
    bool sinkExist = true;
    for ( int i = 0; i < matrix.size(); i++ )
        if ( matrix[sinkCandidate][i] == 1 )
            sinkExist = false;
    if ( matrix[sinkCandidate][sinkCandidate] == 1)
        sinkExist = false;
    for ( int i = 0; i < matrix.size(); i++ )
        if ( matrix[i][sinkCandidate] == 0 && i != sinkCandidate )
            sinkExist = false;
    return sinkExist;
}

int main()
{
    int vertices = 0;
    int vertex = 0;
    std::cin >> vertices;
    std::vector<std::vector<int>> matrix(vertices);
    for( int i = 0; i < vertices; i++)
        for( int j = 0; j < vertices; j++ )
        {
            std::cin >> vertex;
            matrix[i].push_back(vertex);
        }
    if ( findSink(matrix) == true)
        std::cout << "YES";
    else
        std::cout << "NO";
}


