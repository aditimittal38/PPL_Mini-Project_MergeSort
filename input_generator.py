import random

n = 10000000
decimal_places = 7
with open("input.txt", "w") as f:
    f.write(f"{n}\n")
    for _ in range(n):
        f.write(f"{random.uniform(0, 1e9):.{decimal_places}f}\n")

print(f"input.txt with {n} double values ({decimal_places} decimal places) created.")
