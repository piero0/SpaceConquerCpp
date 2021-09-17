#include "tui_cmd.hpp"

TUICmds::TUICmds(Callbacks* cb): m_callbacks(cb) {
    addCommands(m_callbacks->commands.begin(), m_callbacks->commands.end());
}

bool TUICmds::match(std::string& cmd, Command& c) {
    if(cmd.length() == 0) return false;

    if(cmd.substr(0, c.short_name.length()) == c.short_name || cmd.substr(0, c.name.length()) == c.name) {
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

std::string TUICmds::parseCommandLine(std::string& cmd) {
    auto response {""s};
    for(auto& c: m_commands) {
        auto args = getArgs(cmd);
        if(match(args[0], c)) {
            response = std::invoke(c.callback, m_callbacks, args);
            break;
        }
    }
    return response;
}

void TUICmds::addCommand(Command& cmd) {
    m_commands.push_back(cmd);
}

template<class Iter> void TUICmds::addCommands(Iter begin, Iter end) {
    while(begin != end) {
        m_commands.push_back(*begin++);
    }
}
