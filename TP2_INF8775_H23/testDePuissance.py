import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns
import sys
import numpy as np

df = pd.read_csv(sys.argv[1]).groupby(['algo','taille','serie']).mean().reset_index()

g = sns.lmplot(data=df, x='taille', y='temps',col='algo',ci=None,aspect=1,height=4, fit_reg=False)
g.set(xscale='log')
g.set(yscale='log')

for ax, algo in zip(g.axes.flat, ['approx1','approx2']):
    if (algo == 'approx1'):
        ax.set_title("avec matrices de distance")
        ax.plot([1000,50000],[0.0240294,62.0883816])
    elif (algo == 'approx2'):
        ax.set_title("sans matrice de distances")
        ax.plot([1000, 50000],[0.0412714,104.8301636])

g.fig.suptitle('Test de puissance - Approximatif')

plt.savefig('test_puissance_approx') 
