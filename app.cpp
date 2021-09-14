#include "tui.hpp"

int main() {
    Config cfg;
    Universe u(cfg);
    u.generatePlanets();
    TUI t(u);

    t.drawBoard();

    return 0;
}