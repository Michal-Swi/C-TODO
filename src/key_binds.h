struct AddNewHeaderBind {
	static char above;
	static char below;
	static char here;
};

char AddNewHeaderBind::above = 'a';
char AddNewHeaderBind::below = 'b';
char AddNewHeaderBind::here = 'h';

struct ChangeCompletionLevelBind {
	static char up;
	static char down;
};

char ChangeCompletionLevelBind::up = 'u';
char ChangeCompletionLevelBind::down = 'd'; 

struct ExitBind {
	static char force;
	static char save;
};
char ExitBind::force = 'f';
char ExitBind::save = 's';

