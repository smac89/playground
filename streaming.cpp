#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <iomanip>
#include <map>
#include <utility>
#include <unordered_set>

using ull = unsigned long long;

struct LogEntry {
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

    logEntries = std::move(reorder(logEntries));

    #ifdef DEBUG
    std::cout << "Count: " << logEntries.size() << '\n';
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
        #ifdef DEBUG
        std::cout << query << std::endl << *lo << std::endl << *hi << std::endl;
        #endif

        if (query.starttime < lo->starttime) {
            query.starttime = lo->starttime;
        }

        if (query.endtime > hi->endtime) {
            query.endtime = hi->endtime;
        }

        if (hi != lo) {
            query.streamed = (hi->prev_streamed - lo->prev_streamed - lo->streamed) +
            ((lo->endtime - query.starttime) / lo->duration * lo->streamed) +
            ((query.endtime - hi->starttime) / hi->duration * hi->streamed);
        } else {
            query.duration = query.endtime - query.starttime;
            query.streamed = query.duration / 1000 * lo->bitrate;
        }
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
    using index_set = std::unordered_set<std::size_t>;

    std::map<ull, index_set> rng_to_entries;
    std::map<ull, index_set>::const_iterator iter;

    for (std::size_t v = 0; v < entries.size(); v++) {
        rng_to_entries[entries[v].starttime].insert(v);
        rng_to_entries[entries[v].endtime].insert(v);
    }

    LogEntries merged;
    index_set main_set, var_set;

    unsigned int curr_bitrate = 0;

    for (iter = rng_to_entries.begin(); ++iter != rng_to_entries.end(); ) {
        --iter;
        LogEntry range = {};
        std::tie(range.starttime, var_set) = *iter++;
        range.endtime = iter->first;
        range.duration = range.endtime - range.starttime;

        // determine all entries that overlap this range
        for (auto &v : var_set) {
            if (main_set.erase(v) == 0) {
                curr_bitrate += entries[v].bitrate;
                main_set.insert(v);
            } else {
                curr_bitrate -= entries[v].bitrate;
            }
        }

        // set the bitrate of this range to be an aggregate of bitrate of the overlaps
        range.bitrate = curr_bitrate;
        
        // calculate the amount streamed
        range.streamed = range.duration / 1000 * range.bitrate;

        if (!merged.empty()) {
            range.prev_streamed = merged.back().streamed + merged.back().prev_streamed;
        }
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

template <typename T>
std::ostream &operator << (std::ostream& oss, const std::vector<T>& v) {
    for (auto& item : v) {
        oss << item << "\n";
    }
    return oss;
}
