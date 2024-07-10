make:
	echo " " > log.log
	rm log.log
	g++ -o todo main.cpp -lncurses -g -Wno-return-type 
	./todo

lines:
	mv toml.hpp ..
	mv todo ..
	wc -l *
	mv ../todo .
	mv ../toml.hpp .

