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
        reloadConfig();
}

void TUI::reloadConfig() {
    m_cfg = m_universe.getConfig();
    m_xsize = m_cfg->xsize;
    m_ysize = m_cfg->ysize;
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

std::vector<std::string> TUI::drawEventLog() {
    std::vector<std::string> log;
    log.push_back("Event Log:");

    size_t lines = m_cfg->ysize + 1;
    auto& events = m_universe.getEvents();
    auto it = events.end();

    if(events.size() >= lines) {
        std::advance(it, -lines);
    } else {
        it = events.begin();
    }

    while(it != events.end()) {
        log.push_back(it->message);
        it++;
    }

    return log;
}

void TUI::drawBoard() {
    auto planets = drawPlanets();
    auto log = drawEventLog();

    std::string left,right;

    std::cout << "\n";

    for(auto p=planets.begin(), l=log.begin(); p != planets.end() || l != log.end();) {
        left = (p != planets.end()) ? *p++ : ""s;
        right = (l != log.end()) ? *l++ : ""s;
        std::cout << left << "\t\t" << right << "\n";
    }

    std::cout << "\n";
}

void TUI::mainLoop() {
    std::string cmd {""s}, response {m_universe.getInitMsg()};
    while(m_universe.isPlaying()) {
        reloadConfig();
        drawBoard();
        std::cout << response << "\n> ";
        std::getline(std::cin, cmd);
        response = m_cmds.parseCommandLine(cmd);
    }
}
