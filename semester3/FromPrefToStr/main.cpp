/*Задача 2. Преобразования (6 баллов)
 B1, B2. Реализовать набор преобразований строки в префикс-функцию, z-функцию и обратно,
 а также из префикс-функции в z-функцию и обратно (6 шт).
 Все преобразования должны работать за линейное время (размер алфавита считаем константой).
 Преобразование в строку должно выдавать лексикографически минимально возможную строку.
 Задача в контесте B1: Найти лексикографически-минимальную строку, построенную по префикс-функции,
 в алфавите a-z*/

#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::string;
using std::vector;

//функция построения z-функции по строке
vector<int> convertFromTextToZ (const string &text) {
    vector<int> zFunc;
    zFunc.assign(text.size(), 0);
    
    if (text.empty()) {
        return zFunc;
    }
    
    int leftPosition = 0;
    
    int rightPosition = 0;
    for (int i = 1; i < text.size(); i++) {
        if (i > rightPosition) {
            int j = 0;
            while (i + j < text.size() && text[j] == text[i + j]) {
                j++;
            }
            zFunc[i] = j;
        }
        else {
            if (zFunc[i - leftPosition] + i > rightPosition) {
                int j = rightPosition - i;;
                while (rightPosition + j < text.size() && text[j] == text[rightPosition + j]) {
                    j++;
                }
                zFunc[i] = j;
            }
            else {
                zFunc[i] = zFunc[i - leftPosition];
            }
        }
        if (rightPosition < i + zFunc[i]) {
            rightPosition = i + zFunc[i];
            leftPosition = i;
        }
    }
    return zFunc;
}

//функция построения префикс-функции по z-функции
vector<int> convertFromZToPrefix (const vector<int> &zFunc) {
    vector<int> prefixes(zFunc.size(), 0);
    
    if (prefixes.empty()){
        return prefixes;
    }
    
    prefixes[0] = 0;
    for (int i = 1; i < zFunc.size(); i++) {
        for (int j = zFunc[i] - 1; j >= 0; j--) {
            if (prefixes[i + j] == 0) {
                prefixes[i + j] = j + 1;
            }
            else {
                break;
            }
        }
    }
    return prefixes;
}

//функция построения минимальной строки по префикс-функции
string buildMinString(const vector<int> &prefixes) {
    string answer;
    if (!prefixes.empty()) {
        const char minLetter = 'a';
        answer += minLetter;
        for (int i = 1; i < prefixes.size(); i++) {
            if (prefixes[i] == 0) {
                int prevPrefix = prefixes[i - 1];
                char maxLetter;
                maxLetter = answer[prevPrefix] + 1;
                while (prevPrefix > 0) {
                    if (maxLetter < answer[prevPrefix] + 1) {
                        maxLetter = answer[prevPrefix] + 1;
                    }
                    prevPrefix = prefixes[prevPrefix - 1];
                }
                answer += maxLetter;
            } else {
                answer += answer[prefixes[i] - 1];
            }
        }
    }
    return answer;
}

int main() {
    vector<int> prefixes;
    vector<int> zFunc;
    int zFuncElem = 0;
    while(cin >> zFuncElem) {
        zFunc.push_back(zFuncElem);
    }
    string answer;
    prefixes = convertFromZToPrefix(zFunc);
    answer = buildMinString(prefixes);
    for (auto letter : answer) {
        cout << letter;
    }
    return 0;
}
