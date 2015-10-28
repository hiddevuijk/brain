import numpy as np
import matplotlib.ticker as ticker
from matplotlib import pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages
import sys

names=["V1","V2","V4","DP","MT","8m","5","8l","TEO","2","F1","STPc","7A","46d","10","9/46v","9/46d","F5","TEpd","PBr","7m","7B","F2","STPi","ProM","F7","8B","STPr","24c"]

results = np.genfromtxt("firing_rates.csv",delimiter=';')
xvalues = np.genfromtxt("xvalues.csv")


pdf_pages = PdfPages('firing_rates.pdf')
fig = plt.figure(figsize=(8.27,11.69),dpi=100)
l=-1
r=0
maxpp = 15
for i in range(maxpp):
	l +=2
	plt.subplot(maxpp,2,l)
	if i ==0:
		plt.title("Excitatory")
	plt.plot(results[i])
	plt.yticks([max(results[i])],[r'$%3.1e$' % max(results[i])])
	plt.ylabel(names[i] ,rotation='horizontal',position=(0.5,0.2))
	plt.xticks([len(xvalues)],[r'$%g$' % xvalues[-1]])

	r += 2
	i2 = i + len(results)/2
	plt.subplot(maxpp,2,r)
	if i==0:
		plt.title("Inhibitory")
	plt.plot(results[i2])
	plt.yticks([max(results[i2])],[r'$%3.1e$' % max(results[i2])])
	plt.xticks([len(xvalues)],[r'$%g$' % xvalues[-1]])

plt.subplots_adjust(wspace=0.5,hspace=0.6,left=0.15,right=0.95,top=0.95,bottom=0.05)	
pdf_pages.savefig(fig)

fig = plt.figure(figsize=(8.27,11.69),dpi=100)
l=-1
r=0
for i in range(maxpp,len(results)/2):
	l +=2
	plt.subplot(maxpp,2,l)
	plt.plot(results[i])
	plt.yticks([max(results[i])],[r'$%3.1e$' % max(results[i])])
	plt.ylabel(names[i] ,rotation='horizontal',position=(0.5,0.2))
	plt.xticks([len(xvalues)],[r'$%g$' % xvalues[-1]])

	r += 2
	i2 = i + len(results)/2
	plt.subplot(maxpp,2,r)
	plt.plot(results[i2])
	plt.yticks([max(results[i2])],[r'$%3.1e$' % max(results[i2])])
	plt.xticks([len(xvalues)],[r'$%g$' % xvalues[-1]])

plt.subplots_adjust(wspace=0.5,hspace=0.6,left=0.15,right=0.95,top=0.95,bottom=0.05)	
pdf_pages.savefig(fig)



pdf_pages.close()





