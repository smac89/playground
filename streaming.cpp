#include <iostream>
#include <algorithm>
#include <iterator>
#include <set>
#include <vector>
#include <iomanip>

using ull = unsigned long long;

struct LogEntry {
    ull endtime, starttime;
    unsigned int bitrate;
    double streamed, prev_streamed, duration;
};

using LogEntries = std::vector<LogEntry>;

LogEntries reorder(const LogEntries&);
void repl(LogEntries&, int);
bool compare(const LogEntry&, const LogEntry&);
std::istream &operator >> (std::istream&, LogEntry&);

#ifdef DEBUG
std::ostream &operator << (std::ostream&, const LogEntry&);

template <typename F, typename S>
std::ostream &operator << (std::ostream&, const std::pair<F, S>&);
#endif

int main() {

    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    LogEntries logEntries;
    int logsize;
    std::cin >> logsize;

    std::copy_n(std::istream_iterator<LogEntry>(std::cin), logsize,
        std::back_inserter(logEntries));

    #ifdef DEBUG
    std::cout << "\n";
    for (auto& ent : logEntries) {
        std::cout << ent << '\n';
    }
    std::cout << "\n\n";
    #endif

    logEntries = reorder(logEntries);

    #ifdef DEBUG
    // std::cout << "Count: " << logEntries.size() << '\n';
    for (auto& ent : logEntries) {
        std::cout << ent << '\n';
    }
    std::cout << "\n\n";
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
void repl(LogEntries& entries, int count) {
    LogEntries::const_iterator hi, lo;
    LogEntry query;
    std::cout.precision(3);
    std::cout.setf(std::ios_base::fixed);

    for (std::cin >> query.starttime >> query.endtime; count--; 
        std::cin >> query.starttime >> query.endtime) {
        std::tie(lo, hi) = std::equal_range(entries.begin(), entries.end(),
            query, compare);

        hi--;

        if (query.starttime < lo->starttime) {
            query.starttime = lo->starttime;
        }

        if (query.endtime > hi->endtime) {
            query.endtime = hi->endtime;
        }

        query.streamed = (hi->prev_streamed - lo->prev_streamed - lo->streamed) +
        ((lo->endtime - query.starttime) / lo->duration * lo->streamed) +
        ((query.endtime - hi->starttime) / hi->duration * hi->streamed);

        std::cout << query.streamed << '\n';
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
    LogEntries extract_ranges(const LogEntries&);

    LogEntries ranges = extract_ranges(entries);
    LogEntries::iterator hi, lo;

    for (auto &entry : entries) {
        std::tie(lo, hi) = std::equal_range(ranges.begin(), ranges.end(), 
            entry, compare);

        std::for_each(lo, hi, [&entry](LogEntry& val) {
            val.bitrate += entry.bitrate;
        });
    }

    lo = ranges.begin();
    hi = ranges.end();

    lo->streamed = lo->duration / 1000 * lo->bitrate;

    while (++lo != hi) {
        lo->streamed = lo->duration / 1000 * lo->bitrate;
        lo->prev_streamed = (lo - 1)->prev_streamed + (lo - 1)->streamed;
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

    for (auto& entry : entries) {
        times.insert(entry.starttime);
        times.insert(entry.endtime);
    }

    LogEntries merged;
    LogEntry range = {};

    for (iter = times.begin(); ++iter != times.end(); ) {
        --iter;
        range.starttime = *iter++;
        range.endtime = *iter;
        range.duration = range.endtime - range.starttime;
        merged.push_back(range);
    }
    return merged;
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
