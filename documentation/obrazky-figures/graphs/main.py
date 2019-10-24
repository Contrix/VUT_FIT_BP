import matplotlib.pyplot as plt
import numpy as np
import math

"""
# water desnsity 33
t = np.arange(0, 41, 1)
h = [999.8426, 999.9015, 999.9429, 999.9672, 999.9750, 999.9668, 999.9430, 999.9043, 999.8509, 999.7834, 999.7021, 999.6074, 999.4996, 999.3792, 999.2464, 999.1016, 998.9450, 998.7769, 998.5976, 998.4073, 998.2063, 997.9948, 997.7730, 997.5412, 997.2994, 997.0480, 996.7870, 996.5166, 996.2371, 995.9486, 995.6511, 995.3450, 995.0302, 994.7071, 994.3756, 994.0359, 993.6883, 993.3328, 992.9695, 992.5987, 992.2204]
plt.plot(t, h, 'b+')
plt.ylabel(r'Hustota $[kg\cdot m^{-3}]$')
plt.xlabel(r'Teplota $[^\circ C$]')
plt.grid(True, linestyle='--')
plt.savefig('../water_density.pdf')
#plt.show()

"""

"""
# sound speed 49
# t = [0, 5, 10, 15, 20, 40]
# v = [331.46, 334.5, 337.5, 340.6, 343.6, 355.1]
t = np.arange(0, 41, 1)
v = []
for i in t:
	v.append(math.sqrt(1.402 * 287.05 * (i + 273.15)))
plt.plot(t, v, 'b+')
plt.ylabel(r'Rychlost $[m\cdot s^{-1}]$')
plt.xlabel(r'Teplota $[^\circ C]$')
plt.grid(True, linestyle='--')
#plt.show()
plt.savefig('../sound_speed.pdf')
"""

"""
# light speed 126
c = 2.997924580*10**8
g = 3670*10**-6
lam = np.arange(600, 900, 10)
v = {}
v[15] = []
v[20] = []

for i in lam:
	n = ((64.328+29498.1/(146-(1/i)**2)+255.4/(41-(1/i)**2))/10**6)+1
	v[15].append(c/n)
	v[20].append(c/((n-1)*(1+15*g)/(1+20*g) + 1))


plt.plot(v[15], lam, 'b+')
plt.ylabel(r'Vlnová délka $[\mu m]$')
plt.xlabel(r'Rychlost $[m\cdot s^{-1}]$')
plt.grid(True, linestyle='--')
# plt.show()
plt.savefig('../light_speed.pdf')

"""

from matplotlib.ticker import (MultipleLocator, FormatStrFormatter, AutoMinorLocator)

"""
podlaha = (398, 506, 601, 691, 795, 896, 1002, 1096, 1198, 1301, 1394, 1487, 1590, 1693, 1792, 1900, 1989)
voda = (353, 448, 558, 644, 750, 853, 941, 1047, 1183, 1247, 1355, 1459, 1592, 1655, 1741, 1853, 1976)
pena = (357, 452, 558, 665, 756, 853, 982, 1073, 1157, 1252, 1370, 1510, 1558, 1663, 1754, 1853, 1948)
"""

podlaha = (372, 486, 590, 680, 782, 872, 995, 1092, 1185, 1280, 1381, 1486, 1584, 1672, 1786, 1875, 1995)
voda = (988, 1040, 1087, 1142, 1155, 1169, 1212, 1240, 1255, 1321, 1408, 1511, 1613, 1718, 1790, 1899, 1998)
pena = (470, 589, 673, 744, 816, 905, 1009, 1087, 1182, 1275, 1379, 1462, 1547, 1657, 1750, 1878, 1953)


majorLocator = MultipleLocator(2)
majorFormatter = FormatStrFormatter('%d')
minorLocator = MultipleLocator(1)



ind = np.arange(len(podlaha))  # the x locations for the groups
width = 0.2  # the width of the bars

fig, ax = plt.subplots()
# ax.grid()
rects1 = ax.bar(ind - width, podlaha, width, color='#D68F74', label='Podlaha')
rects2 = ax.bar(ind  , voda, width, color='#4E7189', label='Voda')
rects2 = ax.bar(ind + width, pena, width, color='#91C067', label='Pěna')

# Add some text for labels, title and custom x-axis tick labels, etc.

ax.set_ylabel('Referenční vzdálenost [mm]')
ax.set_xlabel('Změřená vzdálenost [mm]')
ax.set_xticks(ind)
ax.set_xticklabels((0, 400, 600, 800, 1000, 1200, 1400, 1600, 1800, 2000))
ax.legend()
# plt.setp(ax.get_xticklabels(), rotation=45, ha="right", rotation_mode="anchor")

ax.xaxis.set_major_locator(majorLocator)
# ax.xaxis.set_major_formatter(majorFormatter)

# for the minor ticks, use no labels; default NullFormatter
ax.xaxis.set_minor_locator(minorLocator)

#plt.show()
plt.savefig('../lidar_mes.pdf')

