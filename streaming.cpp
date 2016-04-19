#include <iostream>
#include <algorithm>
#include <iterator>
#include <tuple>
#include <set>
#include <vector>
#include <utility>
#include <iomanip>
#include <limits>
// #include <bits/stdc++.h>

using ull = unsigned long long;

// 100 seconds of information cached
constexpr std::size_t STORED_DURATION = 100 * 1000;

class LogEntry {
public:
    LogEntry () : endtime(0), starttime(0), bitrate(0), streamed(0), prev_streamed(0), duration(0) {}
    ull endtime, starttime;
    unsigned int bitrate;
    double streamed, prev_streamed, duration;
};

using LogEntries = std::vector<LogEntry>;

LogEntries reorder(const LogEntries&);
void repl(const LogEntries&, int);
bool compare(const LogEntry&, const LogEntry&);
std::istream &operator >> (std::istream&, LogEntry&);

#ifdef DEBUG
template <typename T>
std::ostream &operator << (std::ostream&, const std::vector<T>&);

std::ostream &operator << (std::ostream&, const LogEntry&);

template <typename F, typename S>
std::ostream &operator << (std::ostream&, const std::pair<F, S>&);
#endif

int main() {

    // Remove synchronization between cstdio and iostream
    std::ios_base::sync_with_stdio(false);
    // Prevent flushing of output stream before each io operation
    std::cin.tie(nullptr);

    LogEntries logEntries;
    int logsize;
    std::cin >> logsize;

    std::copy_n(std::istream_iterator<LogEntry>(std::cin), logsize,
        std::back_inserter(logEntries));

    #ifdef DEBUG
    // std::cout << "\n" << logEntries << "\n\n";
    #endif

    logEntries = reorder(logEntries);

    #ifdef DEBUG
    std::cout << "\n" << logEntries << "\n\n";

    // std::cout << "\n" << cache << "\n\n";
    // std::cout << "Entries size: " << logEntries.size() << '\n';
    #endif

    if (std::cin >> logsize) {
        repl(logEntries, logsize);
    }

    return 0;
}

/**
 * @brief Reads time ranges and sells lambourghinis'
 * @details The repl stands for READ-EVALUATE-PRINT-LOOP.
 * This method in particular will read any time range given, determine which log entries
 * fall under this time range. It will then take the streams available under this
 * time range and print what percentage of those streams were fully contained
 * within the range given
 * 
 * @param entries The collection of logs. This must have gone through the reorder
 * function before it can be used here
 * @param count The number of queries to expect
 */
void repl(const LogEntries& entries, int count) {
    LogEntries::const_iterator hi, lo;
    LogEntry query;
    std::cout.precision(3);
    std::cout.setf(std::ios_base::fixed);

    for (std::cin >> query.starttime >> query.endtime; count--; 
        std::cin >> query.starttime >> query.endtime) {

        std::tie(lo, hi) = std::equal_range(entries.begin(), entries.end(),
            query, compare);

        hi--;
        std::cout << query << std::endl << *lo << std::endl << *hi << std::endl;

        if (query.starttime < lo->starttime) {
            query.starttime = lo->starttime;
        }

        if (query.endtime > hi->endtime) {
            query.endtime = hi->endtime;
        }

        query.streamed = (hi->prev_streamed - lo->prev_streamed - lo->streamed) +
        ((lo->endtime - query.starttime) / lo->duration * lo->streamed) +
        ((query.endtime - hi->starttime) / hi->duration * hi->streamed);

        std::cout << query.streamed << "\n\n";
    }
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
    ull smaller = std::numeric_limits<ull>::max(), larger = 0;

    for (auto& entry: entries) {
        if (entry.starttime < smaller) {
            smaller = entry.starttime;
        }

        if (entry.endtime > larger) {
            larger = entry.endtime;
        }
    }

    LogEntries merged;
    LogEntry range;

    #ifdef DEBUG
    std::cout << "Min: " << smaller << "\nMax: " << larger << '\n';
    #endif

    while (smaller + STORED_DURATION < larger) {
        range.starttime = smaller;
        range.endtime = smaller + STORED_DURATION;
        range.duration = range.endtime - range.starttime;
        merged.push_back(range);
        smaller += STORED_DURATION;
    }
    range.starttime = smaller;
    range.endtime = larger;
    range.duration = range.endtime - range.starttime;
    merged.push_back(range);

    return merged;
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

    for (auto &entry : entries) {
        std::tie(lo, hi) = std::equal_range(ranges.begin(), ranges.end(), 
            entry, compare);

        std::for_each(lo, hi, [&entry](LogEntry& val) {
            // if (val.endtime > entry.starttime && val.starttime < entry.endtime) {
            if (val.starttime < entry.starttime && val.endtime > entry.endtime) {
                 val.streamed += (entry.endtime - entry.starttime) / 1000 * entry.bitrate;
            } else if (val.starttime < entry.starttime) {
                val.streamed += (val.endtime - entry.starttime) / 1000 * entry.bitrate;
            } else if (val.endtime > entry.endtime) {
                val.streamed += (entry.endtime - val.starttime) / 1000 * entry.bitrate;
            } else {
                val.streamed += (val.endtime - val.starttime) / 1000 * entry.bitrate;
            }
            // }
        });
    }

    LogEntries::iterator curr = ranges.begin();
    // curr->streamed = curr->duration / 1000 * curr->bitrate;

    while (++curr != ranges.end()) {
        // curr->streamed = curr->duration / 1000 * curr->bitrate;
        curr->prev_streamed = (curr - 1)->streamed + (curr - 1)->prev_streamed;
    }
    return ranges;
}

inline bool compare(const LogEntry& lhs, const LogEntry& rhs) {
    return lhs.endtime <= rhs.starttime;
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

template <typename T>
std::ostream &operator << (std::ostream& oss, const std::vector<T>& v) {
    for (auto& item : v) {
        oss << item << "\n";
    }
    return oss;
}
