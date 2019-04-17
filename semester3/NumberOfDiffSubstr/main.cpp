#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::ostream;
using std::string;
using std::vector;

//константы
const int alphabet = 256;

class SuffArray {
public:
    explicit SuffArray(const string &currLine);
    vector<int> lcp() const;
    friend int numberOfDiffSubstrings(const SuffArray &array);
    friend ostream &operator<<(ostream &os, const SuffArray &array);
private:
    void buildSuffArray();
    vector<int> suffArray;
    const string line;
    vector<vector<int>> allEquivClasses;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

int main() {
    string line;
    getline(cin, line);
    SuffArray suff(line);
    cout << suff;
    cout << "\n";
    cout << numberOfDiffSubstrings(suff);
    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

SuffArray::SuffArray(const string &currLine) : line(currLine) {
    buildSuffArray();
}

//подсчёт LCP для соседних элементов суфф. массива
vector<int> SuffArray::lcp() const {
    //все lcp соседних элементов в суфф. массиве
    vector<int> answer(suffArray.size() - 1, 0);
    
    for (int i = 0; i < suffArray.size() - 1; i++) {
        int shift = 1 << (allEquivClasses.size() - 1);
        int first = suffArray[i];
        int second = suffArray[i + 1];
        for (int j = allEquivClasses.size() - 1; j >= 0; j--) {
            if (allEquivClasses[j][first] == allEquivClasses[j][second]) {
                first += shift;
                second += shift;
                answer[i] += shift;
            }
            shift /= 2;
        }
    }
    return answer;
}

//подсчёт количества различных подстрок
int numberOfDiffSubstrings(const SuffArray &array) {
    int answer = 0;
    vector<int> neighborLcp = array.lcp();
    for (int i = 1; i < array.suffArray.size(); i++) {
        answer += array.suffArray.size() - 1 - array.suffArray[i] - neighborLcp[i - 1];
    }
    return answer;
}

//пострение суфф. массива
void SuffArray::buildSuffArray() {
    //Добавление наименьшего символа для сортировки суффиксов вместо цикл. подстрок
    string currLine = line + "#";
    suffArray.assign(currLine.size(), 0);
    vector<int> equivClasses(currLine.size(), 0);
    
    //сортировка подсчетом циклических подстрок длины 1
    vector<int> sortArray(alphabet, 0);
    for (char symbol : currLine) {
        sortArray[symbol]++;
    }
    int changer = 0;
    int groupBegin = 0;
    for (int i = 0; i < sortArray.size(); i++) {
        changer = sortArray[i];
        sortArray[i] = groupBegin;
        groupBegin += changer;
    }
    for (int i = 0; i < currLine.size(); i++) {
        suffArray[sortArray[currLine[i]]] = i;
        sortArray[currLine[i]]++;
    }
    
    //подсчёт классов эквивалентности
    int currClass = 0;
    for (int i = 1; i < currLine.size(); i++) {
        if (currLine[suffArray[i]] != currLine[suffArray[i - 1]]) {
            currClass++;
        }
        equivClasses[suffArray[i]] = currClass;
    }
    int numberOfClasses = currClass + 1;
    
    allEquivClasses.push_back(equivClasses);
    
    //фаза алгоритма
    vector<int> currCycleLines(line.size(), 0);
    for (int i = 1; i < currLine.size(); i *= 2) {
        
        //сортируем по вторым частям
        vector<int> tmpArray = suffArray;
        for (int j = 0; j < currLine.size(); j++) {
            tmpArray[j] -= i;
            if (tmpArray[j] < 0) {
                tmpArray[j] += currLine.size();
            }
        }
        
        //сортировка подсчетом по первым частям (по вторым уже отсортирован)
        sortArray.assign(numberOfClasses, 0);
        for (int j = 0; j < tmpArray.size(); j++) {
            sortArray[equivClasses[tmpArray[j]]]++;
        }
        changer = 0;
        groupBegin = 0;
        for (int j = 0; j < sortArray.size(); j++) {
            changer = sortArray[j];
            sortArray[j] = groupBegin;
            groupBegin += changer;
        }
        for (int j = 0; j < tmpArray.size(); j++) {
            suffArray[sortArray[equivClasses[tmpArray[j]]]] = tmpArray[j];
            sortArray[equivClasses[tmpArray[j]]]++;
        }
        
        //подсчёт классов эквивалентности
        currClass = 0;
        vector<int> tmpClasses = equivClasses;
        tmpClasses[suffArray[0]] = 0;
        for (int j = 1; j < suffArray.size(); j++) {
            int firstPart1 = equivClasses[suffArray[j - 1]];
            int firstPart2 = equivClasses[suffArray[j]];
            int secondPart1 = equivClasses[(suffArray[j - 1] + i) % suffArray.size()];
            int secondPart2 = equivClasses[(suffArray[j] + i) % suffArray.size()];
            
            if (firstPart1 != firstPart2 || secondPart1 != secondPart2) {
                currClass++;
            }
            
            tmpClasses[suffArray[j]] = currClass;
        }
        numberOfClasses = currClass + 1;
        equivClasses = tmpClasses;
        
        allEquivClasses.push_back(equivClasses);
    }
}

ostream &operator<<(ostream &os, const SuffArray &array) {
    for (int i = 0; i < array.suffArray.size(); i++) {
        os << array.suffArray[i] << " ";
    }
    return os;
}

