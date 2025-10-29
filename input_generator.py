import random

n = 10_000_000
with open("input.txt", "w") as f:
    f.write(f"{n}\n")
    for _ in range(n):
        f.write(f"{random.uniform(0, 1e9):.3f}\n")

print("input.txt with 10^7 double values (3 decimal places) created.")
