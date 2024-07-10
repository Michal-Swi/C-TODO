make:
	@mkdir -p ~/.config/C-TODO
	@touch ~/.config/C-TODO/config
	@sudo mv ~/C-TODO/src/todo /usr/bin/
	@mv ~/C-TODO ~/.C-TODO

