import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns
import sys
import scipy as sp
import math

df = pd.read_csv(sys.argv[1]).groupby(['algo','taille','serie']).mean().reset_index()

for i in range((int)(df.size/4)):
    if ((df.loc[i]['algo'] == 'progdyn1') or (df.loc[i]['algo'] == 'progdyn2')):
        df.at[i,'f(n)'] = (df.loc[i]['taille']**2)*(2**df.loc[i]['taille'])

g = sns.lmplot(data=df, x='f(n)', y='temps', col='algo', ci=None,robust=True)

def annotate(data, **kws):
    r, p = sp.stats.pearsonr(data['f(n)'], data['temps'])
    ax = plt.gca()
    ax.text(.05, .8, 'r2={:.8f}, p={:.5g}'.format(r, p),
            transform=ax.transAxes)

g.map_dataframe(annotate)
for ax, algo in zip(g.axes.flat, ['progdyn1','progdyn2']):
    if (algo == 'progdyn1'):
        ax.set_title("avec matrices de distance")
    elif (algo == 'progdyn2'):
        ax.set_title("sans matrice de distances")

g.fig.suptitle('Test des constantes - Programmation dynamique')
plt.savefig('test_constante_progdyn')
