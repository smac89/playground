#include <iostream>
#include <algorithm>
#include <iterator>
#include <tuple>
#include <set>
#include <vector>
#include <utility>
#include <iomanip>
#include <chrono>
// #include <bits/stdc++.h>

using ull = unsigned long long;
constexpr double CACHE_SIZE_FACTOR = 1.0 / (10 * 10 * 10); // size of the range between each cache

class LogEntry {
public:
    LogEntry () : endtime(0), starttime(0), bitrate(0), streamed(0), prev_streamed(0), duration(0) {}
    ull endtime, starttime;
    unsigned int bitrate;
    double streamed, prev_streamed, duration;
};

using LogEntries = std::vector<LogEntry>;
using LogCache = std::vector<std::pair<const std::size_t, const std::size_t>>;

LogEntries reorder(const LogEntries&, LogCache&);
void repl(const LogEntries&, const LogCache&, int);
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

    LogCache cache;
    logEntries = reorder(logEntries, cache);

    #ifdef DEBUG
    // std::cout << "\n" << logEntries << "\n\n";

    // std::cout << "\n" << cache << "\n\n";
    std::cout << "Entries size: " << logEntries.size() << "\nCache size: " << cache.size() << '\n';
    std::cout << "Cache contents:\n" << cache << '\n';
    #endif

    if (std::cin >> logsize) {
        repl(logEntries, cache, logsize);
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
void repl(const LogEntries& entries, const LogCache& cache, int count) {
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

    for (std::size_t t = 0; t < entries.size(); t++) {
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

/**
 * @brief Aranges the logs into non overlapping ranges for easier usage
 * @details The logs are taken one by one and we cut them up into smaller
 * logs which do not overlap. By doing this, it is easier to work with ranges
 * because you don't have to worry about anything they overlap
 * 
 * @param entries The initial entries we got as input
 */
LogEntries reorder(const LogEntries &entries, LogCache& cache) {
    LogEntries ranges = extract_ranges(entries);
    LogEntries::iterator hi, lo;

    // compute the cache size (atleast size 1)
    std::size_t num_buckets = static_cast<double>(ranges.size()) * CACHE_SIZE_FACTOR + 1;
    cache.reserve(num_buckets);

    // The max time interval maintained within each range in the cache
    const ull max_interval = (ranges.back().endtime - ranges[0].starttime + num_buckets) / num_buckets;

    #ifdef DEBUG
    std::cout << "Cache size factor: " << CACHE_SIZE_FACTOR << '\n'
    << "Number of buckets: " << num_buckets << '\n'
    << "Max interval: " << max_interval << '\n';
    #endif

    for (auto &entry : entries) {
        std::tie(lo, hi) = std::equal_range(ranges.begin(), ranges.end(), 
            entry, compare);

        std::for_each(lo, hi, [&entry](LogEntry& val) {
            val.bitrate += entry.bitrate;
        });
    }

    LogEntries::iterator beg = ranges.begin(), curr;
    beg->streamed = beg->duration / 1000 * beg->bitrate;
    ull start_time = beg->starttime;

    // auto start = std::chrono::steady_clock::now();

    for (curr = beg + 1; curr != ranges.end(); curr++) {
        curr->streamed = curr->duration / 1000 * curr->bitrate;
        curr->prev_streamed = (curr - 1)->streamed + (curr - 1)->prev_streamed;

        if (curr->starttime - start_time > max_interval) {
            cache.emplace_back(std::distance(ranges.begin(), beg), std::distance(ranges.begin(), curr));
            beg = curr;
            start_time = curr->starttime;
        }
    }
    cache.emplace_back(std::distance(ranges.begin(), beg), std::distance(ranges.begin(), curr));
    // std::cout << "This took: "
    // << std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now() - start).count()
    // << " seconds\n";

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
