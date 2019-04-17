#include <iostream>
#include <string>
#include <vector>

using std::istream;
using std::ostream;
using std::string;
using std::vector;

class BigInteger {
public:

    BigInteger() = default;
    BigInteger(int number);
    BigInteger(const string &strNumber);
    BigInteger(const BigInteger &other);

    BigInteger &operator=(int other);
    BigInteger &operator=(const BigInteger &other);

    friend BigInteger summOfPositive(const BigInteger &left, const BigInteger &right);
    friend BigInteger diffOfPositive(const BigInteger &left, const BigInteger &right);

    friend BigInteger abs(const BigInteger &value);
    BigInteger operator-() const;
    string toString() const;
    explicit operator bool() const;

    friend BigInteger operator+(const BigInteger &left, const BigInteger &right);
    friend BigInteger operator-(const BigInteger &left, const BigInteger &right);
    friend BigInteger operator*(const BigInteger &left, const BigInteger &right);
    friend BigInteger operator/(const BigInteger &left, const BigInteger &right);
    friend BigInteger operator%(const BigInteger &left, const BigInteger &right);

    BigInteger &operator++();
    BigInteger operator++(int);
    BigInteger &operator--();
    BigInteger operator--(int);

    BigInteger &operator+=(const BigInteger &other);
    BigInteger &operator-=(const BigInteger &other);
    BigInteger &operator*=(const BigInteger &other);
    BigInteger &operator/=(const BigInteger &other);
    BigInteger &operator%=(const BigInteger &other);

    friend bool operator<(const BigInteger &left, const BigInteger &right);
    friend bool operator<=(const BigInteger &left, const BigInteger &right);
    friend bool operator>(const BigInteger &left, const BigInteger &right);
    friend bool operator>=(const BigInteger &left, const BigInteger &right);
    friend bool operator==(const BigInteger &left, const BigInteger &right);
    friend bool operator!=(const BigInteger &left, const BigInteger &right);

    friend ostream &operator<<(ostream &os, const BigInteger &number);
    friend istream &operator>>(istream &is, BigInteger &number);

private:

    void leftShift();
    void rightShift();

    void normalize();

    vector<int> digits_;
    bool isNegative_ = false;
};

ostream &operator<<(ostream &os, const BigInteger &number) {
    if (number.isNegative_) {
        os << '-';
    }
    for (size_t i = number.digits_.size() - 1; i < number.digits_.size(); i--) {
        os << number.digits_[i];
    }
    return os;
}

istream &operator>>(istream &is, BigInteger &number) {
    string strNumber;
    is >> strNumber;
    number = BigInteger(strNumber);
    return is;
}

BigInteger::BigInteger(int number) {
    if (number == 0) {
        digits_.push_back(0);
        isNegative_ = false;
        return;
    }
    if (number < 0) {
        isNegative_ = true;
        number = -number;
    } else {
        isNegative_ = false;
    }
    while (number > 0) {
        int digit = number % 10;
        digits_.push_back(digit);
        number /= 10;
    }
    this->normalize();
}

BigInteger::BigInteger(const string &strNumber) {
    if (strNumber.empty() || strNumber == "-") {
        *this = BigInteger(0);
    }
    else {
        for (size_t i = strNumber.size() - 1; i >= 1; i--) {
            digits_.push_back(strNumber[i] - '0');
        }
        if (strNumber[0] == '-') {
            isNegative_ = true;
        } else {
            digits_.push_back(strNumber[0] - '0');
        }
        this -> normalize();
    }
}

BigInteger::BigInteger(const BigInteger &other) {
    isNegative_ = other.isNegative_;
    digits_ = other.digits_;
}

BigInteger &BigInteger::operator=(const BigInteger &other) {
    if (this != &other) {
        isNegative_ = other.isNegative_;
        digits_ = other.digits_;
    }
    return *this;
}

BigInteger operator+(const BigInteger &left, const BigInteger &right){
    if (!left.isNegative_ && !right.isNegative_) {
        return summOfPositive(left, right);
    } else if (left.isNegative_ && right.isNegative_){
        return -summOfPositive(-left, -right);
    } else if (left.isNegative_ && !right.isNegative_) {
        return diffOfPositive(right, -left);
    } else if (!left.isNegative_ && right.isNegative_) {
        return diffOfPositive(left, -right);
    }
    return summOfPositive(left, right);
}

BigInteger operator-(const BigInteger &left, const BigInteger &right) {
    return left + (-right);
}

BigInteger summOfPositive(const BigInteger &left, const BigInteger &right) {
    int carry = 0;
    BigInteger answer;
    vector<int> leftDigits(left.digits_);
    vector<int> rightDigits(right.digits_);
    while (leftDigits.size() < rightDigits.size()) {
        leftDigits.push_back(0);
    }
    while (rightDigits.size() < leftDigits.size()) {
        rightDigits.push_back(0);
    }
    rightDigits.push_back(0);
    leftDigits.push_back(0);
    for (size_t i = 0; i < leftDigits.size(); i++) {
        int currSumm = leftDigits[i] + rightDigits[i] + carry;
        answer.digits_.push_back(currSumm % 10);
        carry = currSumm / 10;
    }
    answer.normalize();
    return answer;
}

BigInteger diffOfPositive(const BigInteger &left, const BigInteger &right) {
    if (left == right) {
        return BigInteger(0);
    }
    if (left > right){
        BigInteger answer;
        int carry = 0;
        vector<int> leftDigits = left.digits_;
        vector<int> rightDigits = right.digits_;
        while (rightDigits.size() <= leftDigits.size()) {
            rightDigits.push_back(0);
        }
        while (leftDigits.size() <= rightDigits.size()) {
            leftDigits.push_back(0);
        }
        for (size_t i = 0; i < left.digits_.size(); i++) {
            int currDigit = leftDigits[i] - rightDigits[i] - carry;
            if (currDigit < 0) {
                currDigit += 10;
                carry = 1;
            } else {
                carry = 0;
            }
            answer.digits_.push_back(currDigit);
        }
        answer.normalize();
        return answer;
    } else {
        return -diffOfPositive(right, left);
    }
}

BigInteger BigInteger::operator-() const {
    BigInteger answer = *this;
    if (answer != BigInteger(0)) {
        answer.isNegative_ = !answer.isNegative_;
    }
    return answer;
}

bool operator<(const BigInteger &left, const BigInteger &right) {
    bool answer = false;
    if (left.isNegative_ == right.isNegative_) {
        if (left.digits_.size() < right.digits_.size()) {
            answer = true;
        } else if (left.digits_.size() > right.digits_.size()) {
            answer = false;
        } else {
            size_t currDigit;
            currDigit = left.digits_.size() - 1;
            while (currDigit < left.digits_.size() && left.digits_[currDigit] == right.digits_[currDigit]) {
                currDigit--;
            }
            if (currDigit > left.digits_.size()) {
                return false;
            } else {
                answer = left.digits_[currDigit] < right.digits_[currDigit];
            }
        }
        if (left.isNegative_ && right.isNegative_) {
            answer = !answer;
        }
    } else if (left.isNegative_ && !right.isNegative_) {
        answer = true;
    } else if (!left.isNegative_ && right.isNegative_) {
        answer = false;
    }
    return answer;
}

bool operator==(const BigInteger &left, const BigInteger &right) {
    if (left.isNegative_ != right.isNegative_) {
        return false;
    }
    if (left.digits_.size() != right.digits_.size()) {
        return false;
    }
    if (left.digits_ != right.digits_) {
        return false;
    }
    return true;
}

bool operator>(const BigInteger &left, const BigInteger &right) {
    return !(left < right || left == right);
}

BigInteger &BigInteger::operator=(int other) {
    *this = BigInteger(other);
    return *this;
}

BigInteger operator*(const BigInteger &left, const BigInteger &right) {
    BigInteger answer;
    if (left == BigInteger(0) || right == BigInteger(0)) {
        return BigInteger(0);
    }
    answer.digits_.assign(left.digits_.size() + right.digits_.size(), 0);
    for (size_t i = 0; i < left.digits_.size(); i++) {
        for (size_t j = 0; j < right.digits_.size(); j++) {
            answer.digits_[i + j] += left.digits_[i] * right.digits_[j];
        }
    }
    int carry = 0;
    for (size_t i = 0; i < answer.digits_.size(); i++) {
        answer.digits_[i] += carry;
        carry = answer.digits_[i] / 10;
        answer.digits_[i] %= 10;
    }
    answer.normalize();
    answer.isNegative_ = left.isNegative_ ^ right.isNegative_;
    return answer;
}

bool operator<=(const BigInteger &left, const BigInteger &right) {
    return left < right || left == right;
}

bool operator>=(const BigInteger &left, const BigInteger &right) {
    return left > right || left == right;
}

bool operator!=(const BigInteger &left, const BigInteger &right) {
    return !(left == right);
}

BigInteger operator/(const BigInteger &left, const BigInteger &right) {
    if (abs(left) < abs(right)) {
        return BigInteger(0);
    }
    BigInteger minued = abs(left);
    BigInteger answer;
    BigInteger subtractor = abs(right);
    while (subtractor.digits_.size() < minued.digits_.size()) {
        subtractor.leftShift();
    }
    while (subtractor.digits_.size() >= right.digits_.size()) {
        int quotentDigit = 0;
        while (minued - subtractor >= BigInteger(0)) {
            quotentDigit++;
            minued -= subtractor;
        }
        answer.digits_.insert(answer.digits_.begin(), quotentDigit);
        subtractor.rightShift();
    }
    answer.normalize();
    answer.isNegative_ = left.isNegative_ ^ right.isNegative_;
    return answer;
}

BigInteger operator%(const BigInteger &left, const BigInteger &right) {
    return left - ((left / right) * right);
}

BigInteger &BigInteger::operator+=(const BigInteger &other) {
    *this = *this + other;
    return *this;
}

BigInteger &BigInteger::operator-=(const BigInteger &other) {
    *this = *this - other;
    return *this;
}

BigInteger &BigInteger::operator/=(const BigInteger &other) {
    *this = *this / other;
    return *this;
}

void BigInteger::leftShift() {
    digits_.insert(digits_.begin(), 0);
}

void BigInteger::rightShift() {
    digits_.erase(digits_.begin());
}

BigInteger &BigInteger::operator%=(const BigInteger &other) {
    *this = *this % other;
    return *this;
}

BigInteger &BigInteger::operator++() {
    *this += BigInteger(1);
    return *this;
}

BigInteger BigInteger::operator++(int) {
    BigInteger returnValue = *this;
    *this += BigInteger(1);
    return returnValue;
}

BigInteger &BigInteger::operator--() {
    *this -= BigInteger(1);
    return *this;
}

BigInteger BigInteger::operator--(int) {
    BigInteger returnValue = *this;
    *this -= BigInteger(1);
    return returnValue;
}

string BigInteger::toString() const {
    string returnValue;
    if (isNegative_) {
        returnValue += "-";
    }
    for (size_t i = digits_.size() - 1; i < digits_.size(); i--) {              ///size_t problem
        returnValue += static_cast<char>(digits_[i] + '0');
    }
    return returnValue;
}

BigInteger abs(const BigInteger &value) {
    BigInteger returnValue = value;
    returnValue.isNegative_ = false;
    return returnValue;
}

BigInteger::operator bool() const {
    return *this != BigInteger(0);
}

BigInteger &BigInteger::operator*=(const BigInteger &other) {
    *this = *this * other;
    return *this;
}

void BigInteger::normalize() {
    if (digits_.empty()) {
        digits_.push_back(0);
        isNegative_ = false;
    } else {
        while (digits_.size() > 1 && digits_.back() == 0) {
            digits_.pop_back();
        }
        if(digits_.size() == 1 && digits_[0] == 0) {
            isNegative_ = false;
        }
    }
}
