import numpy as np
import matplotlib.ticker as ticker
from matplotlib import pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages
import sys

names=["V1","V2","V4","DP","MT","8m","5","8l","TEO","2","F1","STPc","7A","46d","10","9/46v","9/46d","F5","TEpd","PBr","7m","7B","F2","STPi","ProM","F7","8B","STPr","24c"]


results_n = np.genfromtxt("corr_in_n.csv",delimiter=';')
results_a = np.genfromtxt("corr_in_a.csv",delimiter=';')
results_b = np.genfromtxt("corr_in_b.csv",delimiter=';')
xvalues = np.genfromtxt("xvalues_n.csv")

size = len(results_n[0])

pdf_pages = PdfPages('corr_in.pdf')
fig = plt.figure(figsize=(8.27,11.69),dpi=100)
l=-1
r=0
maxpp = 15
for i in range(maxpp):
	l +=2
	plt.subplot(maxpp,2,l)
	if i ==0:
		plt.title("Excitatory")

	result_n = np.concatenate([results_n[i][size/2:],results_n[i][:size/2]])
	result_a = np.concatenate([results_a[i][size/2:],results_a[i][:size/2]])
	result_b = np.concatenate([results_b[i][size/2:],results_b[i][:size/2]])

	plt.plot(result_n,color='blue')
	plt.plot(result_a,color='red')
	plt.plot(result_b,color='green')

	ymax = max(max(result_n),max(result_a),max(result_b))
	plt.yticks([0,ymax],[0,r'$%3.1e$' % ymax])

	plt.ylabel(names[i] ,rotation='horizontal',position=(0.5,0.2))
	plt.xticks([0,size/2,size],[-1*size/2,0,size/2])

	r += 2
	i2 = i + len(results_n)/2
	plt.subplot(maxpp,2,r)
	if i==0:
		plt.title("Inhibitory")

	result_n = np.concatenate([results_n[i2][size/2:],results_n[i2][:size/2]])
	result_a = np.concatenate([results_a[i2][size/2:],results_a[i2][:size/2]])
	result_b = np.concatenate([results_b[i2][size/2:],results_b[i2][:size/2]])

	plt.plot(result_n,color='blue')
	plt.plot(result_a,color='red')
	plt.plot(result_b,color='green')

	ymax = max(max(result_n),max(result_a),max(result_b))
	plt.yticks([0,ymax],[0,r'$%3.1e$' % ymax])

	plt.xticks([0,size/2,size],[-1*size/2,0,size/2])

plt.subplots_adjust(wspace=0.5,hspace=0.6,left=0.15,right=0.95,top=0.95,bottom=0.05)	
pdf_pages.savefig(fig)

fig = plt.figure(figsize=(8.27,11.69),dpi=100)
l=-1
r=0
for i in range(maxpp,len(results_n)/2):
	l +=2
	plt.subplot(maxpp,2,l)

	result_n = np.concatenate([results_n[i][size/2:],results_n[i][:size/2]])
	result_a = np.concatenate([results_a[i][size/2:],results_a[i][:size/2]])
	result_b = np.concatenate([results_b[i][size/2:],results_b[i][:size/2]])

	plt.plot(result_n,color='blue')
	plt.plot(result_a,color='red')
	plt.plot(result_b,color='green')

	ymax = max(max(result_n),max(result_a),max(result_b))
	plt.yticks([0,ymax],[0,r'$%3.1e$' % ymax])

	plt.xticks([0,size/2,size],[-1*size/2,0,size/2])

	r += 2
	i2 = i + len(results_n)/2
	plt.subplot(maxpp,2,r)

	result_n = np.concatenate([results_n[i2][size/2:],results_n[i2][:size/2]])
	result_a = np.concatenate([results_a[i2][size/2:],results_a[i2][:size/2]])
	result_b = np.concatenate([results_b[i2][size/2:],results_b[i2][:size/2]])

	plt.plot(result_n,color='blue')
	plt.plot(result_a,color='red')
	plt.plot(result_b,color='green')

	ymax = max(max(result_n),max(result_a),max(result_b))
	plt.yticks([0,ymax],[0,r'$%3.1e$' % ymax])

	plt.xticks([0,size/2,size],[-1*size/2,0,size/2])

plt.subplots_adjust(wspace=0.5,hspace=0.6,left=0.15,right=0.95,top=0.95,bottom=0.05)	
pdf_pages.savefig(fig)



pdf_pages.close()





