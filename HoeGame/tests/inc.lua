-- the first program in every language

print("Luaded Include file 'inc.lua'")

function demo()
  print("Start demo")
  sleep()
  print("sl 1.")
  sleep()
  print("sl 2")
  sleep()
  print("sl 3")
  sleep()
  print("sl 4")
  sleep()
  print("End demo")

  for a=1,19,1 do
    for b=1,98,1 do
	  c = 100 - a - b
	  sum = a*50+b*7+c*3
	  if (sum == 1000) then
	   print("a=",a," b=",b," c=",c)
	  end
	end
  end
end










