
#include "StdAfx.h"
#include "../include/hoe_path.h"

BEGIN_HOEGAME
namespace AI {
namespace Path {
/*
potrebuje:
 ohodnocenou frontu
 flag closed
 heuristiku
 pocitaci funkci
 vracet sousedy
 funkce w (pomerna vzdalenost)

 g = vzdalenost od startu
 h = heuristika
 f = pomerovaci funkce g + h

AStar_algoritmus
   s.g = 0           // s je startovní políèko
   s.h = Heuristika(s)
   s.f = s.g + s.h
   s.pøedchozí = null
   Open.push s
   Dokud je Open neprázdná
         Open.pop n // n má nejmenší f
         pokud n je cílové políèko
               konstruuj cestu
               vra úspìch
         pro kadého souseda n' vrcholu n
               novág = n.g + w(n,n')
               pokud n' je v Open nebo Closed, a n'.g <= novág
                     zpracuj dalšího souseda
               n'.pøedchozí = n
               n'.g = novág
               n'.h = Heuristika(n')
               n'.f = n'.g + n'.h
               pokud je n' v Closed
                     vypus ho z Closed
               pokud n' ještì není v Open
                     Open.push n'
         Closed.push n
   vra neúspìch // cesta neexistuje
*/

} // namespace Path
} // namespace AI

END_HOEGAME


