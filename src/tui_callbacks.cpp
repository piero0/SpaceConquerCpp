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
    tmp << m_universe->positionToString(p->position) << " owner: "s << static_cast<ushort>(p->owner) << " ships:"s << p->ships << " prod: "s << p->production;
    return tmp.str();
}

std::string Callbacks::quit(Args) {
    m_universe->setPlaying(false);
    return "Good bye"s;
}

std::string Callbacks::next(Args) {
    std::ostringstream tmp;
    m_universe->nextRound();
    tmp << "Round #" << m_universe->getRoundCount();
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

    tmp << "Sent " << ships << " ships from " << src_pos << " -> " << dst_pos << ", distance: " << tr->distance;

    return tmp.str();
}

std::string Callbacks::help(Args) {
    std::ostringstream tmp;
    tmp << "Commands:\n"
        << "info send next quit help\n"
        << "First letter only can be used, e.g i for info";
    return tmp.str();
}

std::string Callbacks::debug(Args) {
    std::ostringstream tmp;

    auto cfg = m_universe->getConfig();
    tmp << "Map size " << cfg->xsize << "x" << cfg->ysize << " planets: " << cfg->planets_num << "\n";
    tmp << "Round #" << m_universe->getRoundCount() << "\n";

    tmp << "Planets:\n";
    for(auto& pl: m_universe->getPlanets()) {
        auto p = pl.second;
        tmp << p.name << " owner: "s << static_cast<ushort>(p.owner) << " ships:"s << p.ships << " prod: "s << p.production << "\n";
    }
    tmp << "Transports:\n";
    for(auto& tr: m_universe->getTransports()) {
        tmp << tr.destination->name << " dist: " << tr.distance << " ships: " << tr.ships << "\n";
    }

    tmp << "Events count " << m_universe->getEvents().size() << "\n";
    return tmp.str();
}

std::string Callbacks::reset(Args) {
    m_universe->generatePlanets();
    return "Game restarted\n"s + m_universe->getInitMsg();
}

std::string Callbacks::save(Args args) {
    if(args.size() != 2) {
        return "Invalid format:\n\t save <name> e.g. save mygame";
    }

    auto filename = args[1] + ".save"s;
    Saver s;
    auto save_ok = s.save(m_universe, filename);

    if(!save_ok) {
        return "Could not save the game"s;
    }

    return "Game saved as "s + filename;
}

std::string Callbacks::load(Args args) {
    if(args.size() != 2) {
        return "Invalid format:\n\t load <name> e.g. load mygame";
    }

    auto filename = args[1] + ".save"s;
    Saver s;
    auto load_ok = s.load(m_universe, filename);

    if(!load_ok) {
        return "Could not load the game"s;
    }

    return "Game loaded"s;
}

std::string Callbacks::newgame(Args args) {
    std::string msg;
    ushort x = 10, y = 10, pnum = 10;

    switch(args.size()) {
        case 1:
            break; //use defaults
        case 3:
            x = std::atoi(args[1].c_str());
            y = std::atoi(args[2].c_str());
            break;
        case 4:
            x = std::atoi(args[1].c_str());
            y = std::atoi(args[2].c_str());
            pnum = std::atoi(args[3].c_str());
            break;
        default:
            return "Invalid format: newgame XSIZE YSIZE [PLANETS]";
            break;
    };

    const ushort MAX_SIZE = m_universe->getMaxSize();

    if(x < 1 || x > MAX_SIZE || y < 1 || y > MAX_SIZE) {
        return "Invalid map size: 1 <= x,y << " + std::to_string(MAX_SIZE);
    }

    if(pnum > x*y*0.8 || pnum < 3) {
        return "Invalid number of planets: 3 < planets < 80% of x*y";
    }

    auto cfg = m_universe->getConfig();
    cfg->planets_num = pnum;
    cfg->xsize = x;
    cfg->ysize = y;

    m_universe->generatePlanets();

    return m_universe->getInitMsg();
}

std::string Callbacks::planets(Args) {
    std::ostringstream tmp;
    auto planets = m_universe->getUserPlanets(Owner::YOU);
    for(auto& p: planets) {
        tmp << p.name << " ships:"s << p.ships << " prod: "s << p.production << "\n";
    }
    return tmp.str();
}

std::string Callbacks::transports(Args) {
    std::ostringstream tmp;
    for(auto& t: m_universe->getTransports()) {
        if(t.source->owner == Owner::YOU) {
            tmp << t.source->name << " -> " << t.destination->name << " ships: " << t.ships << " dist: " << t.distance << "\n";
        }
    }
    return tmp.str();
}
