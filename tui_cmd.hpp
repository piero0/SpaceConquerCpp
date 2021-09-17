#pragma once
#include <string>
#include <vector>
#include <functional>
#include "tui_callbacks.hpp"

class TUICmds {
    std::vector<Command> m_commands;
    Callbacks* m_callbacks = nullptr;

    bool match(std::string& cmd, Command& c);
    Args getArgs(std::string& cmd);

    public:
        TUICmds(Callbacks* cb);
        std::string parseCommandLine(std::string& cmd);
        void addCommand(Command& cmd);
        template<class Iter> void addCommands(Iter begin, Iter end);
};
