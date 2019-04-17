#include <iostream>
#include <map>
#include <vector>

using std::cin;
using std::cout;
using std::map;
using std::ostream;
using std::string;
using std::vector;

const int AlphabetSize = 256;

//структура вершины
struct Node {
    explicit Node(int size);
    
    Node(int parent_, int left_, int right_, char nextSymbol, int nextVertex);
    
    bool has(char symbol) const;
    
    map<char, int> nextVertices;
    //родитель вершины
    int parent;
    //суффиксная ссылка
    int suffLink;
    //правая граница, за которую отвечает входящее ребро
    int left;
    //левая граница, за которую отвечает входящее ребро
    int right;
    //номер родителя
    int parentNumber;
};

//класс суффиксное дерево
class SuffixTrie {
public:
    SuffixTrie() = default;
    
    //построение дерева двух строк
    void buildTrie(const string &first, const string &second);
    
    //перегруженный оператор вывода
    friend ostream &operator<<(ostream &os, const SuffixTrie &trie);
    
    //обновление дерева при для нового символа строки
    void update(int symbolNumber, int &currVertex, int &edgePosition);
    
    //нумерация вершин для вывода
    void numerate(int vertex, int &currNumber);
    
    //продление листа
    int createLeaf(int symbolNumber, int parent);
    
    //раздление ребра для вставки вершины
    int splitEdge(int symbolNumber, int currVertex, int edgePosition);
    
private:
    //первая строка, по которой построено дерево
    string firstLine;
    //вторая строка, по которой построено дерево
    string secondLine;
    //вектор вершин дерева
    vector<Node> vertices;
    //нумерация вершин
    vector<int> numeration;
    //конкатенация первой и второй строки
    string line;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

int main() {
    string firstLine;
    cin >> firstLine;
    string secondLine;
    cin >> secondLine;
    SuffixTrie curr;
    curr.buildTrie(firstLine, secondLine);
    cout << curr;
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

int SuffixTrie::createLeaf(int symbolNumber, int parent) {
    Node leaf(line.size() - 1);
    leaf.parent = parent;
    leaf.left = symbolNumber;
    vertices.push_back(leaf);
    return vertices.size() - 1;
}

int SuffixTrie::splitEdge(int symbolNumber, int currVertex, int edgePosition) {
    char symbol = line[symbolNumber];
    
    //создаём вершину в середине
    Node divNode(vertices[currVertex].parent, vertices[currVertex].left, edgePosition - 1, line[edgePosition],
                 currVertex);
    vertices.push_back(divNode);
    int divNodeNumber = vertices.size() - 1;
    
    //обновляем родителя текущей
    int divNodeParent = vertices[divNodeNumber].parent;
    char parentSymbol = line[vertices[currVertex].left];
    vertices[divNodeParent].nextVertices[parentSymbol] = divNodeNumber;
    
    //обновляем текущую
    vertices[currVertex].parent = divNodeNumber;
    vertices[currVertex].left = edgePosition;
    
    //создаём лист
    Node leaf(line.size() - 1);
    leaf.parent = divNodeNumber;
    leaf.left = symbolNumber;
    vertices.push_back(leaf);
    int leafNumber = vertices.size() - 1;
    
    vertices[divNodeNumber].nextVertices[symbol] = leafNumber;
    return divNodeNumber;
}

void SuffixTrie::update(int symbolNumber, int &currVertex, int &edgePosition) {
    char symbol = line[symbolNumber];
    //продление суффикса
    while (true) {
        if (vertices[currVertex].right < edgePosition) {
            //переход по первому правилу
            if (!vertices[currVertex].has(line[symbolNumber])) {
                int leaf = createLeaf(symbolNumber, currVertex);
                
                vertices[currVertex].nextVertices[symbol] = leaf;
                currVertex = vertices[currVertex].suffLink;
                edgePosition = vertices[currVertex].right + 1;
                
                continue;
            } else {
                currVertex = vertices[currVertex].nextVertices[symbol];
                edgePosition = vertices[currVertex].left;
            }
        }
        
        //если зашли в фиктивную вершину
        if (edgePosition == -1) {
            edgePosition++;
            break;
        }
        
        // переход по второму правилу
        if (symbol != line[edgePosition]) {
            //разделение ребра на две части
            int divNodeNumber = splitEdge(symbolNumber, currVertex, edgePosition);
            
            //переход по суффиксной ссылке
            currVertex = vertices[vertices[divNodeNumber].parent].suffLink;
            edgePosition = vertices[divNodeNumber].left;
            
            //прыгаем вниз по ребру
            while (edgePosition <= vertices[divNodeNumber].right) {
                currVertex = vertices[currVertex].nextVertices[line[edgePosition]];
                edgePosition += vertices[currVertex].right - vertices[currVertex].left + 1;
            }
            
            if (edgePosition == vertices[divNodeNumber].right + 1) {
                vertices[divNodeNumber].suffLink = currVertex;
            } else {
                vertices[divNodeNumber].suffLink = vertices.size();
            }
            edgePosition = vertices[currVertex].right - (edgePosition - vertices[divNodeNumber].right) + 2;
        }
        //переход по третьему правилу
        else {
            edgePosition++;
            break;
        }
    }
}

void SuffixTrie::buildTrie(const string &first, const string &second) {
    firstLine = first;
    secondLine = second;
    line = firstLine + secondLine;
    
    //фиктивная вершина, в котрую ведёт суфф ссылка корня
    Node dummy(line.size() - 1);
    dummy.parent = -1;
    dummy.right = -1;
    dummy.left = -1;
    
    for (int i = 0; i < 256; i++) {
        dummy.nextVertices[i] = 1;
    }
    
    Node root(line.size() - 1);
    root.parent = 0;
    root.right = -1;
    root.left = -1;
    root.suffLink = 0;
    
    vertices.push_back(dummy);
    vertices.push_back(root);
    
    int edgePosition = 0;
    int currVertex = 1;
    
    for (int i = 0; i < line.size(); i++) {
        update(i, currVertex, edgePosition);
    }
    
    //нумерация вершин для последующего вывода
    int currNumber = -1;
    numerate(1, currNumber);
}

void SuffixTrie::numerate(int vertex, int &currNumber) {
    currNumber++;
    numeration.push_back(vertex);
    int parentNumber = currNumber;
    for (const auto &i: vertices[vertex].nextVertices) {
        vertices[i.second].parentNumber = parentNumber;
        numerate(i.second, currNumber);
    }
}

ostream &operator<<(ostream &os, const SuffixTrie &trie) {
    os << trie.vertices.size() - 1 << "\n";
    for (int i = 1; i < trie.numeration.size(); i++) {
        Node vertex = trie.vertices[trie.numeration[i]];
        os << vertex.parentNumber << " ";
        if (vertex.left < trie.firstLine.size() && vertex.right == trie.line.size() - 1) {
            vertex.right = trie.firstLine.size() - 1;
        }
        if (vertex.left >= trie.firstLine.size()) {
            os << 1 << " " << vertex.left - trie.firstLine.size() << " " << vertex.right - trie.firstLine.size() + 1 << "\n";
        } else {
            os << 0 << " " << vertex.left << " " << vertex.right + 1 << "\n";
        }
    }
    return os;
}

Node::Node(int size) : right(size) {}

bool Node::has(char symbol) const {
    return nextVertices.find(symbol) != nextVertices.end();
}

Node::Node(int parent_, int left_, int right_, char nextSymbol, int nextVertex) : parent(parent_),
left(left_),
right(right_) {
    nextVertices[nextSymbol] = nextVertex;
}
