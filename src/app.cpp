#include "tui.hpp"

int main() {
    Config cfg;
    Universe u(cfg);
    u.generatePlanets();

    Callbacks cbs(&u);
    TUI t(u, &cbs);

    t.mainLoop();

    return 0;
}
