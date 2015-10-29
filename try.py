import numpy as np

a = np.asarray([1,2,3,4,5,6,7,8,9,10])
c = np.concatenate([a[5:],a[:5]])

print c


