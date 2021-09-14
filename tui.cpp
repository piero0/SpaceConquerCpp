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


TUI::TUI(Universe& universe):m_universe(universe) {
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
            Position pos {x, y};
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

    for(auto& line: planets) {
        std::cout << line << "\n";
    }
}

Position TUI::convertToPosition(const std::string& location) {
    //Format should be column (one letter), row (a number)
    //e.g. A3, G10

    Position pos {std::numeric_limits<ushort>::max(), std::numeric_limits<ushort>::max()};

    auto col = std::toupper(location[0]);
    if(col >= 'A' && col <= 'Z') {
        pos.first = static_cast<ushort>(col - 'A');
    }

    auto row = std::atoi(location.substr(1).c_str());

    if(std::to_string(row) == location.substr(1)) {
        pos.second = row;
    }

    return pos;
}

void TUI::processCommand(std::string& cmd) {
    if(cmd == "quit") {
        m_playing = false;
    } else if(cmd.substr(0, 4) == "info") {
        auto len = cmd.length();
        if(len < 7) {
            std::cout << "missing planet's position e.g. info A1\n";
            return;
        }

        auto loc = cmd.substr(5, cmd.length() - 5);
        auto pos = convertToPosition(loc);
        auto p = m_universe.getPlanetByPos(pos);
        if(p == nullptr) {
            std::cout << "No planet found at " << loc << "\n";
            return;
        }
        
        //show ships and production only to owner
        std::cout << p->name << " owner: " << p->owner << " ships:" << p->ships << " prod: " << p->production << "\n";
    } else if(cmd == "next") {
        m_universe.nextRound();
    }
}

void TUI::mainLoop() {
    std::string cmd {""s};
    while(m_playing) {
        drawBoard();
        std::cout << "> ";
        std::getline(std::cin, cmd);
        processCommand(cmd);
    }
}
