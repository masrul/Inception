/**
 * Author      : masrulhuda (masrulhuda@Masruls-MBP)
 * Host        : Masruls-MBP
 * Created     : Tuesday Apr 04, 2023 22:10:17 CDT
 */

#include "progressbar.hpp"

int main() {
    progressbar bar(100);
    bar.set_todo_char(" ");
    bar.set_done_char("█");
    bar.set_opening_bracket_char("{");
    bar.set_closing_bracket_char("}");
    for (int i = 0; i < 100; ++i) {
        bar.update();
        // ... the program
    }
    return 0;
}

