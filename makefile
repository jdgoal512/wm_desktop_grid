build:
	g++ -o switch_workspace *.cpp *.h json/json.h

install:
	cp switch_workspace /usr/bin
