from collections import deque

def shortest_path_to_friendship(friends_list, start, end, k):
	visited = [False for _ in range(len(friends_list))]
	pathto = [0 for _ in range(len(friends_list))]
	queue = deque()

	queue.append(start)

	bob = friends_list[start]

	while queue:
		a = queue.popleft()
		visited[a] = True
		if a == end:
			break
		curr = friends_list[a]

		# Need atleast k friends in common with anyone to make more friends
		if curr.count_friends() < k:
			continue

		for p in curr.friends:
			if not visited[p]:
				mate = friends_list[p]
				if mate.is_friends_with(bob) or bob.has_k_friends_in_common(k, mate):
					bob.make_friend(p)
					queue.append(p)
					pathto[p] = a

	p = pathto[end]
	ans = -1
	while p != 0:
		p = pathto[p]
		ans += 1
	return ans



class Person(object):
	__slots__ = ['friends', 'key']

	def __init__(self, key):
		self.friends = set()
		self.key = key

	def make_friend(self, f):
		self.friends.add(f)

	def has_k_friends_in_common(self, k, other):
		return len(other.friends.intersection(self.friends)) >= k

	def is_friends_with(self, other):
		return other.key in self.friends

	def count_friends(self):
		return len(self.friends)

	def __eq__(self, other):
		if type(other) is type(self):
			return self.key == other.key
		return false 


if __name__ == '__main__':
	for c in range(int(input())):
		n, m, k = map(int, input().split())
		l = [Person(i + 1) for i in range(n + 1)]

		for f in range(m):
			a, b = map(int, input().split())
			l[a].make_friend(b)
			l[b].make_friend(a)

		print ("Case #%d: %d" %(c + 1, shortest_path_to_friendship(l, 1, n, k)))


