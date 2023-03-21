import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns
import sys

df = pd.read_csv(sys.argv[1]).groupby(['algo','taille','serie']).mean().reset_index()

# g = sns.FacetGrid(df, col='serie', hue='algo', height=4, aspect=1)
# g = sns.lmplot(data=df, x='taille', y='temps', hue='algo', height=4, aspect=1)
g = sns.lmplot(data=df, x='taille', y='temps',col='algo',ci=None,aspect=1,height=4, fit_reg=False)


# g = g.map(plt.plot, 'taille', 'temps')
g.set(xscale='log')
g.set(yscale='log')
g.add_legend(frameon=True)
g.legend.set_title("Algorithmes")

g.fig.suptitle('Test de puissance')
# Glouton log-log line
# plt.plot([1000,0.0176132],[50000,48.9])

# Approx log-log line
# plt.plot([1000,0.024],[50000,62.08],'orange')

plt.savefig('test_puissance') 
