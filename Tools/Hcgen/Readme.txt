
Utilita pro generovani struktur ze souboru konstant

features:
	vytvoreni vhodne struktury dat
	vygenerovani parseru:
		pokud je to mozne, jen hash klicu
		pokud ne, dalsi rozliseni, prvni pismeno, delka retezce, v
			nejhorsim pripade cele stringy

	staveni defaultnich promenych podle generovaneho souboru
	pokud ruzne neslucitelne typy , vygenerovat universal value
	jedna strukura jako universalni hodnoty
	mody:
		const 	- bez parseru, jen konstanty
		strict	- parser vyzaduje presny typ, nelze rozsirit strukturu , s vyjimkou cisel, ktere lze konvertovat,
				
		universal - hodnoty jsou universal stringy, s vyjimkou nastaveni typu struktury
		traditional - podobne jako strict, sporne hodnoty jsou jako universal, jina konstanta muze rozsirit strukturu
		

