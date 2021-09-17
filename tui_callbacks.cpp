#include "tui_callbacks.hpp"
#include <sstream>

std::string Callbacks::info(Args args) {
    auto location = args[1];
    auto pos = m_universe->convertToPosition(location);
    auto p = m_universe->getPlanetByPos(pos);

    if(p == nullptr) {
        return "No planet found at "s + location;
    }
    
    //show ships and production only to owner
    std::ostringstream tmp; 
    tmp << p->name << " owner: "s << p->owner << " ships:"s << p->ships << " prod: "s << p->production;
    return tmp.str();
}

std::string Callbacks::quit(Args args) {
    m_universe->setPlaying(false);
    return ""s;
}

std::string Callbacks::next(Args args) {
    m_universe->nextRound();
    return "Next Round"s;
}

std::string Callbacks::send(Args args) {
    return "Send not implemented yet"s;
}
