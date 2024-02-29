/* THIS HEADER SHOULD BE USED *
 * ONLY TO DRAW ON SCREEN NO  *
 * MATTER WHAT, IT SHOULD BE  *
 * IN HERE NOT IN ANY OTHER   *
 *			FILE              *
 */                            

#include <ncurses.h>
#include "getTasks.h"

void drawScreen() {
	// Vector and string libaries are already included in getTasks.h.
	std::vector<std::string> tasks = getTasks();
	for (int i = 0; i < tasks.size(); i++) {
		const char *task = tasks[i].c_str();
		mvprintw(i, 0, task); 
	}
}
