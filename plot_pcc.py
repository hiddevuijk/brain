import numpy as np
import matplotlib.ticker as ticker
from matplotlib import pyplot as plt

ymin=0.0


names=["V1","V2","V4","DP","MT","8m","5","8l","TEO","2","F1","STPc","7A","46d","10","9/46v","9/46d","F5","TEpd","PBr","7m","7B","F2","STPi","ProM","F7","8B","STPr","24c"]

pcc = np.genfromtxt("pcc.csv",delimiter=';');
pcce=pcc[0:len(pcc)/2]
pcci=pcc[len(pcc)/2:]

plt.subplot(211)
plt.bar(range(0,len(pcce)),pcce,align='center')
plt.ylim([ymin,1])
plt.xticks(range(0,len(pcce)),names, rotation='vertical')

plt.subplot(212)
plt.bar(range(0,len(pcci)),pcci,align='center')
plt.ylim([ymin,1])
plt.xticks(range(0,len(pcci)),names, rotation='vertical')

plt.show()





