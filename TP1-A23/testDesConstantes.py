import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns
import sys

df = pd.read_csv(sys.argv[1]).groupby(['algo','taille','serie']).mean().reset_index()

for i in range((int)(df.size/4)):
    if df.loc[i]['algo'] == 'conv':
        df.at[i,'f(n)'] = df.loc[i]['taille']**3
    elif df.loc[i]['algo'] == 'strassen':
        df.at[i,'f(n)'] = df.loc[i]['taille']**2.807
    else:
        df.at[i,'f(n)'] = df.loc[i]['taille']**2.807

# g = sns.FacetGrid(df, hue='algo', height=4, aspect=1)
# g = g.map(sns.scatterplot, 'f(n)', 'temps')
g = sns.lmplot(data=df, x='f(n)', y='temps', hue='algo', height=4, aspect=1)
g.set(xscale='log')
g.set(yscale='log')
g.add_legend(frameon=True)
g.legend.set_title("Algorithmes")
plt.savefig('test_constante')
