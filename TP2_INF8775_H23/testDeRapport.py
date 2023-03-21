import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns
import sys

df = pd.read_csv(sys.argv[1]).groupby(['algo','taille','serie']).mean().reset_index()

for i in range((int)(df.size/4)):
    if df.loc[i]['algo'] == 'glouton':
        df.at[i,'convergence'] = df.loc[i]['temps']/(df.loc[i]['taille']**2)
    elif df.loc[i]['algo'] == 'approx':
        df.at[i,'convergence'] = df.loc[i]['temps']/(df.loc[i]['taille']**2)
    else:
        df.at[i,'convergence'] = df.loc[i]['temps']/(2**df.loc[i]['taille'])

# g = sns.FacetGrid(df, hue='algo', height=4, aspect=1)
# g = g.map(sns.scatterplot, 'taille', 'temps')
g = sns.FacetGrid(data=df, col='algo', height=4, aspect=1)
g.map_dataframe(sns.scatterplot, x='taille', y='convergence')
# g.refline(y=0.0000000196)
# g.add_legend(frameon=True)
# g.legend.set_title("Algorithmes")

for ax, algo in zip(g.axes.flat, ['progdyn', 'glouton', 'approx']):
    if (algo == 'approx'):
        ax.axhline(y=0.0000000196)  # Changer la valeur de y en fonction de chaque sous-graphique
        ax.set_title("Approximatif")
    elif (algo == 'glouton'):
        ax.axhline(y=0.0000000245)
        ax.set_title("Glouton")
    else:
        ax.set_title("Programmation dynamique")

g.fig.suptitle('Test du rapport')

# g.axhline(y=0.0000000002, color='red')
# plt.axhline(y=0.0000000002)

# # Glouton log-log line
# plt.axhline(y = 0.0000000196)

plt.savefig('test_rapport_prog') 
