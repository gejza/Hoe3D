
# vytvareni tabulek znaku
# tabulka mapovani velkych pismen na mala
# 

import re

a = file('CaseFolding.txt')

c = re.compile('([0-9A-F]+); (.); ([^;]*); #([^\n]*)')
mp = re.compile('[0-9A-F]+')

chs = []
numcapital = 0
numsmall = 0
latin = 0
greek = 0
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
		if ch["comment"].find("LATIN") != -1:
			latin = latin + 1
		elif ch["comment"].find("GREEK") != -1:
			greek = greek + 1
		elif ch["comment"].find("GLAGOLITIC") != -1:
			pass
		elif ch["comment"].find("COPTIC") != -1:
			pass
		elif ch["comment"].find("CYRILLIC") != -1:
			pass
		elif ch["comment"].find("DESERET") != -1:
			pass
		elif ch["comment"].find("GEORGIAN") != -1:
			pass
		elif ch["comment"].find("ARMENIAN") != -1:
			pass
		elif ch["comment"].find("ROMAN") != -1:
			pass
		else:
			print ch
			
		if ch["comment"].find("CAPITAL") != -1:
			numcapital = numcapital + 1
		elif ch["comment"].find("SMALL") != -1:
			numsmall = numsmall + 1
			
		if ch["comment"].find("LETTER") != -1:
			pass
		elif ch["comment"].find("NUMERAL") != -1:
			pass
		elif ch["comment"].find("SIGN") != -1:
			pass
		elif ch["comment"].find("SYMBOL") != -1:
			pass
		else:
			pass

print len(chs)
print numcapital
print numsmall
print latin
print greek

