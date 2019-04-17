#include <algorithm>
#include <cfloat>
#include <cmath>
#include <iostream>
#include <limits.h>
#include <queue>
#include <set>


using std::initializer_list;
using std::cin;
using std::cout;
using std::make_pair;
using std::pair;
using std::queue;
using std::set;
using std::sort;
using std::vector;

const double cPrecision = 0.000000001;

class Point {
public:
    Point() = default;
    
    Point(int x_, int y_, int z_, int number_);
    
    Point(const Point &point);
    
    int getX() const;
    
    int getY() const;
    
    int getZ() const;
    
    int getNumber() const;
    
    Point &operator=(const Point &other);
    
    friend bool operator!=(const Point &left, const Point &right);
    
    friend bool operator<(const Point &first, const Point &second);
    
private:
    int x;
    int y;
    int z;
    int number;
};

class Vector {
public:
    Vector() = default;
    
    Vector(const Vector &inputVector);
    
    explicit Vector(pair<Point, Point> segment);
    
    Vector(double x_, double y_, double z_);
    
    void changeDirection();
    
    double getLength() const;
    
    double getX() const;
    
    double getY() const;
    
    double getZ() const;
    
    Vector &operator=(const Vector &other);
    
    friend Vector operator/(const Vector &vector, double number);
    
    friend Vector operator-(const Vector &left, const Vector &right);
    
    friend double dotProduct(const Vector &first, const Vector &second);
    
    friend Vector operator*(Vector first, Vector second);
    
private:
    double x;
    double y;
    double z;
};

class Face {
public:
    Face() = default;
    
    Face(const Face &other);
    
    Face(const Point &first_, const Point &second_, const Point &third_);
    
    Face &operator=(const Face &other);
    
    void sort();
    
    int getFirstEdge() const;
    
    int getSecondEdge() const;
    
    int getThirdEdge() const;
    
    vector<pair<int, int>> getDirectEdges() const;
    
private:
    int first;
    int second;
    int third;
    vector<pair<int, int>> directEdges;
};

bool cmp(const Face &left, const Face &right);

//нахождние первой грани оболочки
Face findFirstFace(const vector<Point> &points) {
    Point firstMin(INT_MAX, INT_MAX, INT_MAX, 0);
    Point secondMin;
    //нахождение первой точки грани
    for (size_t i = 0; i < points.size(); i++) {
        if (points[i] < firstMin) {
            firstMin = points[i];
        }
    }
    double minAngle = DBL_MAX;
    //нахождение точки, такой что отрезок вектор (firstMin, secondMin) имеет наибольший угол с осью ординат
    for (size_t i = 0; i < points.size(); i++) {
        Vector toCheck(make_pair(firstMin, points[i]));
        Vector ox(1, 0, 0);
        double angle = dotProduct(toCheck, ox) / (toCheck.getLength() * ox.getLength());
        if (angle < minAngle) {
            minAngle = angle;
            secondMin = points[i];
        }
    }
    int thirdMinNumber = 0;
    //нахождение третьей точки, такой что грань образованная найденными тремя точками
    // образует ниабольший угол среди всех остальных и что она по часовой стрелке ориентирована относительно
    // внешний нормали к грани
    if (firstMin.getNumber() == 0 || secondMin.getNumber() == 0) {
        thirdMinNumber++;
        if (firstMin.getNumber() == 1 || secondMin.getNumber() == 1) {
            thirdMinNumber++;
        }
    }
    Vector firstVector(make_pair(firstMin, secondMin));
    Vector secondVector(make_pair(secondMin, points[thirdMinNumber]));
    Vector firstNormal(firstVector * secondVector);
    minAngle = DBL_MAX;
    for (size_t i = 0; i < points.size(); i++) {
        if (i != firstMin.getNumber() && i != secondMin.getNumber() && i != thirdMinNumber) {
            Vector toCheck(make_pair(secondMin, points[i]));
            Vector currNormal(firstVector * toCheck);
            Vector normal(firstNormal * currNormal);
            double angle = dotProduct(firstVector, normal);
            if (angle >= 0) {
                angle = dotProduct(firstNormal, currNormal) / (firstNormal.getLength() * currNormal.getLength());
                if (angle < minAngle) {
                    minAngle = angle;
                    thirdMinNumber = static_cast<int>(i);
                }
            }
        }
    }
    Point thirdMin = points[thirdMinNumber];
    return Face(firstMin, secondMin, thirdMin);
}

//нахождние точки, что грань образованная ею и двумя точками рассматриваемой грани имеет наибольший угол
Point maxAnglePoint(const vector<Point> &points, const Face &face, pair<Point, Point> edge) {
    vector<Vector> vectorEdges;
    Vector vectorEdge(edge);
    for (auto currEdge : face.getDirectEdges()) {
        vectorEdges.emplace_back(Vector(make_pair(points[currEdge.first], points[currEdge.second])));
    }
    Vector first(vectorEdges[0]);
    Vector second(vectorEdges[2]);
    second.changeDirection();
    //нормаль к обрабатываемой грани
    Vector normal(first * second);
    normal = normal / normal.getLength();
    //вектор параллельный обрабатываемой грани
    Vector checker(normal * vectorEdge);
    checker = checker / checker.getLength();
    Point min;
    double minAngle = DBL_MAX;
    for (auto point : points) {
        if (point != points[face.getFirstEdge()] && point != points[face.getSecondEdge()] &&
            point != points[face.getThirdEdge()]) {
            Vector toCheck(make_pair(edge.first, point));
            double length = dotProduct(toCheck, vectorEdge) / vectorEdge.getLength();
            if (length != 0) {
                double coeff = vectorEdge.getLength() / length;
                Vector corrVectorEdge = vectorEdge / coeff;
                toCheck = toCheck - corrVectorEdge;
            }
            double angle = dotProduct(checker, toCheck);
            angle = angle / (checker.getLength() * toCheck.getLength());
            if (angle < minAngle) {
                minAngle = angle;
                min = point;
            }
        }
    }
    return min;
}

//добавление рёбер грани, лежащих в текущей оболочке
void addFace(set<pair<int, int>> &hull, const Face &face) {
    for (auto edge : face.getDirectEdges()) {
        pair<int, int> reversedEdge(edge.second, edge.first);
        if (hull.count(reversedEdge)) {
            hull.erase(reversedEdge);
        } else {
            hull.insert(edge);
        }
    }
}

//построение выпуклой оболочки множества
vector<Face> buildConvexHull(const vector<Point> &points) {
    queue<Face> faces;
    vector<Face> answer;
    Face firstFace = findFirstFace(points);
    faces.push(firstFace);
    set<pair<int, int>> hull;
    for (auto edge : firstFace.getDirectEdges()) {
        hull.insert(edge);
    }
    while (!faces.empty()) {
        Face currFace(faces.front());
        faces.pop();
        for (auto edge : currFace.getDirectEdges()) {
            if (hull.count(edge)) {
                pair<Point, Point> pointEdge(points[edge.first], points[edge.second]);
                Point newPoint = maxAnglePoint(points, currFace, pointEdge);
                Face newFace(pointEdge.second, pointEdge.first, newPoint);
                addFace(hull, newFace);
                faces.push(newFace);
            }
        }
        answer.push_back(currFace);
    }
    return answer;
}

//////////////////////////////////////////////////////////////////////////////////////////

int main() {
    int number = 0;
    int testsNumber = 1;
    cin >> testsNumber;
    for (int j = 0; j < testsNumber; j++) {
        cin >> number;
        vector<Point> points;
        for (int i = 0; i < number; i++) {
            int x;
            int y;
            int z;
            cin >> x >> y >> z;
            points.emplace_back(Point(x, y, z, i));
        }
        vector<Face> faces = buildConvexHull(points);
        for (size_t i = 0; i < faces.size(); i++) {
            faces[i].sort();
        }
        sort(faces.begin(), faces.end(), cmp);
        cout << faces.size() << "\n";
        for (auto face : faces) {
            cout << "3 " << face.getFirstEdge() << " "
            << face.getSecondEdge() << " " << face.getThirdEdge() << "\n";
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////

Point::Point(int x_, int y_, int z_, int number_) : x(x_), y(y_), z(z_), number(number_) {}

Point::Point(const Point &point) : x(point.getX()), y(point.getY()), z(point.getZ()), number(point.getNumber()) {}

int Point::getX() const {
    return x;
};

int Point::getY() const {
    return y;
};

int Point::getZ() const {
    return z;
};

bool operator<(const Point &first, const Point &second) {
    if (first.x < second.x) {
        return true;
    } else if (fabs(first.x - second.x) < cPrecision && first.y < second.y) {
        return true;
    } else {
        return fabs(first.x - second.x) < cPrecision &&
        fabs(first.y - second.y) < cPrecision && first.z < second.z;
    }
}

bool operator!=(const Point &left, const Point &right) {
    return left.getNumber() != right.getNumber();
}

Point &Point::operator=(const Point &other) {
    if (this != &other) {
        x = other.getX();
        y = other.getY();
        z = other.getZ();
        number = other.getNumber();
    }
    return *this;
}

int Point::getNumber() const {
    return number;
}


Vector::Vector(const Vector &inputVector) : x(inputVector.x), y(inputVector.y), z(inputVector.z) {}

Vector::Vector(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}

double dotProduct(const Vector &first, const Vector &second) {
    return first.x * second.x + first.y * second.y + first.z * second.z;
}

Vector operator*(Vector first, Vector second) {
    double x_ = first.y * second.z - first.z * second.y;
    double y_ = first.z * second.x - first.x * second.z;
    double z_ = first.x * second.y - first.y * second.x;
    return Vector(x_, y_, z_);
}

void Vector::changeDirection() {
    x = -x;
    y = -y;
    z = -z;
}

double Vector::getLength() const {
    return sqrt(x * x + y * y + z * z);
}

Vector &Vector::operator=(const Vector &other) {
    if (this != &other) {
        x = other.x;
        y = other.y;
        z = other.z;
    }
    return *this;
}

Vector::Vector(pair<Point, Point> segment) : x(segment.second.getX() - segment.first.getX()),
y(segment.second.getY() - segment.first.getY()),
z(segment.second.getZ() - segment.first.getZ()) {}

Vector operator-(const Vector &left, const Vector &right) {
    return Vector(left.x - right.x, left.y - right.y, left.z - right.z);
}

Vector operator/(const Vector &vector, double number) {
    return Vector(vector.x / number, vector.y / number, vector.z / number);
}

double Vector::getX() const {
    return x;
}

double Vector::getY() const {
    return y;
}

double Vector::getZ() const {
    return z;
}

Face::Face(const Point &first_, const Point &second_, const Point &third_) {
    first = first_.getNumber();
    second = second_.getNumber();
    third = third_.getNumber();
    directEdges.emplace_back(make_pair(first, second));
    directEdges.emplace_back(make_pair(second, third));
    directEdges.emplace_back(make_pair(third, first));
}

bool cmp(const Face &left, const Face &right) {
    if (left.getFirstEdge() < right.getFirstEdge()) {
        return true;
    } else if (left.getFirstEdge() == right.getFirstEdge() &&
               left.getSecondEdge() < right.getSecondEdge()) {
        return true;
    } else {
        return left.getFirstEdge() == right.getFirstEdge() &&
        left.getSecondEdge() == right.getSecondEdge() &&
        left.getThirdEdge() < right.getThirdEdge();
    }
}

void Face::sort() {
    if (first < second && first < third) {
        return;
    }
    if (second < first && second < third) {
        int changer = second;
        second = third;
        third = first;
        first = changer;
    }
    if (third < first && third < second) {
        int changer = third;
        third = second;
        second = first;
        first = changer;
    }
}

Face::Face(const Face &other) : first(other.first),
second(other.second),
third(other.third),
directEdges(other.directEdges) {}

Face &Face::operator=(const Face &other) {
    if (this != &other) {
        first = other.first;
        second = other.second;
        third = other.third;
        directEdges = other.directEdges;
    }
    return *this;
}

int Face::getFirstEdge() const {
    return first;
}

int Face::getSecondEdge() const {
    return second;
}

int Face::getThirdEdge() const {
    return third;
}

vector<pair<int, int>> Face::getDirectEdges() const {
    return directEdges;
}

