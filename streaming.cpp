#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <iomanip>
#include <map>
#include <cmath>

struct LogEntry {
    long long time;
    int bitrate;
    double total_streamed;

    operator long long () const { return this->time; }
};

constexpr double MS_PER_SEC = 10 * 10 * 10;

using LogEntries = std::vector<LogEntry>;

void repl(const LogEntries&, std::size_t);
void read_entries(LogEntries&, std::size_t);
LogEntries reorder(LogEntries&);


#ifdef DEBUG
template <typename T>
std::ostream &operator << (std::ostream&, const std::vector<T>&);

std::ostream &operator << (std::ostream&, const LogEntry&);
#endif

int main() {

    // Remove synchronization between cstdio and iostream
    std::ios_base::sync_with_stdio(false);
    // Prevent flushing of output stream before each io operation
    std::cin.tie(nullptr);

    LogEntries logEntries;
    std::size_t logsize;
    std::cin >> logsize;

    read_entries(logEntries, logsize);

    #ifdef DEBUG
    std::cout << "\n" << logEntries << "\n\n";
    #endif

    logEntries = reorder(logEntries);

    #ifdef DEBUG
    std::cout << "Count: " << logEntries.size() << '\n';
    std::cout << logEntries << "\n\n";
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
void repl(const LogEntries& entries, std::size_t count) {
    LogEntries::const_iterator hi, lo;
    long long start, end;
    double streamed;

    std::cout.precision(3);
    std::cout.setf(std::ios_base::fixed);

    for (std::cin >> start >> end; count--; std::cin >> start >> end) {

        lo = std::lower_bound(entries.begin(), entries.end(), start);
        hi = std::upper_bound(lo, entries.end(), end);

        if (hi == entries.end()) {
            end = (hi - 1)->time;
        }

        #ifdef DEBUG
        std::cout << lo->time << " <= " << start << "\n"
        << (hi == entries.end() ? hi - 1 : hi)->time << " >= " << end << std::endl;
        #endif
 
        streamed = (lo->time - start) / MS_PER_SEC * (lo == entries.begin() ? 0 : (lo - 1)->bitrate)
        + (hi - 1)->total_streamed - lo->total_streamed +
        (end - (hi - 1)->time) / MS_PER_SEC * (hi - 1)->bitrate;

        std::cout << streamed << '\n';
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
LogEntries reorder(LogEntries &entries) {
    std::sort(entries.begin(), entries.end(), 
        [](const LogEntry& lhs, const LogEntry& rhs) {
            return lhs.time < rhs.time;
        });

    LogEntries merged;

    int bits = 0;
    LogEntries::iterator iter;
    LogEntry* prev = nullptr;

    for (iter = entries.begin(); iter != entries.end(); ++iter) {
        bits += iter->bitrate;

        if (prev == nullptr) {
            prev = &(*iter);
            continue;
        }

        if (iter->time == prev->time) {
            prev->bitrate = bits;
            continue;
        }

        iter->total_streamed = prev->total_streamed + (iter->time - prev->time) / MS_PER_SEC * prev->bitrate;
        iter->bitrate = bits;
        merged.push_back(*prev);
        prev = &(*iter);
    }
    prev->bitrate = bits;
    merged.push_back(*prev);

    return merged;
}

void read_entries(LogEntries& vec, std::size_t num) {
    vec.reserve(num << 1);
    long long tm;
    int bit, dur;

    while (num--) {
        std::cin >> tm >> dur >> bit;
        vec.push_back({tm, -bit, 0});
        vec.push_back({tm - dur, bit, 0});
    }
}

std::ostream &operator << (std::ostream& oss, const LogEntry& entry) {
    return oss << "start=" << entry.time << ", bitrate(s)=" << entry.bitrate 
    << ", streamed(kb)=" << std::setprecision(3) << std::fixed << entry.total_streamed;
}

template <typename T>
std::ostream &operator << (std::ostream& oss, const std::vector<T>& v) {
    for (auto& item : v) {
        oss << item << "\n";
    }
    return oss;
}
