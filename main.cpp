#include <iostream>
#include <string>
#include <regex>
#include <numeric>
#include <exception>
#include <set>

using namespace std;

struct Options {
    Options(int argc, char* argv[]);

    bool should_check_add(const string& add) const;

    bool check_all;
    set<string> check_adds;
};

int popcount(uint32_t i);
void print_help();
bool do_check(const string& input, const Options& options);

const char* OPTION_ALL = "--all";
const char* OPTION_ADD = "--add";

int main(int argc, char* argv[]) {
    Options options{argc, argv};

    string input;
    char buffer[128];
    try {
        while (cin.getline(buffer, sizeof(buffer))) {
            input.append(buffer).append(" ");
        }
    } catch (...) {}

    return !do_check(input, options);
}

bool do_check(const string& input, const Options& options) {
    int tb[256] = {0};
    for (int i = 0; i <= 9; ++i) {
        tb[i + '0'] = popcount(i);
    }
    for (int i = 0; i <= 5; ++i) {
        tb[i + 'a'] = tb[i + 'A'] = popcount(10 + i);
    }

    regex reg{u8R"(\S+\s+#\(\s*(\d+)\s*,\s*\d*'[hH]([0-9a-fA-F]+)\)\s*(\w+))"};
    sregex_iterator result_begin(input.begin(), input.end(), reg);
    const sregex_iterator result_end;
    bool successful = true;

    for (sregex_iterator i = result_begin; i != result_end; i++) {
        uint32_t n = stoi((*i)[1]);
        string bs{(*i)[2]};
        string add{(*i)[3]};
        if (!options.check_all && !options.should_check_add(add)) {
            continue;
        }
        string line{i->str()};
        if (accumulate(bs.begin(), bs.end(), 0, [&tb](int sum, char c) { return sum + tb[c]; }) != n) {
            cout << "[FAIL]: " << line << endl;
            cerr << line << endl;
            successful = false;
        } else {
            cout << "[OK  ]: " << line << endl;
        }
    }

    return successful;
}


int popcount(uint32_t i) {
    i = i - ((i >> 1u) & 0x55555555u);
    i = (i & 0x33333333u) + ((i >> 2u) & 0x33333333u);
    return (((i + (i >> 4u)) & 0x0F0F0F0Fu) * 0x01010101u) >> 24u;
}

void print_help() {
    cout <<
         R"(cz-verilog-check [--all] [--add <address>]...
    example:
    cz-verilog-check --add H32
)";
}

Options::Options(int argc, char** argv) {
    if (argc <= 1) {
        print_help();
        throw invalid_argument("arg count error");
    }
    for (int i = 1; i < argc; ++i) {
        string opt{argv[i]};
        if (opt == OPTION_ALL) {
            check_all = true;
        } else if (opt == OPTION_ADD) {
            check_adds.emplace(argv[++i]);
        }
    }
}

bool Options::should_check_add(const string& add) const {
    for (const string& a :check_adds) {
        if (add.size() >= a.size() && add.compare(add.size() - a.size(), add.size(), a) == 0) {
            return true;
        }
    }
    return false;
}

