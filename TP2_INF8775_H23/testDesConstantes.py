import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns
import sys
import scipy as sp

df = pd.read_csv(sys.argv[1]).groupby(['algo','taille','serie']).mean().reset_index()

for i in range((int)(df.size/4)):
    if df.loc[i]['algo'] == 'glouton':
        df.at[i,'f(n)'] = df.loc[i]['taille']**2
    elif df.loc[i]['algo'] == 'approx':
        df.at[i,'f(n)'] = df.loc[i]['taille']**2
    else:
        df.at[i,'f(n)'] = df.loc[i]['taille']**2.8074

# g = sns.FacetGrid(df, hue='algo', height=4, aspect=1)
# g = g.map(sns.scatterplot, 'f(n)', 'temps')
g = sns.lmplot(data=df, x='f(n)', y='temps', col='algo', ci=None,robust=True)
# g.add_legend(frameon=True)
# g.legend.set_title("Algorithmes")
def annotate(data, **kws):
    r, p = sp.stats.pearsonr(data['f(n)'], data['temps'])
    ax = plt.gca()
    ax.text(.05, .8, 'r2={:.8f}, p={:.5g}'.format(r, p),
            transform=ax.transAxes)
    print(r)
g.map_dataframe(annotate)

plt.savefig('test_constante')
