################### PART 2 ##########################


# shuffle inverse
def cut_inv(pos, N, cut_num):
    return (pos + cut_num) % N


def deal_into_new_stack_inv(pos, N):
    return N - 1 - pos


def deal_with_increment_inv(pos, N, incr):
    c = modinv(N % incr, incr) * (- pos) % incr
    return (pos + c * N) // incr


def reverse_shuffle(pos, N, rev_g):
    for line in rev_g:
        if line[0] == 'c':
            cut_num = int(line[3:])
            pos = cut_inv(pos, N, cut_num)
        elif line[5] == 'w':
            incr = int(line[19:])
            pos = deal_with_increment_inv(pos, N, incr)
        elif line[5] == 'i':
            pos = deal_into_new_stack_inv(pos, N)
    return pos


# regular shuffle
def quick_deal_into_new_stack(pos, N):
    return N - 1 - pos


def quick_cut(pos, N, cut_num):
    return (pos - cut_num) % N


def quick_deal_with_increment(pos, N, incr):
    res = pos * incr
    res %= N
    return res


def quick_shuffle(pos, N, g):
    for line in g:
        if line[0] == 'c':
            cut_num = int(line[3:])
            pos = quick_cut(pos, N, cut_num)
        elif line[5] == 'w':
            incr = int(line[19:])
            pos = quick_deal_with_increment(pos, N, incr)
        elif line[5] == 'i':
            pos = quick_deal_into_new_stack(pos, N)
    return pos


# other helpful functions
def convert_grid(g, N):
    p = 1
    for line in g:
        if len(line) < 6:
            continue
        elif line[5] == 'w':
            p *= int(line[19:])
            p %= N
        elif line[5] == 'i':
            p *= N-1
            p %= N
    return p


def convert_dec_to_bin(dec):
    max_pow = 0
    while 2 ** (max_pow + 1) <= dec:
        max_pow += 1

    res = [0 for _ in range(max_pow + 1)]
    cnt = 0
    while dec > 0:
        if dec - 2 ** max_pow >= 0:
            dec -= 2 ** max_pow
            res[cnt] = 1
        max_pow -= 1
        cnt += 1
    return res


def egcd(a, b):
    if a == 0:
        return (b, 0, 1)
    else:
        g, y, x = egcd(b % a, a)
        return (g, x - (b // a) * y, y)


def modinv(a, m):
    g, x, y = egcd(a, m)
    if g != 1:
        raise Exception('modular inverse does not exist')
    else:
        return x % m


## READ DATA
file = open('C:/Users/s_ten/PycharmProjects/CodeOfAdvent/data/problem22.txt')

grid = []
for line in file.readlines():
    grid.append(line[:-1])


# Get input
deck_size = 119315717514047
number_of_shuffles = 101741582076661


# Convert number_of_shuffles to binary
number_of_shuffles_binary = convert_dec_to_bin(number_of_shuffles)
cnt = len(number_of_shuffles_binary)


# pre-allocation
new_grid = [[] for _ in range(cnt)]


# simplify data/grid
i = 0
while i < cnt:
    if i == 0:
        temp_grid = grid
    else:
        temp_grid = new_grid[i-1] + new_grid[i-1]

    incr = convert_grid(temp_grid, deck_size)
    pos0 = quick_shuffle(0, deck_size, temp_grid)
    new_grid[i] = ['deal with increment ' + str(incr), 'cut ' + str(deck_size - pos0)]
    i += 1

# Only use the relevant parts of that grid
total_grid = []
for i in range(cnt):
    if number_of_shuffles_binary[-i - 1]:
        total_grid += new_grid[i]

print(reverse_shuffle(2020, deck_size, total_grid[::-1]))
