#include "tui.hpp"

int main() {
    auto log = spdlog::basic_logger_st("app", "debug.log");
    spdlog::set_default_logger(log);
    spdlog::set_level(spdlog::level::debug);

    Config cfg;
    Universe u(cfg);
    Callbacks cbs(&u);
    TUI tui(u, &cbs);

    tui.mainLoop();

    return 0;
}
