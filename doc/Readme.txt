Hoe Engine
--------------------------

_HOE_OPENGL_ - OpenGL
_HOE_D3D9_   - DIRECT3D9
_HOE_D3D8_   - DIRECT3D8

_WIN32	     - System na windowsech
_LINUX	     - LINUX
_XBOX		 - Xbox

_DEBUG	     - Debug veci

config
--------------------------
config.h config.cpp config.asm

- nacitani ze souboru
- autodetekce
  - MMX,MMX 2
  - CPU speed
  - SSE,SSE2,3DNow
- pres text a consoli
- seznamy funkci pro updaty...
- kontakt na ref (zjistovani stavu graficky karty...)
- podle potreby nastavit states...

states
-------------------------
states.h states.cpp states_d3d9.cpp states_opengl.cpp
- uzka spoluprace s konfigem..
- konfigurace pres konfig
- jenom udrzeni renderovacich stavu...

ref
--------------------------
- OpenGL nebo DIRECT3D funkce...
- chybi podpora vic adapteru - doplnit

math
---------------------------
funkce zatim oznacovat jako _hoe_<func>maji vlastni namespace HoeMath
v headru hoe3d_math.h

model
--------------------------
struct hfm_header
{
	int id;
	int ver;
};

model - pole int - ukonceny 0

s tim textem to neni spatnej napad......

setmaterial 1
setvertex shader 1
draw...

base tools (multiplatform)
-utils (vsechno mozny)
-time manager
-file system
-ref
-input
-sound support
-math
-input
-net



- engine
-- middle class
-- textures
-- materials
  2d
   models
   kamera
   light
   physics
   sound manager
   objects
   world
   
shadery
problem univerzalnosti mezi diRECT a opengl
extension

nez bude hotovo musi se osefovat model shader

//////////////
model fvf:
definovani streamu s daty
p - x,y,z pozice (3f)
r - rhw pozice (4f)
n - normala (3f)
d - diffuse (dw)
s - specular (dw)
2 - texture (2f)
4fxr
p - pozice, mod. x - fixed
n - normala mod x - fixed
sc - specular
c - diffuse


fixed3:position
float3:position

jazyk pro vytvareni resources
ruzne skripty pro soubory, nebo odkazy na soubory
// shader skript, data stream soubor, textura
// definice materialu
// definice 

ns
becher:podlaha
becher:vino:sklenice

v souboru, resource uzavrene podle namespace

:korenarka:strecha
:korenarka:barak
:korenarka:nevim

struct file
 id
 version
 flags?

// name
jmeno
 key -> value
 key -> value

//////////////

 

