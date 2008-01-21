
#include "StdAfx.h"
#include "../include/HoeResource/fvf.h"

/*
 format fvf
 p - pozice, (<fp) <xp - fixed pozice 0123 3210
 r - rhw        -||-
 n - normala, (fn) xn - fixed
 t#1 - prvni, defaultne
 ttt - 3x textura po 2

 f3 - 3x float bez uziti

*/

/*
	{'f',4,1,'\0'},
	{'x',4,1,'\0'},
	{'i',4,1,'\0'},
	{'b',1,1,'\0'},
	{'w',2,1,'\0'},
	{'1',4,1,'f'},
	{'2',8,2,'f'},
	{'3',12,3,'f'},
	{'4',16,4,'f'},
	{'c',4,1,'x'},
	{'p',12,3,'3'},
	{'r',16,4,'4'},
	{'n',12,3,'3'},
	{'d',4,1,'c'},
	{'s',4,1,'c'},
	{'t',8,2,'2'} 
*/ 


