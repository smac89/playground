import sys
import math
import operator

class PowerTower(object):
    __slots__ = ('value', 'tower')
    
    def __init__(self, tower):
        self.tower = map(int, tower.split("^"))
        self.tower = [math.log(self.tower[i]) for i in xrange(len(self.tower) - 1)] + [self.tower[-1]]
        self.value = reduce(operator.mul, self.tower)

    def __lt__(self, other):
        val_l, val_r = self.value, other.value
        if len(self.tower) < len(other.tower):
            for _ in xrange(len(other.tower) - len(self.tower)):
                val_l = math.log(val_l)
        elif len(self.tower) > len(other.tower):
            for _ in xrange(len(self.tower) - len(other.tower)):
                val_r = math.log(val_r)
        return self.val_l < val_r


def domath(cnt):
    lines = [sys.stdin.next().strip() for _ in xrange(cnt)]
    values = {line : (PowerTower(line), cnt) for cnt, line in enumerate(lines)}
    # print values
    return "\n".join(sorted(lines, key=values.__getitem__))

def fake_exponent(l):
    ans = reduce(reduction, (l[v] for v in xrange(len(l) - 1)), 1)
    return ans * l[-1]

if __name__ == '__main__':
    global reduction
    reduction = lambda x, y: math.log(x) * math.log(y)
    print "\n".join("Case %d:\n%s" %(case + 1, domath(int(cnt))) for case, cnt in enumerate(sys.stdin))
