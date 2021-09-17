#include "tui.hpp"

using namespace std::string_literals;

/*
  A B C D E F G H
8| | | | | | | | |8
7| | | | | | | | |7
6| | | | | | | | |6
5| | | | | | | | |5
4| | | | | |O| | |4
3| | | | | | | | |3
2| | | | | | | | |2
1| | | | | | | | |1
  A B C D E F G H
*/

TUI::TUI(Universe& universe, Callbacks* cbs):
    m_universe(universe), m_cmds(cbs) {
    m_cfg = universe.getConfig();
    m_xsize = m_cfg.xsize;
    m_ysize = m_cfg.ysize;
}

std::string TUI::makeHeader() {
    std::string header = "   "s;
    for(size_t x = 0; x < m_xsize; x++) {
        header += LETTERS[x] + " "s;
    }
    return header;
}

std::vector<std::string> TUI::drawPlanets() {
    std::string line = ""s;
    std::string head = makeHeader();

    std::vector<std::string> board;

    board.emplace_back(head);

    for (size_t y = m_ysize; y > 0; y--) {
        auto ystr = std::to_string(y);
        if(ystr.length() == 1) {
            ystr = " "s + ystr;
        }

        line = ystr + "|"s;
        for (size_t x = 0; x < m_xsize; x++) {
            Position pos {x+1, y};
            line += (m_universe.getPlanetByPos(pos) != nullptr) ? "o|"s : " |"s;                    
        }
        line += ystr;
        board.emplace_back(line);
    }

    board.emplace_back(head);

    return board;
}

void TUI::drawBoard() {
    auto planets = drawPlanets();

    std::cout << "\n";
    for(auto& line: planets) {
        std::cout << line << "\n";
    }
    std::cout << "\n";
}

void TUI::mainLoop() {
    std::string cmd {""s}, response {m_universe.getInitMsg()};
    while(m_universe.isPlaying()) {
        drawBoard();
        std::cout << response << "\n> ";
        std::getline(std::cin, cmd);
        response = m_cmds.parseCommandLine(cmd);
    }
}
