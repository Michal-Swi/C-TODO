make:
	echo " " > log.log
	rm log.log
	g++ -o TODO main.cpp -lncurses -g -Wno-return-type 
	./TODO

lines:
	mv toml.hpp ..
	mv TODO ..
	wc -l *
	mv ../TODO .
	mv ../toml.hpp .

