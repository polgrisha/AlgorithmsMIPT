#include <fstream>
#include <iostream>
#include <vector>

using std::cout;
using std::ifstream;
using std::ofstream;
using std::string;
using std::vector;

//построение префикс-функции для паттерна
vector<int> prefixFuncForPattern (const string &text) {
    vector<int> prefixes(text.size(), 0);
    if (prefixes.empty() {
        return prefixes;
    }
    prefixes[0] = 0;
    for (int i = 1; i < text.length(); i++) {
        if (text[i] == text[prefixes[i - 1]]) {
            prefixes[i] = prefixes[i - 1] + 1;
        }
        else {
            int prevPrefix = prefixes[i - 1];
            while (prevPrefix > 0 && text[i] != text[prevPrefix]) {
                prevPrefix = prefixes[prevPrefix - 1];
            }
            if (text[i] == text[prevPrefix]) {
                prefixes[i] = prevPrefix + 1;
            } else {
                prefixes[i] = 0;
            }
        }
    }
    return prefixes;
}

int main() {
    ifstream in("input.txt");
    ofstream out("output.txt");
    string pattern;
    in >> pattern;

    vector<int> prefixes;
    int patternSize = pattern.size();
    pattern += '#';
    prefixes = prefixFuncForPattern(pattern);

    int prevPrefix = prefixes[prefixes.size() - 1];
    int currPrefix = 0;
    int number = 1;
    char symbol;
    //подсчёт префикс функции для текущего символа текста
    while (in >> symbol) {
        if (symbol == pattern[prevPrefix]) {
            currPrefix = prevPrefix + 1;
        }
        else {
            while (prevPrefix > 0 && symbol != pattern[prevPrefix]) {
                prevPrefix = prefixes[prevPrefix - 1];
            }
            if (symbol == pattern[prevPrefix]) {
                currPrefix = prevPrefix + 1;
            } else {
                currPrefix = 0;
            }
        }
        if (currPrefix == patternSize) {
            out << number - patternSize << " ";
        }
        prevPrefix = currPrefix;
        number++;
    }
    return 0;
}