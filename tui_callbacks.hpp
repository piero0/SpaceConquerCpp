#pragma once
#include "universe.hpp"

class Callbacks;

using Args = std::vector<std::string>;
using Callback = std::string (Callbacks::*)(Args);
using namespace std::string_literals;

struct Command {
    std::string name = "???";
    Callback callback;

    Command(std::string n, Callback c): name(n), callback(c) {}
};

class Callbacks {
    Universe* m_universe = nullptr;
    public:
        Callbacks(Universe* u): m_universe(u) {}

        std::string info(Args args);
        std::string quit(Args args);
        std::string next(Args args);
        std::string send(Args args);

        std::array<Command, 4> commands {
            Command("quit"s, &Callbacks::quit),
            Command("info"s, &Callbacks::info),
            Command("next"s, &Callbacks::next),
            Command("send"s, &Callbacks::send)
        };
};
