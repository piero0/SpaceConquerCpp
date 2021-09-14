#include "tui.hpp"

int main() {
    Config cfg;
    Universe u(cfg);
    u.generatePlanets();
    TUI t(u);

    t.mainLoop();

    return 0;
}