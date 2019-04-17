#include <cmath>
#include <iostream>
#include <vector>

using std::abs;
using std::cin;
using std::cout;
using std::vector;

class Vector {
public:
    Vector();
    
    Vector(double x, double y);
    
    double getX() const;
    
    double getY() const;
    
    double getPolarAngle() const;
    
    friend Vector operator+(const Vector &first, const Vector &second);
    
    friend Vector operator-(const Vector &first, const Vector &second);
    
private:
    double x;
    double y;
    double polarAngle;
};

class Polygon {
public:
    Polygon() = default;
    
    explicit Polygon(size_t vertNumber);
    
    explicit Polygon(const vector<Vector> &points);
    
    bool containsPoint(const Vector &point) const;
    
    Vector &operator[](int i);
    
    size_t size() const;
    
private:
    vector<Vector> vertices;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

Polygon minkowskiSum(Polygon &first, Polygon &second) {
    Polygon sum(first.size() + second.size());
    int i = 0;
    int j = 0;
    int k = 0;
    while (i < first.size() && j < second.size()) {
        sum[k] = first[i] + second[j];
        Vector firstEdge(first[i + 1] - first[i]);
        Vector secondEdge(second[j + 1] - second[j]);
        if (firstEdge.getPolarAngle() > secondEdge.getPolarAngle()) {
            i++;
        } else if (firstEdge.getPolarAngle() < secondEdge.getPolarAngle()) {
            j++;
        } else {
            i++;
            j++;
        }
        k++;
    }
    while (i < first.size()) {
        sum[k] = first[i] + second[j];
        i++;
        k++;
    }
    while (j < second.size()) {
        sum[k] = first[i] + second[j];
        j++;
        k++;
    }
    return sum;
}

int main() {
    int pointsNumber = 0;
    cin >> pointsNumber;
    vector<Vector> polygonPoints(pointsNumber);
    for (int i = 0; i < pointsNumber; i++) {
        double x;
        double y;
        cin >> x >> y;
        polygonPoints[i] = Vector(x, y);
    }
    Polygon first(polygonPoints);
    
    cin >> pointsNumber;
    polygonPoints.resize(pointsNumber);
    for (int i = 0; i < pointsNumber; i++) {
        double x;
        double y;
        cin >> x >> y;
        polygonPoints[i] = Vector(-x, -y);
    }
    Polygon second(polygonPoints);
    
    polygonPoints.erase(polygonPoints.begin());
    
    Polygon sum = minkowskiSum(first, second);
    if (sum.containsPoint(Vector(0, 0))) {
        cout << "YES";
    } else {
        cout << "NO";
    }
    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

Vector::Vector() : x(0), y(0), polarAngle(0) {}

Vector::Vector(double x, double y) : x(x), y(y) {
    polarAngle = atan2(y, x);
};

double Vector::getX() const {
    return x;
}

double Vector::getY() const {
    return y;
}

double Vector::getPolarAngle() const {
    return polarAngle;
}

Vector operator+(const Vector &first, const Vector &second) {
    double x = first.getX() + second.getX();
    double y = first.getY() + second.getY();
    return Vector(x, y);
}

Vector operator-(const Vector &first, const Vector &second) {
    double x = first.getX() - second.getX();
    double y = first.getY() - second.getY();
    return Vector(x, y);
}

Polygon::Polygon(size_t vertNumber) {
    vertices.resize(vertNumber + 1);
}

Polygon::Polygon(const vector<Vector> &points) {
    int begin = 0;
    for (size_t i = 0; i < points.size(); i++) {
        if (points[i].getY() < points[begin].getY() ||
            (points[i].getY() == points[begin].getY() && points[i].getX() > points[begin].getX())) {
            begin = static_cast<int>(i);
        }
    }
    vertices.resize(points.size());
    int currVertexNumber = begin;
    for (size_t i = 0; i < vertices.size(); i++) {
        if (currVertexNumber == points.size()) {
            currVertexNumber = 0;
        }
        vertices[i] = points[currVertexNumber];
        currVertexNumber++;
    }
    vertices.push_back(vertices[0]);
}

bool Polygon::containsPoint(const Vector &point) const {
    double x = point.getX();
    double y = point.getY();
    bool answer = false;
    for (size_t i = 1; i < vertices.size(); i++) {
        double firstX = vertices[i - 1].getX();
        double firstY = vertices[i - 1].getY();
        double secondX = vertices[i].getX();
        double secondY = vertices[i].getY();
        if (((y < secondY && y >= firstY) || (y < firstY && y >= secondY)) &&
            x < firstX + (secondX - firstX) * (y - firstY) / (secondY - firstY)) {
            answer = !answer;
        }
    }
    return answer;
}

size_t Polygon::size() const {
    return vertices.size() - 1;
}

Vector &Polygon::operator[](int i) {
    return vertices[i];
}
