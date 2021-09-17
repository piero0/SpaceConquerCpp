#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <iterator>

#include "universe.hpp"
#include "tui_cmd.hpp"
#include "tui_callbacks.hpp"

using namespace std::string_literals;

class TUI {
    const std::string LETTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    Universe& m_universe;
    TUICmds m_cmds;
    Config* m_cfg;
    ushort m_xsize, m_ysize;
    bool m_playing = true;

    std::string makeHeader();
    std::vector<std::string> drawPlanets();
    std::vector<std::string> drawEventLog();

    void drawBoard();
    void reloadConfig();

    public:
        TUI(Universe& universe, Callbacks* cbs);
        void mainLoop();
};
