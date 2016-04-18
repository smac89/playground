#include <iostream>
#include <algorithm>
#include <iterator>
#include <tuple>
#include <set>
#include <vector>
#include <utility>
#include <iomanip>
#include <type_traits>

using ull = unsigned long long;

class LogEntry {
public:
    unsigned long long endtime, starttime;
    unsigned int bitrate;
    double streamed, duration;

    explicit LogEntry() = default;

    template <typename T1, typename T2,
    typename = std::enable_if<std::is_integral<T1>::value && std::is_integral<T2>::value>>
    LogEntry(const std::pair<T1, T2> &val) {
        this->starttime = val.first;
        this->endtime = val.second;
        this->bitrate = this->streamed = this->duration = 0;
    }
};

using LogSet = std::set<LogEntry, bool (*) (const LogEntry&, const LogEntry&)>;
using LogEntries = std::vector<LogEntry>;

void reorder(LogEntries&);
std::vector<std::pair<ull, ull>> extract_ranges(LogEntries&);
std::istream &operator >> (std::istream&, LogEntry&);

#ifdef DEBUG
std::ostream &operator << (std::ostream&, const LogEntry&);

template <typename F, typename S>
std::ostream &operator << (std::ostream&, const std::pair<F, S>&);
#endif

int main() {

    LogEntries logEntries;
    int logsize;
    std::cin >> logsize;

    std::copy_n(std::istream_iterator<LogEntry>(std::cin), logsize,
        std::back_inserter(logEntries));

    std::cout << "\n";
    for (auto& ent : logEntries) {
        std::cout << ent << '\n';
    }
    std::cout << "\n\n";

    reorder(logEntries);
    return 0;
}

void reorder(LogEntries &entries) {
    LogSet set(order_by_start);

    std::vector<std::pair<ull, ull>> ranges = extract_ranges(entries);
    std::vector<std::pair<ull, ull>>::iterator hi, lo;

    auto compare = [](const LogEntry& lhs, const LogEntry& rhs) {
        return lhs.endtime <= rhs.starttime;
    };

    for (auto &entry : entries) {
        std::tie(lo, hi) = std::equal_range(ranges.begin(), ranges.end(), 
            entry, compare);

        std::cout << entry << "\n";
        // std::cout << *lo << "\n";
        std::for_each(lo, hi,
            [](const std::pair<ull, ull>& val) {
                std::cout << val << '\n';
            });
        // std::copy(lo, hi,
        //     std::ostream_iterator<std::pair<ull, ull>>(std::cout, "\n"));
        std::cout << "\n\n";
    }
}

/**
 * @brief Extract the non-overlapping ranges from the given log entries
 * @details The method ...
 * 
 * @param entries The vector of log entries from which to extract non
 * overlapping ranges from
 * @return A vector of pairs where each pair describes a non overlapping range
 * found in the entries in the form of (start, end) where start > end
 */
std::vector<std::pair<ull, ull>> extract_ranges(LogEntries& entries) {
    // Use a set to avoid duplicates and make the entries sorted
    std::set<ull> times;
    std::set<ull>::const_iterator iter;

    for (int t = 0; t < entries.size(); t++) {
        times.insert(entries[t].starttime);
        times.insert(entries[t].endtime);
    }

    std::vector<std::pair<ull, ull>> merged;
    std::pair<ull, ull> range;

    for (iter = times.begin(); iter != times.end();) {
        range.first = *iter++;
        if (iter != times.end()) {
            range.second = *iter;
            merged.push_back(range);
        }
    }
    return merged;
}

std::istream &operator >> (std::istream& iss, LogEntry& entry) {
    iss >> entry.endtime >> entry.duration >> entry.bitrate;
    entry.starttime = entry.endtime - entry.duration;
    entry.streamed = entry.duration / 1000 * entry.bitrate;
    return iss;
}

std::ostream &operator << (std::ostream& oss, const LogEntry& entry) {
    oss << "start=" << entry.starttime << ", end=" << entry.endtime
    << ", bitrate(s)=" << entry.bitrate << ", streamed(kb)="
    << std::setprecision(3) << std::fixed << entry.streamed;

    return oss;
}

template <typename F, typename S>
std::ostream &operator << (std::ostream& oss, const std::pair<F, S> &p) {
    return oss << "(First=>" << p.first << ", second=>" << p.second << ")";
}
