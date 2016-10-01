import sys

oppo = {
'+x' : {'+x' : '+x', '+y' : '+y', '+z' : '+z', '-x': 'NO', '-y': '-y', '-z': '-z'},
'+y' : {'+x' : '+y', '+y' : '-x', '+z' : '+y', '-x': 'NO', '-y': '+x', '-z': '+y'},
'+z' : {'+x' : '+z', '+y' : '+z', '+z' : '-x', '-x': 'NO', '-y': '+z', '-z': '+x'},
'-x' : {'+x' : '-x', '+y' : '-y', '+z' : '-z', '-x': 'NO', '-y': '+y', '-z': '+z'},
'-y' : {'+x' : '-y', '+y' : '+x', '+z' : '-y', '-x': 'NO', '-y': '-x', '-z': '-y'},
'-z' : {'+x' : '-z', '+y' : '-z', '+z' : '+x', '-x': 'NO', '-y': '-z', '-z': '-x'}
}

for line in sys.stdin.readlines():
	inp = line.split()
	if inp[0].isdigit():
		continue
	pos = '+x'
	for v in inp:
		if v == 'No':
			continue
		pos = oppo[pos][v]

	print (pos)
