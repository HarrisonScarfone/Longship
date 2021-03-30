string = "0000000000000000000000000000000000000000000000000000000000000001"
print(len(string))

print("{")

j = 1

for i in range(64):
    print("{", end="")
    print(f"{i}, {j}", end="")
    print("},")
    j = j << 1