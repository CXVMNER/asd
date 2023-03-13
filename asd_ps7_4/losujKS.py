import random
f = open("KS_in_7_Karolczak.txt", 'w')

n = 10
m = random.randint(n-1, n*2)
w = random.randint(1, n)

k = w
while k == w:
    k = random.randint(1, n)

s = random.randint(n*2, n*3)

f.write(f"{n} {m} {w} {k} {s}\n")

o = ""
for _ in range(n):
    o += f"{random.randint(1, 15)} "

f.write(f"{o}\n")

ones = []
for _ in range(n+1):
    ones.append(1)
edges = []
for i in range(n+1):
    edges.append(list(ones))
    edges[i][i] = 0

ranE = ""
for _ in range(m):
    x = 0
    y = 0
    while(True):
        x = random.randint(1, n)
        y = random.randint(1, n)
        if edges[x][y] == 1:
            edges[x][y] = 0
            break
    ranE += f"{x} {y}\n"

f.write(ranE)
f.close()