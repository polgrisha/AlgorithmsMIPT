#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::make_pair;
using std::pair;
using std::string;
using std::vector;

//константы
const int alphabet = 256;

class SuffArray {
public:
    SuffArray(const string &firstLine, const string &secondLine);

    long long calcLcp(long long first, long long second);

    pair<bool, string> calcCommonSubstring(long long number);

private:
    void buildSuffArray();

    vector<long long> suffArray;
    vector<vector<long long>> allEquivClasses;
    vector<bool> affixToLine;
    const string line;
};

int main() {
    string firstLine;
    string secondLine;
    getline(cin, firstLine);
    getline(cin, secondLine);
    long long number;
    cin >> number;
    SuffArray suffArray(firstLine, secondLine);
    pair<bool, string> answer = suffArray.calcCommonSubstring(number);
    if (answer.first) {
        cout << answer.second;
    } else {
        cout << "-1";
    }
    return 0;
}

void SuffArray::buildSuffArray() {
    string currLine = line;
    suffArray.assign(currLine.size(), 0);
    vector<long long> equivClasses(currLine.size(), 0);

    //сортировка подсчетом циклических подстрок длины 1
    vector<long long> sortArray(alphabet, 0);
    for (long long i = 0; i < currLine.size(); i++) {
        sortArray[currLine[i]]++;
    }
    long long changer = 0;
    long long groupBegin = 0;
    for (long long i = 1; i < sortArray.size(); i++) {
        changer = sortArray[i];
        sortArray[i] = groupBegin;
        groupBegin += changer;
    }
    for (long long i = 0; i < currLine.size(); i++) {
        suffArray[sortArray[currLine[i]]] = i;
        sortArray[currLine[i]]++;
    }

    //подсчёт классов эквивалентности
    long long currClass = 0;
    for (long long i = 1; i < currLine.size(); i++) {
        if (currLine[suffArray[i]] != currLine[suffArray[i - 1]]) {
            currClass++;
        }
        equivClasses[suffArray[i]] = currClass;
    }
    long long numberOfClasses = currClass + 1;

    allEquivClasses.push_back(equivClasses);

    //фаза алгоритма
    vector<long long> currCycleLines(line.size(), 0);
    for (long long i = 1; i < currLine.size(); i *= 2) {

        //сортируем по вторым частям
        vector<long long> temporaryArray = suffArray;
        for (long long j = 0; j < currLine.size(); j++) {
            temporaryArray[j] -= i;
            if (temporaryArray[j] < 0) {
                temporaryArray[j] += currLine.size();
            }
        }

        //сортировка подсчетом по первым частям
        sortArray.assign(numberOfClasses, 0);
        for (long long j = 0; j < temporaryArray.size(); j++) {
            sortArray[equivClasses[temporaryArray[j]]]++;
        }
        changer = 0;
        groupBegin = 0;
        for (long long j = 0; j < sortArray.size(); j++) {
            changer = sortArray[j];
            sortArray[j] = groupBegin;
            groupBegin += changer;
        }
        for (long long j = 0; j < temporaryArray.size(); j++) {
            suffArray[sortArray[equivClasses[temporaryArray[j]]]] = temporaryArray[j];
            sortArray[equivClasses[temporaryArray[j]]]++;
        }

        //подсчёт классов эквивалентности
        currClass = 0;
        vector<long long> tmpClasses = equivClasses;
        tmpClasses[suffArray[0]] = 0;
        for (long long j = 1; j < suffArray.size(); j++) {
            long long firstPart1 = equivClasses[suffArray[j - 1]];
            long long firstPart2 = equivClasses[suffArray[j]];
            long long secondPart1 = equivClasses[(suffArray[j - 1] + i) % suffArray.size()];
            long long secondPart2 = equivClasses[(suffArray[j] + i) % suffArray.size()];

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

long long SuffArray::calcLcp(long long first, long long second) {
    long long answer = 0;
    long long shift = 1 << (allEquivClasses.size() - 1);
    for (long long j = allEquivClasses.size() - 1; j >= 0; j--) {
        if (allEquivClasses[j][first] == allEquivClasses[j][second]) {
            first += shift;
            second += shift;
            answer += shift;
        }
        shift /= 2;
    }
    return answer;
}

SuffArray::SuffArray(const string &firstLine, const string &secondLine) : line(firstLine + '#' + secondLine + '$') {
    long long lineNumber = 0;
    for (long long i = 0; i < line.size(); i++) {
        affixToLine.push_back(lineNumber);
        if (line[i] == '#') {
            lineNumber = 1;
        }
    }
    buildSuffArray();
}

pair<bool, string> SuffArray::calcCommonSubstring(long long number) {
    long long counter = 0;
    string answer = "";
    long long i = 0;
    while (affixToLine[suffArray[i]] == affixToLine[suffArray[i + 1]]) {
        i++;
    }
    long long minLcp = 0;
    while (i < suffArray.size() - 1 && counter < number) {
        long long currLcp = calcLcp(suffArray[i], suffArray[i + 1]);
        answer = line.substr(suffArray[i], currLcp);
        counter += std::max(static_cast<long long>(0), (currLcp - minLcp));
        if (counter == number) {
            break;
        }
        minLcp = currLcp;
        i++;
        while (i < suffArray.size() - 1 && affixToLine[suffArray[i]] == affixToLine[suffArray[i + 1]]) {
            currLcp = calcLcp(suffArray[i], suffArray[i + 1]);
            if (currLcp < minLcp) {
                minLcp = currLcp;
            }
            i++;
        }
    }

    if (i == suffArray.size() - 1) {
        return make_pair(false, "");
    }
    if (counter > number) {
        while (counter != number) {
            answer.pop_back();
            counter--;
        }
    }
    return make_pair(true, answer);
}
