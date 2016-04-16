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
    int bitrate;
    double streamed, duration;
};

using LogSet = std::set<LogEntry, bool (*) (const LogEntry&, const LogEntry&)>;
using LogEntries = std::vector<LogEntry>;
using CountEntry = LogEntries::size_type;
using StartGroup = std::list<std::pair<ull, CountEntry>>;

void reorder(LogEntries&, StartGroup&);
StartGroup get_start_times(LogEntries&);
std::istream &operator >> (std::istream&, LogEntry&);

#ifdef DEBUG
std::ostream &operator << (std::ostream&, const LogEntry&);

template <typename F, typename S>
std::ostream &operator << (std::ostream&, const std::pair<F, S>&);
#endif

int main() {

    LogEntries logEntries;
    CountEntry logsize;
    std::cin >> logsize;

    // LogSet entries([](const LogEntry& lhs, const LogEntry& rhs) {
    //         return lhs.starttime < rhs.starttime;
    // });

    std::copy_n(std::istream_iterator<LogEntry>(std::cin), logsize,
        std::back_inserter(logEntries));

    // sort the entries based on start times
    std::sort(logEntries.begin(), logEntries.end(),
        [](const LogEntry& lhs, const LogEntry& rhs) {
            return lhs.starttime < rhs.starttime;
    });

    StartGroup startTimes = get_start_times(logEntries);

    for (auto& ent : logEntries) {
        std::cout << ent << '\n';
    }

    std::cout << "\n\n";

    for (auto& p : startTimes) {
        std::cout << p << '\n';
    }

    // reorder(logEntries, startTimes);
    return 0;
}

void reorder(LogEntries &entries, StartGroup &startTimes) {
    auto order_by_end = [](const LogEntry& lhs, const LogEntry& rhs) {
        return lhs.endtime < rhs.endtime;
    };

    ull start = startTimes.front().first;
    CountEntry countbeg = 0, countend = startTimes.front().second;
    startTimes.pop_front();

    while (!startTimes.empty()) {
        ull next_start = startTimes.front().first;

        std::sort(entries.begin() + countbeg, entries.begin() + countend, order_by_end);

        LogEntry &l = entries[countbeg];
        if (l.endtime <= next_start) {
            for (countbeg++; countbeg < countend; countbeg++) {

                LogEntry& next_log = entries[countbeg];
                l.bitrate += next_log.bitrate;

                // next_log.
            }
        }
        
    }




    // while (!startTimes.empty()) {
    //     ull end = startTimes.front();
    //     startTimes.pop_front();

    //     LogEntry entry = {.endtime = end, .starttime = start};

    // }
}

StartGroup get_start_times(LogEntries& entries) {
    // get the unique start times of all the entries along with the number
    // of elements with the same start time
    StartGroup startTimes;
    ull curr = entries[0].starttime, next;
    CountEntry count = 0, i = 1;

    while (i != entries.size()) {
        next = entries[i].starttime;
        if (next > curr) {
            startTimes.push_back(std::make_pair(curr, i - count));
            count = i;
            curr = next;
        }
        i++;
    }

    startTimes.push_back(std::make_pair(curr, i - count));
    return startTimes;
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
