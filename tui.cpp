#include "univers.hpp"
#include <iostream>
#include <string>

using namespace std::string_literals;

class TUI {
    const std::string LETTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    ushort XSIZE = 10;
    ushort YSIZE = 10;

    public:
        TUI(ushort x, ushort y): XSIZE(x), YSIZE(y) {}

        std::string makeHeader() {
            std::string header = "   "s;
            for(size_t x = 0; x < XSIZE; x++) {
                header += LETTERS[x] + " "s;
            }
            return header;
        }

        void drawPlanets() {
            std::string line = ""s;
            std::string board = ""s;

            std::string head = makeHeader();

            board += head + "\n"s;

            for (size_t y = YSIZE; y > 0; y--) {
                auto ystr = std::to_string(y);
                if(ystr.length() == 1) {
                    ystr = " "s + ystr;
                }

                line = ystr + "|"s;
                for (size_t x = 0; x < XSIZE; x++) {
                    line += " |"s;
                }
                line += ystr + "\n"s;
                board += line;
            }

            board += head + "\n"s;

            board += "> command?\n";

            std::cout << board;
        }
};

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

int main(int argc, char** argv) {
    if(argc != 3) {
        std::cout << argv[0] << " x y\n";
        return 0;
    }

    auto t = TUI(std::atoi(argv[1]), std::atoi(argv[2]));
    t.drawPlanets();
}
