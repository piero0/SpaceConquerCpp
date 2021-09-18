#include "saver.hpp"

bool Saver::save(Universe* u, std::string filename) {
    std::ofstream of(filename, std::ios_base::binary);

    if(!of.is_open()) {
        return false;
    }

    auto cfg = u->getConfig();
    
    of << "#config\n";
    of << cfg->xsize << "," << cfg->ysize << "," << cfg->planets_num << "," << cfg->production_min << "," << cfg->production_max << "\n";

    of << "#planets\n";
    for(auto& p: u->getPlanets()) {
        of << p.first.first << "," << p.first.second << "," << static_cast<int>(p.second.owner) << ","
            << p.second.name << "," << p.second.production << "," << p.second.ships << "," << p.second.ships_in_production << "\n";
    }

    of << "#transports\n";
    for(auto& t: u->getTransports()) {
        of << t.source->name << "," << t.destination->name << ","
            << t.distance << "," << t.ships << "\n";
    }

    of << "#events\n";
    for(auto& e: u->getEvents()) {
        of << static_cast<int>(e.type) << "," << e.message << "\n";
    }

    of << "#state\n";
    of << u->getRoundCount() << "," << u->isFreePlay() << "\n";

    of << "#end\n";
    of.close();

    return true;
}

bool Saver::load(Universe* u, std::string filename) {
    auto sec = Section::START;
    m_u = u;

    std::ifstream sf(filename, std::ios_base::binary);

    if(!sf.is_open()) {
        return false;
    }

    for(std::string line; std::getline(sf, line);) {
        if(line[0] == '#') {
            if(line == "#config") {sec = Section::CONFIG; }
            else if(line == "#planets") {sec = Section::PLANETS; }
            else if(line == "#transports") {sec = Section::TRANSPORTS; }
            else if(line == "#events") {sec = Section::EVENTS; }
            else if(line == "#state") {sec = Section::STATE; }
            else if(line == "#end") {sec = Section::END; }
            continue;
        }

        switch(sec) {
        case Section::CONFIG:
            loadConfig(line); break;
        case Section::PLANETS:
            loadPlanet(line); break;
        case Section::TRANSPORTS:
            loadTransport(line); break;
        case Section::EVENTS:
            loadEvent(line); break;
        case Section::STATE:
            loadState(line); break;
        default:
            continue;
            break;
        }
    }

    sf.close();

    return true;
}

void Saver::loadPlanet(std::string line) {
    auto& planet_vec = m_u->getPlanets();
    auto args = splitArgs(line);
    
    Planet p;
    p.position = {std::atoi(args[0].c_str()), std::atoi(args[1].c_str())};
    p.owner = static_cast<Owner>(std::atoi(args[2].c_str()));
    p.name = args[3];
    p.production = std::atoi(args[4].c_str());
    p.ships = std::atoi(args[5].c_str());
    p.ships_in_production = std::atof(args[6].c_str());

    planet_vec[p.position] = p;
}

void Saver::loadConfig(std::string line) {
    auto cfg = m_u->getConfig();
    auto args = splitArgs(line);
    cfg->xsize = std::atoi(args[0].c_str());
    cfg->ysize = std::atoi(args[1].c_str());
    cfg->planets_num = std::atoi(args[2].c_str());
    cfg->production_min = std::atof(args[3].c_str());
    cfg->production_max = std::atof(args[4].c_str());
}

void Saver::loadTransport(std::string line) {
    auto& t = m_u->getTransports();
    auto args = splitArgs(line);

    Transport tr;
    tr.source = m_u->getPlanetAt(args[0]);
    tr.destination = m_u->getPlanetAt(args[1]);
    tr.distance = std::atoi(args[2].c_str());
    tr.ships = std::atoi(args[3].c_str());

    t.push_back(tr);
}

void Saver::loadEvent(std::string line) {
    auto args = splitArgs(line);
    auto& evt = m_u->getEvents();

    Event e;
    e.type = static_cast<Event::Type>(std::atoi(args[0].c_str()));
    e.message = args[1];
    
    evt.push_back(e);
}

void Saver::loadState(std::string line) {
    auto args = splitArgs(line);
    m_u->setRoundCount(std::atoi(args[0].c_str()));
    m_u->setFreePlay((args[1] == "1") ? true : false);
}

std::vector<std::string> Saver::splitArgs(std::string& line, size_t max_splits) {
    std::vector<std::string> args;
    size_t epos = 0, spos = 0, splits = 0;

    while(epos != std::string::npos) {
        epos = line.find(',', spos);
        args.push_back(line.substr(spos, epos-spos));
        spos = epos + 1;

        splits++;
        if(max_splits > 0 && splits == max_splits + 1) break;
    }

    return args;
}
