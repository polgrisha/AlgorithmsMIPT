#include <algorithm>
#include <iomanip>
#include <iostream>
#include <math.h>

using std::cin;
using std::cout;
using std::make_pair;
using std::pair;
using std::sort;
using std::vector;

class Vector;

const double Precision = 0.000000001;

class Point {
public:
    Point() = default;
    
    Point(double first, double second, double third);
    
    Point(const Point &point);
    
    double getX() const;
    
    double getY() const;
    
    double getZ() const;
    
    Point &operator=(const Point &point);
    
    friend Point operator+(Point point, Vector vector);
    
private:
    double x;
    double y;
    double z;
};

class Vector {
public:
    Vector() = default;
    
    Vector(const Vector &inputVector);
    
    explicit Vector(const pair<Point, Point> &segment);
    
    Vector(double first, double second, double third);
    
    double getX() const;
    
    double getY() const;
    
    double getZ() const;
    
    double getLength() const;
    
    Vector &operator=(const Vector &other);
    
    Vector multByDouble(double number);
    
    friend double scalarProduct(const Vector &first, const Vector &second);
    
private:
    double x;
    double y;
    double z;
};

////////////////////////////////////////////////////////////////////////////////////

double countMinDist (const pair<Point, Point> &firstSegment, const pair<Point, Point> &secondSegment) {
    Vector first(firstSegment);
    Vector second(secondSegment);
    Vector third(make_pair(firstSegment.first, secondSegment.first));
    double coeff11 = scalarProduct(first, first);
    double coeff12 = scalarProduct(first, second);
    double coeff21 = scalarProduct(second, first);
    double coeff22 = scalarProduct(second, second);
    double coeff13 = -scalarProduct(first, third);
    double coeff23 = -scalarProduct(second, third);
    double denominator = coeff11 * coeff22 - coeff12 * coeff21;
    
    double firstLineShift = 0;
    double secondLineShift = 0;
    
    if (fabs(coeff11) < Precision) {
        firstLineShift = 0;
        if (fabs(coeff22) < Precision) {
            secondLineShift = 0;
        } else {
            secondLineShift = coeff23 / coeff22;
        }
    }
    else if (fabs(coeff22) < Precision) {
        secondLineShift = 0;
        if (fabs(coeff11) < Precision) {
            firstLineShift = 0;
        } else {
            firstLineShift = -coeff13 / coeff11;
        }
    } else {
        
        if (fabs(denominator) > Precision) {
            firstLineShift = -(coeff22 * coeff13 - coeff12 * coeff23) / denominator;
            secondLineShift = -(coeff13 * coeff21 - coeff11 * coeff23) / denominator;
        } else {
            firstLineShift = 0;
            secondLineShift = coeff23 / coeff22;
        }
        
        if (firstLineShift < 0) {
            firstLineShift = 0;
            secondLineShift = coeff23 / coeff22;
        }
        
        if (firstLineShift > 1) {
            firstLineShift = 1;
            secondLineShift = (coeff12 + coeff23) / coeff22;
        }
        
        if (secondLineShift < 0) {
            secondLineShift = 0;
            firstLineShift = -coeff13 / coeff11;
            if (firstLineShift < 0) {
                firstLineShift = 0;
            }
            if (firstLineShift > 1) {
                firstLineShift = 1;
            }
        }
        
        if (secondLineShift > 1) {
            secondLineShift = 1;
            firstLineShift = (coeff12 - coeff13) / coeff11;
            if (firstLineShift < 0) {
                firstLineShift = 0;
            }
            if (firstLineShift > 1) {
                firstLineShift = 1;
            }
        }
    }
    
    Point firstAnswerPoint = firstSegment.first + first.multByDouble(firstLineShift);
    Point secondAnswerPoint = secondSegment.first + second.multByDouble(secondLineShift);
    Vector answer(make_pair(firstAnswerPoint, secondAnswerPoint));
    return answer.getLength();
}

int main() {
    double x;
    double y;
    double z;
    cin >> x >> y >> z;
    Point segment11(x, y, z);
    cin >> x >> y >> z;
    Point segment12(x, y, z);
    cin >> x >> y >> z;
    Point segment21(x, y, z);
    cin >> x >> y >> z;
    Point segment22(x, y, z);
    cout << fixed << setprecision(10) << countMinDist(make_pair(segment11, segment12), make_pair(segment21, segment22));
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////

Point::Point(double first, double second, double third) : x(first), y(second), z(third) {}

Point::Point(const Point &point) : x(point.getX()), y(point.getY()), z(point.getZ()) {}

double Point::getX() const {
    return x;
}

double Point::getY() const {
    return y;
}

double Point::getZ() const {
    return z;
}

Point &Point::operator=(const Point &other) {
    if (this != &other) {
        x = other.getX();
        y = other.getY();
        z = other.getZ();
    }
    return *this;
}

Point operator+(Point point, Vector vector) {
    return Point(point.getX() + vector.getX(), point.getY() + vector.getY(), point.getZ() + vector.getZ());
}


Vector::Vector(const Vector &inputVector) {
    x = inputVector.x;
    y = inputVector.y;
    z = inputVector.z;
}

Vector::Vector(double first, double second, double third) : x(first), y(second), z(third) {}

double scalarProduct(const Vector &first, const Vector &second) {
    return first.x * second.x + first.y * second.y + first.z * second.z;
}

Vector operator*(Vector first, Vector second) {
    double x_ = first.y * second.z - first.z * second.y;
    double y_ = first.z * second.x - first.x * second.z;
    double z_ = first.x * second.y - first.y * second.x;
    return Vector(x_, y_, z_);
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

Vector::Vector(const pair<Point, Point> &segment) {
    x = segment.second.getX() - segment.first.getX();
    y = segment.second.getY() - segment.first.getY();
    z = segment.second.getZ() - segment.first.getZ();
}

Vector Vector::multByDouble(double number) {
    return Vector(x * number, y * number, z * number);
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
