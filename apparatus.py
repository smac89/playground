from math import factorial as fac

if __name__ == '__main__':
    n, m = map(int, raw_input().split())
    seen = set()
    for _ in xrange(m):
        r1, r2 = (raw_input(), raw_input())
