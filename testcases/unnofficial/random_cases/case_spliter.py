import random
import os


def generate_case(path, quant):
    f = open(path)
    case = []
    for i in f:
        case.append(i)

    ns = int(case[4].split(" = ")[1])
    nv = int(case[5].split(" = ")[1])
    no = int(case[6].split(" = ")[1])
    intervals = [(0, ns-1), (ns, ns + nv-1), (ns + nv, ns + nv + no-1)]

    new_case = ""
    new_case += case[0] + case[1] + case[2] + case[3]
    new_case += "#RoutedShapes = " + str(quant[0]) + "\n"
    new_case += "#RoutedVias = " + str(quant[1]) + "\n"
    new_case += "#Obstacles = " + str(quant[2]) + "\n"

    case = case[7:]

    shapes = case[intervals[0][0]:intervals[0][1]+1]
    vias = case[intervals[1][0]:intervals[1][1]+1]
    obstacles = case[intervals[2][0]:intervals[2][1]+1]

    for i in range(quant[0]):
        new_case += shapes.pop(random.randint(0, len(shapes) - 1))

    for i in range(quant[1]):
        new_case += vias.pop(random.randint(0, len(vias) - 1))

    for i in range(quant[2]):
        new_case += obstacles.pop(random.randint(0, len(obstacles) - 1))

    return new_case



OUT_PATH = "./"
IN_PATH  = "../case1"  # (1503, 3, 414)
'''
IN_PATH  = "../case2"  # (4518, 34, 4773)
IN_PATH  = "../case3"  # (97146, 52, 79012)
'''
MAX_SHAPES = 50
MAX_VIAS = 3
MAX_OBSTACLES = 50

s = v = o = 1
log = ""
for i in range(max([MAX_SHAPES, MAX_VIAS, MAX_OBSTACLES])):
    new_case = generate_case(IN_PATH, (s, v, o))

    path = str(s) + "-" + str(v) + "-" + str(o)
    f = open(path, 'w')
    f.write(new_case)
    f.close()
    # ./bi1st ../cases/case-3 no

    log += path + "\n"
    os.system('../../src/bi1st ./' + path + ' no > result.log')
    os.system('../../src/eval_3 ./' + path + ' ../../src/out/saida.txt > result_eval.log')

    f = open("result.log")
    f2 = open("result_eval.log")

    ind = 0
    for j in f:
        if ind == 11:
            log += "pontos_hanan:" + j.split(": ")[1]
        elif ind == 12:
            log += "pontos_gerados:" + j.split(": ")[1]
            break
        ind += 1

    ind = 0
    for j in f2:
        if ind == 6:
            print(j, path, i)
            try:
                log += "custo:" + j.split(" = ")[1]
            except:
                log += "custo:" + "-1\n"
            break
        ind += 1

    log += "\n"
    f.close()


    if s <= MAX_SHAPES:
        s += 1
    if s <= MAX_VIAS:
        v += 1
    if s <= MAX_OBSTACLES:
        o += 1

f = open("result.log", "w")
f.write(log)
f.close()
