#include "tui_cmd.hpp"

bool TUICmds::match(std::string& cmd, Command& c) {
    if(cmd.length() == 0) return false;

    if(cmd[0] == c.name[0] || cmd.substr(0, c.name.length()) == c.name) {
        return true;
    }
    
    return false;
}

Args TUICmds::getArgs(std::string& cmd) {
    Args args;
    size_t epos = 0, spos = 0;

    while(epos != std::string::npos) {
        epos = cmd.find(' ', spos);
        args.push_back(cmd.substr(spos, epos-spos));
        spos = epos + 1;
    }

    return args;
}

void TUICmds::parseCommandLine(std::string& cmd) {
    for(auto& c: m_commands) {
        auto args = getArgs(cmd);
        if(match(args[0], c)) {
            c.callback(args);
        }
    }
}

void TUICmds::addCommand(Command& cmd) {
    m_commands.push_back(cmd);
}
