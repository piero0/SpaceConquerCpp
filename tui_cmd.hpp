#pragma once
#include <string>
#include <vector>

using Args = std::vector<std::string>;
using Callback = void (*)(Args);

struct Command {
    std::string name = "???";
    Callback callback;
};

class TUICmds {
    std::vector<Command> m_commands;
    public:
        void parseCommandLine(std::string& cmd);
        bool match(std::string& cmd, Command& c);
        Args getArgs(std::string& cmd);
        void addCommand(Command& cmd);
};
