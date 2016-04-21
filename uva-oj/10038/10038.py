import sys

for line in sys.stdin:
	line = map(int, line.strip().split())
	sz = next(line)
	s = set([k for k in range(1, sz)])
	prev = next(line)
	for v in line:
		s.discard(abs(prev - v))
		prev = v
	if not s:
		print("Jolly")
	else:
		print("Not jolly")
