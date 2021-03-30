import itertools

test = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1]
test2 = [0,0,0,0,0,0,1,1,1,1]

print(len(test2))

a = list(itertools.combinations(test2, 9))

print(len(a))
