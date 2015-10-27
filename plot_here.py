import numpy as np
import matplotlib.ticker as ticker
from matplotlib import pyplot as plt

names=["V1","V2","V4","DP","MT","8m","5","8l","TEO","2","F1","STPc","7A","46d","10","9/46v","9/46d","F5","TEpd","PBr","7m","7B","F2","STPi","ProM","F7","8B","STPr","24c"]

results = np.genfromtxt("firing_rates.csv",delimiter=';')
xvalues = np.genfromtxt("xvalues.csv")

rspace = 1
cspace = 1
rs=2
cs=2

grid_size = ((rs+rspace)*(len(results)/4+1),cs*2+cspace)

plt.figure(figsize=(12,9))

for i in range(len(results)/2):
	lr = 0
	ii =i
	if (i >= len(results)/4) :
		lr = 1
		ii -= len(results)/4
	plt.subplot2grid(grid_size, (ii*(rs+rspace),lr*(cs+cspace)), rowspan=rs,colspan=cs)
	
	plt.plot(results[i])
	plt.xticks([len(xvalues)],[r'$%g$' % xvalues[-1]])
	plt.yticks([max(results[i])],[r'$%g$' % max(results[i])])
	plt.ylabel(names[i],rotation='horizontal')

plt.subplots_adjust(left=0.1, bottom=0.02, right=0.95, top=0.98)
plt.savefig("fig.png")





