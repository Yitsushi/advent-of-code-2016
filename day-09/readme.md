Interesting fact

### Part1

```
# v1
❯ time (for i in `seq 1 1000`; do ./part1/solution input > /dev/null; done)
( for i in `seq 1 1000`; do; ./part1/solution input > /dev/null; done; )  1.59s user 1.02s system 57% cpu 4.562 total

# v2
❯ time (for i in `seq 1 1000`; do ./part1_v2/solution input > /dev/null; done)
( for i in `seq 1 1000`; do; ./part1_v2/solution input > /dev/null; done; )  1.60s user 1.04s system 57% cpu 4.594 total
```

### Part2


```
# v1
❯ time (for i in `seq 1 1000`; do ./part2/solution input > /dev/null; done)
( for i in `seq 1 1000`; do; ./part2/solution input > /dev/null; done; )  2.18s user 1.03s system 61% cpu 5.186 total

# v2
❯ time (for i in `seq 1 1000`; do ./part2_v2/solution input > /dev/null; done)
( for i in `seq 1 1000`; do; ./part2_v2/solution input > /dev/null; done; )  2.13s user 1.04s system 61% cpu 5.131 total
```
