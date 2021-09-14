#pragma once

#include "universe.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace std::string_literals;

class TUI {
    const std::string LETTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    Universe& m_universe;
    Config m_cfg;
    ushort m_xsize, m_ysize;

    public:
        TUI(Universe& universe);
        std::string makeHeader();
        std::vector<std::string> drawPlanets();
        void drawBoard();
};