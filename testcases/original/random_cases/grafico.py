import matplotlib
from matplotlib import *
import matplotlib.pyplot as plt


def getValues ():
    l = [] #labels
    x = [] #pontos_hanan
    y = [] #pontos_gerados
    z = [] #custo

    f = []
    fi = open("result.log")
    n = 0
    t = []
    for i in fi:
        t.append(i)
        n += 1
        if n % 5 == 0:
            f.append(t)
            t = []
    fi.close()

    for i in f:
        spli = i[0].split("-")
        l.append(int(spli[0])*4 + int(spli[1]) + int(spli[2])*4)
        x.append(i[1].split(":")[1][:-1])
        y.append(i[2].split(":")[1][:-1])
        z.append(i[3].split(":")[1][:-1])
    return [l, x, y, z]


l, x, y, z = getValues()

plt.plot([0, 100], [0, 0], color="black")
plt.plot([0, 0], [0, 100], color="black")


plt.title('Grafico')
plt.xlabel('Quantidade de pontos do resultado')
plt.ylabel('Quantidade de pontos da grade de hanan')
plt.grid(True)

ax = plt.gca()
y_formatter = matplotlib.ticker.ScalarFormatter(useOffset=False)
ax.yaxis.set_major_formatter(y_formatter)
plt.locator_params(axis='x',nbins=10)


plt.plot(l, x, color="red", label="#P Hanan")
plt.plot(l, y, color="blue", label="#P Resultado")
plt.plot(l, z, color="green", label="Custo")

plt.legend()

plt.show()
