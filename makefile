make:
	@rm -f src/log.log
	@g++ -o todo src/main.cpp -lncurses -g -Wno-return-type
	@mv todo src/

install:
	@mkdir -p ~/.config/C-TODO
	@touch ~/.config/C-TODO/config
	@sudo mv ~/C-TODO/src/todo /usr/bin/
	@mv ~/C-TODO ~/.C-TODO

