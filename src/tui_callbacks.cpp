#include "tui_callbacks.hpp"
#include <sstream>

std::string Callbacks::info(Args args) {
    if(args.size() != 2) {
        return "Invalid format:\n\t info <position> e.g. info A3";
    }

    auto location = args[1];
    auto p = m_universe->getPlanetAt(location);

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
    std::ostringstream tmp;
    m_universe->nextRound();
    tmp << "Next Round #" << m_universe->getRoundCount();
    return tmp.str();
}

std::string Callbacks::send(Args args) {
    std::ostringstream tmp;
    if(args.size() != 4) {
        return "Invalid format:\n\t send <from> <to> <amount> e.g. send A3 D5 10";
    }

    auto src_pos = args[1];
    auto dst_pos = args[2];
    auto ships = std::atoi(args[3].c_str());

    auto src = m_universe->getPlanetAt(src_pos);

    if(src == nullptr) {
        return "No planet at " + src_pos;
    }

    if(src->ships < ships) {
        return "Not enough ships on the source planet";
    }

    auto* dst = m_universe->getPlanetAt(dst_pos);

    if(dst == nullptr) {
        return "No planet at " + dst_pos;
    }

    auto tr = m_universe->makeTransport(src, dst, ships);

    if(tr == nullptr) {
        return "Failed to create transport";
    }

    tmp << "Sent " << ships << " ships from " << src_pos << " to " << dst_pos << " ,distance: " << tr->distance;

    return tmp.str();
}

std::string Callbacks::help(Args args) {
    std::ostringstream tmp;
    tmp << "Commands:\n"
        << "info send next quit help\n"
        << "First letter only can be used, e.g i for info";
    return tmp.str();
}

std::string Callbacks::debug(Args args) {
    std::ostringstream tmp;
    for(auto& pl: m_universe->getPlanets()) {
        auto p = pl.second;
        tmp << p.name << " owner: "s << p.owner << " ships:"s << p.ships << " prod: "s << p.production << "\n";
    }
    for(auto& tr: m_universe->getTransports()) {
        tmp << "transport\n";
    }
    return tmp.str();
}

std::string Callbacks::reset(Args args) {
    m_universe->generatePlanets();
    return "Game restarted\n"s + m_universe->getInitMsg();
}

std::string Callbacks::save(Args args) {
    return "Not implemented"s;
}

std::string Callbacks::load(Args args) {
    return "Not implemented"s;
}
