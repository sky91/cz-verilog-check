#include <iostream>
#include <string>
#include <regex>
#include <numeric>

using std::cin;
using std::cerr;
using std::endl;
using std::string;
using std::regex;
using std::regex_search;
using std::sregex_iterator;
using std::smatch;
using std::stoi;
using std::accumulate;
using std::ios_base;

int popcount(uint32_t i);

int main() {
    string input;
    char buffer[128];
    try {
        while (cin.getline(buffer, sizeof(buffer))) {
            input.append(buffer).append("\n");
        }
    } catch (...) {}

    int tb[256] = {0};
    for (int i = 0; i <= 9; ++i) {
        tb[i + '0'] = popcount(i);
    }
    for (int i = 0; i <= 5; ++i) {
        tb[i + 'a'] = tb[i + 'A'] = popcount(10 + i);
    }

    regex reg{u8R"(\S+\s+#\(\s*(\d+)\s*,\s*\d*'[hH]([0-9a-fA-F]+)\)\s*\w+)"};
    sregex_iterator result_begin(input.begin(), input.end(), reg);
    sregex_iterator result_end;
    bool has_error = false;

    for (sregex_iterator i = result_begin; i != result_end; i++) {
        uint32_t n = stoi((*i)[1]);
        string bs = (*i)[2];
        if (accumulate(bs.begin(), bs.end(), 0, [&tb](int sum, char c) { return sum + tb[c]; }) != n) {
            cerr << (*i).str() << endl;
            has_error = true;
        }
    }

    return has_error;
}


int popcount(uint32_t i) {
    i = i - ((i >> 1u) & 0x55555555u);
    i = (i & 0x33333333u) + ((i >> 2u) & 0x33333333u);
    return (((i + (i >> 4u)) & 0x0F0F0F0Fu) * 0x01010101u) >> 24u;
}