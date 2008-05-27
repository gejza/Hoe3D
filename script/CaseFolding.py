
import re

a = file('CaseFolding.txt')

c = re.compile('([0-9A-F]+); (.); ([^;]*); #([^\n]*)')
mp = re.compile('[0-9A-F]+')

chs = []
for t in a.readlines():
	m = c.match(t)
	if m:
		# 
		ch = {}
		ch["code"] = m.group(1)
		ch["type"] = m.group(2)
		ch["comment"] = m.group(4)
		ch["map"] = mp.findall(m.group(3))
		chs.append(ch)
# ted to chce nejak propocitat tu unicode tabuli
# pres hash?
# 
print len(chs)

