#pragma once
#include "universe.hpp"

class Callbacks;

using Args = std::vector<std::string>;
using Callback = std::string (Callbacks::*)(Args);
using namespace std::string_literals;

struct Command {
    std::string name = "???";
    Callback callback;
    std::string short_name = "?";

    Command(std::string n, Callback c, std::string s=""): name(n), callback(c), short_name(s) {}
};

class Callbacks {
    Universe* m_universe = nullptr;
    public:
        Callbacks(Universe* u): m_universe(u) {}

        std::string info(Args args);
        std::string quit(Args args);
        std::string next(Args args);
        std::string send(Args args);
        std::string save(Args args);
        std::string load(Args args);
        std::string newgame(Args args);
        std::string help(Args args);

        std::string debug(Args args);
        std::string reset(Args args);

        std::array<Command, 10> commands {
            Command("quit"s, &Callbacks::quit, "q"),
            Command("info"s, &Callbacks::info, "i"),
            Command("next"s, &Callbacks::next, "n"),
            Command("send"s, &Callbacks::send, "s"),
            Command("debug"s, &Callbacks::debug, "d"),
            Command("reset"s, &Callbacks::reset, "r"),
            Command("help"s, &Callbacks::help, "?"),
            Command("save"s, &Callbacks::save),
            Command("load"s, &Callbacks::load),
            Command("newgame"s, &Callbacks::newgame)
        };
};
