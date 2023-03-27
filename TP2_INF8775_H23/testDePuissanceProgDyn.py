import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns
import sys
import numpy as np

df = pd.read_csv(sys.argv[1]).groupby(['algo','taille','serie']).mean().reset_index()

g = sns.lmplot(data=df, x='taille', y='temps',col='algo',ci=None,aspect=1,height=4, fit_reg=False)
g.set(xscale='log')
g.set(yscale='log')

for ax, algo in zip(g.axes.flat, ['progdyn1','progdyn2']):
    if (algo == 'progdyn1'):
        ax.set_title("avec matrices de distance")
        # ax.plot([5,25],[0.000007141844418,27.4225493])
        # ax.text(10,0.0000036,"R2 = 0.936")
    elif (algo == 'progdyn2'):
        ax.set_title("sans matrice de distances")
        # ax.text(10,0.0000036,"R2 = 0.957")
        # ax.plot([5, 25],[0.000001423712749,13.24809145])

g.fig.suptitle('Test de puissance - Programmation dynamique')

plt.savefig('test_puissance_progdyn') 
