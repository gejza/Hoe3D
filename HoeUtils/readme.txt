lib - hlavni knihovna lib utils
htex - utilita pro vytvareni texturovych souboru
hmc - utilita pto vytvareni modelovych souboru

format


namespace becher

  index
   45,56,67,78
  ~index

  stream aaa
  ~stream

  model
   
   submodel aaa
   
   
  ~model

  material aaa
   texture = name
   color = { aaa}
   diffuse = blue
   specular = d
  ~material

  texture becher:nic "aaa.jpg" ~
 
~namespace
