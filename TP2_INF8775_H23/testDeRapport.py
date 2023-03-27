import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns
import sys
import math

df = pd.read_csv(sys.argv[1]).groupby(['algo','taille','serie']).mean().reset_index()

for i in range((int)(df.size/4)):
    if ((df.loc[i]['algo'] == 'glouton2') or (df.loc[i]['algo'] == 'glouton1')):
        df.at[i,'convergence'] = df.loc[i]['temps']/(df.loc[i]['taille']**2)
    elif ((df.loc[i]['algo'] == 'approx1') or (df.loc[i]['algo'] == 'approx2')):
        # df.at[i,'convergence'] = df.loc[i]['temps']/(df.loc[i]['taille']**2*math.log(df.loc[i]['taille']))
        df.at[i,'convergence'] = df.loc[i]['temps']/(df.loc[i]['taille']**2)
    elif ((df.loc[i]['algo'] == 'progdyn1') or (df.loc[i]['algo'] == 'progdyn1')):
        df.at[i,'convergence'] = df.loc[i]['temps']/((df.loc[i]['taille']**2)*(2**df.loc[i]['taille']))

# g = sns.FacetGrid(df, hue='algo', height=4, aspect=1)
# g = g.map(sns.scatterplot, 'taille', 'temps')
g = sns.FacetGrid(data=df, col='algo', height=4, aspect=1)
g.map_dataframe(sns.scatterplot, x='taille', y='convergence')
# g.refline(y=0.0000000196)
# g.add_legend(frameon=True)
# g.legend.set_title("Algorithmes")

for ax, algo in zip(g.axes.flat, ['approx1','approx2']):
    if (algo == 'approx1'):
        ax.set_title("avec matrices de distance")
        # ax.axhline(y=0.00000002)
    elif (algo == 'approx2'):
        ax.set_title("sans matrice de distances")
        # ax.axhline(y=0.00000002)

g.fig.suptitle('Test de rapport - Approximatif')

# g.axhline(y=0.0000000002, color='red')
# plt.axhline(y=0.0000000002)

# # Glouton log-log line
# plt.axhline(y = 0.0000000196)

plt.savefig('test_rapport_approx_rate') 
