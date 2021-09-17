#pragma once
#include <string>
#include <vector>
#include <functional>
#include "tui_callbacks.hpp"

class TUICmds {
    std::vector<Command> m_commands;
    std::unordered_map<std::string, size_t> m_long_names;
    std::unordered_map<std::string, size_t> m_short_names;

    Callbacks* m_callbacks = nullptr;

    Command* findCommand(std::string& cmd);
    Args splitArgs(std::string& cmd);

    public:
        TUICmds(Callbacks* cb);
        std::string parseCommandLine(std::string& cmdline);
        template<class Iter> void addCommands(Iter begin, Iter end);
};
