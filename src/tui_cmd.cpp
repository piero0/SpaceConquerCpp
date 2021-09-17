#include "tui_cmd.hpp"

TUICmds::TUICmds(Callbacks* cb): m_callbacks(cb) {
    addCommands(m_callbacks->commands.begin(), m_callbacks->commands.end());
}

Command* TUICmds::findCommand(std::string& cmd) {
    auto idx = m_long_names.find(cmd);

    if(idx != m_long_names.end()) {
        return &m_commands[idx->second];
    }

    idx = m_short_names.find(cmd);
    if(idx != m_short_names.end()) {
        return &m_commands[idx->second];
    }

    return nullptr;
}

Args TUICmds::splitArgs(std::string& cmd) {
    Args args;
    size_t epos = 0, spos = 0;

    while(epos != std::string::npos) {
        epos = cmd.find(' ', spos);
        args.push_back(cmd.substr(spos, epos-spos));
        spos = epos + 1;
    }

    return args;
}

std::string TUICmds::parseCommandLine(std::string& cmdline) {
    auto response {""s};

    auto args = splitArgs(cmdline);
    auto cmd = findCommand(args[0]);
    if(cmd != nullptr) {
        response = std::invoke(cmd->callback, m_callbacks, args);
    }

    return response;
}

template<class Iter> void TUICmds::addCommands(Iter begin, Iter end) {
    size_t idx = 0;
    while(begin != end) {
        m_commands.push_back(*begin);
        m_long_names[begin->name] = idx;

        if(begin->short_name != "") {
            m_short_names[begin->short_name] = idx;
        }

        begin++;
        idx++;
    }
}
