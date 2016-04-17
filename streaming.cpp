#include <iostream>
#include <algorithm>
#include <iterator>
#include <list>
#include <set>
#include <vector>
#include <utility>
#include <iomanip>

using ull = unsigned long long;

struct LogEntry {
    unsigned long long endtime, starttime;
    unsigned int bitrate;
    double streamed, duration;
};

using LogSet = std::set<LogEntry, bool (*) (const LogEntry&, const LogEntry&)>;
using LogEntries = std::vector<LogEntry>;
using LSize = LogEntries::size_type;

void reorder(LogEntries&);
void combine_range(LogEntries&, LogSet&, LSize, LSize);
std::istream &operator >> (std::istream&, LogEntry&);
inline bool order_by_start (const LogEntry&, const LogEntry&);
inline bool order_by_end (const LogEntry&, const LogEntry&);

#ifdef DEBUG
std::ostream &operator << (std::ostream&, const LogEntry&);

template <typename F, typename S>
std::ostream &operator << (std::ostream&, const std::pair<F, S>&);
#endif


int main() {

    LogEntries logEntries;
    LSize logsize;
    std::cin >> logsize;

    std::copy_n(std::istream_iterator<LogEntry>(std::cin), logsize,
        std::back_inserter(logEntries));

    // sort the entries based on start times
    std::sort(logEntries.begin(), logEntries.end(), order_by_start);

    reorder(logEntries);

    // for (auto& ent : logEntries) {
    //     std::cout << ent << '\n';
    // }
    return 0;
}

void reorder(LogEntries &entries) {
    LogSet set(order_by_start);

    for (LogEntries::iterator iter; entries.size() > 0; ) {
        LSize beg = 0, c = 0, size = entries.size();

        while (entries[c++].starttime == entries[beg].starttime &&
            c < size) {}

        // std::cout << entries.size() << std::endl;
        std::sort(entries.begin(), entries.begin() + c, order_by_end);
        combine_range(entries, set, beg, c);
        iter = entries.begin();
        for (LogEntries::iterator rem = iter + 1; rem < entries.begin() + c; rem++) {
            if (rem->duration > 0) {
                entries.erase(iter, rem);
                break;
            }
        }
        // std::sort(entries.begin(), entries.end(), order_by_start);
    }
}

void combine_range(LogEntries& entries, LogSet& set, LSize beg, LSize end) {
    LSize size = entries.size();
    LogEntry &l = entries[beg];
    if (end < size && l.endtime <= entries[end].starttime) {
        // For each log that starts at the same time
        while (++beg < end) {

            LogEntry& next_log = entries[beg];
            // combine their bitrate
            l.bitrate += next_log.bitrate;

            // Shift it's end time to be the end time of shortest burst
            next_log.starttime = l.endtime;
            next_log.duration = next_log.endtime - next_log.starttime;
            beg++;
        }
    }
    else if (end < size) {
        LogEntry entry;
        {
            entry.starttime = l.starttime;
            entry.endtime = entries[end].starttime;
            entry.duration = entries[end].starttime - l.starttime;
            entry.bitrate = l.bitrate;
        }
        

        l.starttime = entry.endtime;

        while (++beg < end) {
            LogEntry& next_log = entries[beg];
            entry.bitrate += next_log.bitrate;

            // combine their bitrate
            l.bitrate += next_log.bitrate;

            // Shift it's end time to be the end time of shortest burst
            next_log.starttime = entry.endtime;
            next_log.duration = next_log.endtime - next_log.starttime;
            beg++;
        }

        set.insert(entry);
    }
    set.insert(l);

    while (entries[c++].starttime == entries[beg].starttime &&
            c < size) {}

    // Recursively call this method
}

bool order_by_start (const LogEntry& lhs, const LogEntry& rhs) {
    return lhs.starttime < rhs.starttime;
}

bool order_by_end (const LogEntry& lhs, const LogEntry& rhs) {
    return lhs.endtime < rhs.endtime;
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
