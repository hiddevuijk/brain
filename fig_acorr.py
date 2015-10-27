import numpy as np
import matplotlib.ticker as ticker
from matplotlib import pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages


names=["V1","V2","V4","DP","MT","8m","5","8l","TEO","2","F1","STPc","7A","46d","10","9/46v","9/46d","F5","TEpd","PBr","7m","7B","F2","STPi","ProM","F7","8B","STPr","24c"]

results = np.genfromtxt("acorr.csv",delimiter=';')
xvalues = np.genfromtxt("xvalues.csv")

name_pos = (-1,0.4)

pdf_pages = PdfPages('auto_corr.pdf')
fig = plt.figure(figsize=(8.27,11.69),dpi=100)
l=-1
r=0
maxpp = 15
for i in range(maxpp):
	l +=2
	plt.subplot(maxpp,2,l)
	if i ==0:
		plt.title("Excitatory")
	result = results[i][:len(results[i])/2]
	result /= max(result)	
	plt.plot(result)
	plt.yticks([0,max(result)],[0,r'$%g$' % max(result)])
	plt.ylabel(names[i] ,rotation='horizontal',position=name_pos)
	plt.xticks([len(xvalues)],[r'$%g$' % xvalues[-1]])

	r += 2
	i2 = i + len(results)/2
	plt.subplot(maxpp,2,r)
	if i==0:
		plt.title("Inhibitory")
	result = results[i2][:len(results[i2])/2]
	result /= max(result)
	plt.plot(result)
	plt.yticks([0,max(result)],[0,r'$%g$' % max(result)])
	plt.xticks([len(xvalues)],[r'$%g$' % xvalues[-1]])

plt.subplots_adjust(wspace=0.5,hspace=0.6,left=0.15,right=0.95,top=0.95,bottom=0.05)	
pdf_pages.savefig(fig)

fig = plt.figure(figsize=(8.27,11.69),dpi=100)
l=-1
r=0
for i in range(maxpp,len(results)/2):
	l +=2
	plt.subplot(maxpp,2,l)
	result = results[i][:len(results[i])/2]
	result /= max(result)
	plt.plot(result)
	plt.yticks([0,max(result)],[0,r'$%g$' % max(result)])
	plt.ylabel(names[i] ,rotation='horizontal',position=name_pos)
	plt.xticks([len(xvalues)],[r'$%g$' % xvalues[-1]])

	r += 2
	i2 = i + len(results)/2
	plt.subplot(maxpp,2,r)
	result = results[i2][:len(results[i2])/2]
	result /= max(result)
	plt.plot(result)
	plt.yticks([0,max(result)],[0,r'$%g$' % max(result)])
	plt.xticks([len(xvalues)],[r'$%g$' % xvalues[-1]])

plt.subplots_adjust(wspace=0.5,hspace=0.6,left=0.15,right=0.95,top=0.95,bottom=0.05)	
pdf_pages.savefig(fig)



pdf_pages.close()





