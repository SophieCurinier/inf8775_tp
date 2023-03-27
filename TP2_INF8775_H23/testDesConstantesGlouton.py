import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns
import sys
import scipy as sp

df = pd.read_csv(sys.argv[1]).groupby(['algo','taille','serie']).mean().reset_index()

for i in range((int)(df.size/4)):
    if ((df.loc[i]['algo'] == 'glouton2') or (df.loc[i]['algo'] == 'glouton1')):
        df.at[i,'f(n)'] = df.loc[i]['taille']**2

g = sns.lmplot(data=df, x='f(n)', y='temps', col='algo', ci=None,robust=True)

def annotate(data, **kws):
    r, p = sp.stats.pearsonr(data['f(n)'], data['temps'])
    ax = plt.gca()
    ax.text(.05, .8, 'r2={:.8f}, p={:.5g}'.format(r, p),
            transform=ax.transAxes)

g.map_dataframe(annotate)
for ax, algo in zip(g.axes.flat, ['glouton1','glouton2']):
    if (algo == 'glouton1'):
        ax.set_title("avec matrice de distances")
    elif (algo == 'glouton2'):
        ax.set_title("sans matrice des distances")

g.fig.suptitle('Test des constantes - Glouton')
plt.savefig('test_constante_glouton')
