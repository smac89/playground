#include <unordered_map>
#include <forward_list>
#include <string>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <locale>

using inv_map = std::unordered_map<std::string, std::forward_list<std::string>>;

void gobble(std::string fname, inv_map &lib);
void repl(const inv_map &dict);
std::string stripPunc(std::string str);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "\n*** Usage: " << argv[0] << "file1 file2 ... ***\n";
        return 1;
    }
    inv_map dict;

    for (int c = 1; c < argc; c++) {
        gobble(argv[c], dict);
    }

    repl(dict);
    return 0;
}

void repl(const inv_map &dict) {
    std::istringstream stream;
    std::cout << "\n[repl]: ";

    for (std::string line; std::getline(std::cin, line); std::cout << std::endl << "[repl]: ") {
        stream.str(line);
        stream.clear();
        std::for_each(std::istream_iterator<std::string>(stream),
                      std::istream_iterator<std::string>{},
                      [&](const std::string &token) {

                        inv_map::const_iterator pos = dict.find(token);
                        if (pos != dict.end()) {
                            const std::forward_list<std::string> &set = pos->second;
                            std::cout << ">> (" << token << ") is found in {";
                            std::copy(set.begin(),
                                      set.end(),
                                      std::ostream_iterator<std::string>(std::cout, ","));
                            
                            std::cout << "}\n";
                        }else std::cout << "(" << token << "): not found in any files\n";

        });
    }
}

void gobble(std::string fname, inv_map &lib) {
    if (FILE *fp = std::fopen(fname.c_str(), "r")) {
        char buf[1024];
        while (std::size_t len = std::fread(buf, sizeof (char), sizeof buf, fp)) {
            std::istringstream str(std::string(buf, len));
            for(std::string word; str >> word; ) {
                std::forward_list<std::string> &files = lib[word];
                if (files.empty() || (!files.empty() && files.front() != fname)) {
                    files.emplace_front(fname);
                }
            }
        }
        fclose(fp);
    }
}

std::string stripPunc(std::string str) {
    static std::locale locl("");

    std::size_t p = 0;
    while (p < str.length() && std::ispunct(str[p], locl)) {
        p++;
    }
    str = str.substr(p);

    p = str.length() - 1;
    while (p >= 0 && std::ispunct(str[p], locl)) {
        p--;
    }
    str = str.substr(0, p + 1);

    return str;
}