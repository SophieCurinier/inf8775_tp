import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns
import sys
import numpy as np

df = pd.read_csv(sys.argv[1]).groupby(['algo','taille','serie']).mean().reset_index()

g = sns.lmplot(data=df, x='taille', y='temps',col='algo',ci=None,aspect=1,height=4, fit_reg=False)
g.set(xscale='log')
g.set(yscale='log')

for ax, algo in zip(g.axes.flat, ['glouton1','glouton2']):
    if (algo == 'glouton1'):
        ax.set_title("avec matrice de distances")
        ax.text(10,0.0000036,"R2 = 0.999")
        ax.plot([1000, 50000],[0.0176132,48.9058684])
    elif (algo == 'glouton2'):
        ax.set_title("sans matrice des distances")
        ax.text(10,0.0000036,"R2 = 0.999")
        ax.plot([1000, 100000],[0.017816,194.188949])

g.fig.suptitle('Test de puissance - Glouton')

plt.savefig('test_puissance_glouton') 
