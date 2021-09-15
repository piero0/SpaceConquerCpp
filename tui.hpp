#pragma once

#include "universe.hpp"
#include "tui_cmd.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace std::string_literals;

class TUI {
    const std::string LETTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    Universe& m_universe;
    Config m_cfg;
    ushort m_xsize, m_ysize;
    bool m_playing = true;

    public:
        TUI(Universe& universe);
        std::string makeHeader();
        std::vector<std::string> drawPlanets();
        void drawBoard();
        Position convertToPosition(const std::string& location);
        void mainLoop();
        void processCommand(std::string& cmd);
};
