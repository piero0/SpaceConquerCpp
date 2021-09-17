#include "tui.hpp"

int main() {
    Config cfg;
    Universe u(cfg);
    Callbacks cbs(&u);
    TUI tui(u, &cbs);

    tui.mainLoop();

    return 0;
}
