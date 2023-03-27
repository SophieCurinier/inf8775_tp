import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns
import sys
import math

df = pd.read_csv(sys.argv[1]).groupby(['algo','taille','serie']).mean().reset_index()

for i in range((int)(df.size/4)):
    if ((df.loc[i]['algo'] == 'glouton2') or (df.loc[i]['algo'] == 'glouton1')):
        df.at[i,'convergence'] = df.loc[i]['temps']/(df.loc[i]['taille']**2)

g = sns.FacetGrid(data=df, col='algo', height=4, aspect=1)
g.map_dataframe(sns.scatterplot, x='taille', y='convergence')

for ax, algo in zip(g.axes.flat, ['glouton1','glouton2']):
    if (algo == 'glouton1'):
        ax.set_title("avec matrice de distances")
        ax.axhline(y=0.00000001956234736)
    elif (algo == 'glouton2'):
        ax.set_title("sans matrice des distances")
        ax.axhline(y=0.00000001966234736)

g.fig.suptitle('Test de rapport - Glouton')

plt.savefig('test_rapport_glouton') 
