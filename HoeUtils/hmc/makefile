
all: hmc.cpp parser.c classes.cpp
	g++ -o hmc main.cpp hmc.cpp parser.c classes.cpp cparser.cpp mshader.cpp fvf.cpp ../utils.cpp -lfl

parser.cpp: parser.l
	flex -d -oparser.c parser.l 

clean:
	rm parser.cpp
	rm hmc
