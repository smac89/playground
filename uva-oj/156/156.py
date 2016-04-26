from sys import stdin

words = dict()

for line in stdin.readlines():
	line = line.strip()
	if line == '#':
		break

	for w in line.split():
		w = w.strip()
		if not w:
			continue
		v = "".join(sorted(w.lower()))
		if v not in words:
			words[v] = [w]
		else:
			words[v].append(w)

l = [v[0] for k,v in words.items() if len(v) == 1]
print ("\n".join(sorted(l)))
