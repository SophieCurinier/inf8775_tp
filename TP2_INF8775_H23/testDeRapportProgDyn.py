import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns
import sys

df = pd.read_csv(sys.argv[1]).groupby(['algo','taille','serie']).mean().reset_index()

for i in range((int)(df.size/4)):
    if ((df.loc[i]['algo'] == 'glouton2') or (df.loc[i]['algo'] == 'glouton1')):
        df.at[i,'convergence'] = df.loc[i]['temps']/(df.loc[i]['taille']**2)
    elif ((df.loc[i]['algo'] == 'approx1') or (df.loc[i]['algo'] == 'approx2')):
        df.at[i,'convergence'] = df.loc[i]['temps']/(df.loc[i]['taille']**2)
    elif ((df.loc[i]['algo'] == 'progdyn1') or (df.loc[i]['algo'] == 'progdyn2')):
        df.at[i,'convergence'] = df.loc[i]['temps']/ ((df.loc[i]['taille']**2)*(2**df.loc[i]['taille']))

g = sns.FacetGrid(data=df, col='algo', height=4, aspect=1)
g.map_dataframe(sns.scatterplot, x='taille', y='convergence')

for ax, algo in zip(g.axes.flat, ['progdyn1','progdyn2']):
    if (algo == 'progdyn1'):
        ax.set_title("avec matrices de distance")
        ax.axhline(y=0.00000000351844280243)
    elif (algo == 'progdyn2'):
        ax.set_title("sans matrice de distances")
        ax.axhline(y=0.00000000930993815422)


g.fig.suptitle('Test du rapport - Programmation dynamique')

plt.savefig('test_rapport_progdyn') 
