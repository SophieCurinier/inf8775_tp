import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns
import sys

df = pd.read_csv(sys.argv[1]).groupby(['algo','taille','serie']).mean().reset_index()

for i in range((int)(df.size/4)):
    if df.loc[i]['algo'] == 'conv':
        df.at[i,'temps'] = df.loc[i]['temps']/(df.loc[i]['taille']**3)
    elif df.loc[i]['algo'] == 'strassen':
        df.at[i,'temps'] = df.loc[i]['temps']/(df.loc[i]['taille']**2.807)
    else:
        df.at[i,'temps'] = df.loc[i]['temps']/(df.loc[i]['taille']**2.807)

g = sns.FacetGrid(df, hue='algo', height=4, aspect=1)
g = g.map(sns.scatterplot, 'taille', 'temps')
g.add_legend()
plt.savefig('test_rapport') 
