import numpy as np
import matplotlib.ticker as ticker
from matplotlib import pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages


names=["V1","V2","V4","DP","MT","8m","5","8l","TEO","2","F1","STPc","7A","46d","10","9/46v","9/46d","F5","TEpd","PBr","7m","7B","F2","STPi","ProM","F7","8B","STPr","24c"]
results = np.genfromtxt("coh_V1.csv",delimiter=';')
f = np.genfromtxt("f.csv",delimiter=';')

pdf_pages = PdfPages('coh_V1.pdf')
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
	plt.yticks([0,max(results[i])],[0,'%1.1f' % max(results[i])])
	plt.ylabel(names[i] ,rotation='horizontal',position=(0.5,0.2))
	plt.xticks([0,len(results[i])],[0,"%.1f" % f[-1]])
	plt.ylim([0,1.3])

	r += 2
	i2 = i + len(results)/2
	plt.subplot(maxpp,2,r)
	if i==0:
		plt.title("Inhibitory")
	plt.plot(results[i2])
	plt.yticks([0,max(results[i2])],[0,'%1.1f' % max(results[i2])])
	plt.xticks([0,len(results[i2])],[0,'%.1f' % f[-1]])
	plt.ylim([0,1.3])

plt.subplots_adjust(wspace=0.5,hspace=0.6,left=0.15,right=0.95,top=0.95,bottom=0.05)	
pdf_pages.savefig(fig)

fig = plt.figure(figsize=(8.27,11.69),dpi=100)
l=-1
r=0
for i in range(maxpp,len(results)/2):
	l +=2
	plt.subplot(maxpp,2,l)
	plt.plot(results[i])
	plt.yticks([0,max(results[i])],[0,'%1.1f' % max(results[i])])
	plt.ylabel(names[i] ,rotation='horizontal',position=(0.5,0.2))
	plt.xticks([0,len(results[i])],[0,'%.1f' % f[-1]])

	plt.ylim([0,1.3])
	r += 2
	i2 = i + len(results)/2
	plt.subplot(maxpp,2,r)
	plt.plot(results[i2])
	plt.yticks([0,max(results[i2])],[0,'%1.1f' % max(results[i2])])
	plt.xticks([0,len(results[i])],[0,'%.1f' % f[-1]])
	plt.ylim([0,1.3])

plt.subplots_adjust(wspace=0.5,hspace=0.6,left=0.15,right=0.95,top=0.95,bottom=0.05)	
pdf_pages.savefig(fig)



pdf_pages.close()





