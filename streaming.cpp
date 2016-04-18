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
    LogEntry () 
    : endtime(0), starttime(0), bitrate(0), prev_bitrate(0), duration(0), streamed(0) {}

    ull endtime, starttime;
    unsigned int bitrate, prev_bitrate;
    double streamed, duration;
};

using LogSet = std::set<LogEntry, bool (*) (const LogEntry&, const LogEntry&)>;
using LogEntries = std::vector<LogEntry>;

LogEntries reorder(const LogEntries&);
LogEntries extract_ranges(const LogEntries&);
void repl(const LogEntries&, int);
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

    logEntries = reorder(logEntries);

    #ifdef DEBUG
    std::cout << "\n";
    for (auto& ent : logEntries) {
        std::cout << ent << '\n';
    }
    std::cout << "\n\n";
    #endif

    std::cin >> logsize;
    repl(logEntries, logsize);

    return 0;
}

void repl(const LogEntries& entries, int count) {
    for (int r = 0; r < count; r++) {

    }
}

/**
 * @brief Aranges the logs into non overlapping ranges for easier usage
 * @details The logs are taken one by one and we cut them up into smaller
 * logs which do not overlap. By doing this, it is easier to work with ranges
 * because you don't have to worry about anything they overlap
 * 
 * @param entries The initial entries we got as input
 */
LogEntries reorder(const LogEntries &entries) {
    LogEntries ranges = extract_ranges(entries);
    LogEntries::iterator hi, lo;

    // Comparator for ranges
    auto compare = [](const LogEntry& lhs, const LogEntry& rhs) {
        return lhs.endtime <= rhs.starttime;
    };

    for (auto &entry : entries) {
        std::tie(lo, hi) = std::equal_range(ranges.begin(), ranges.end(), 
            entry, compare);

        std::for_each(lo, hi, [&](LogEntry& val) {
            val.bitrate += entry.bitrate;
        });
    }

    ranges[0].streamed = ranges[0].duration / 1000 * ranges[0].bitrate;

    for (int t = 1; t < ranges.size(); t++) {
        ranges[t].streamed = ranges[t].duration / 1000 * ranges[t].bitrate;
        ranges[t].prev_bitrate += ranges[t - 1].bitrate;
    }

    return ranges;
}

/**
 * @brief Extract the non-overlapping ranges from the given log entries
 * @details The method ...
 * 
 * @param entries The vector of log entries from which to extract non
 * overlapping ranges from
 * @return A vector of Log Entries where all the entries are non overlapping
 */
LogEntries extract_ranges(const LogEntries& entries) {
    // Use a set to avoid duplicates and make the entries sorted 2 birds one set
    std::set<ull> times;
    std::set<ull>::const_iterator iter;

    for (int t = 0; t < entries.size(); t++) {
        times.insert(entries[t].starttime);
        times.insert(entries[t].endtime);
    }

    LogEntries merged;
    LogEntry range;

    for (iter = times.begin(); iter != times.end();) {
        range.starttime = *iter++;
        if (iter != times.end()) {
            range.endtime = *iter;
            range.duration = range.endtime - range.starttime;
            merged.push_back(range);
        }
    }
    return merged;
}

std::istream &operator >> (std::istream& iss, LogEntry& entry) {
    iss >> entry.endtime >> entry.duration >> entry.bitrate;
    entry.starttime = entry.endtime - entry.duration;
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
