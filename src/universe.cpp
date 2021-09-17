#include "universe.hpp"

void Planet::nextRound() {
    ships_in_production += production;
    ships = static_cast<ushort>(std::floor(ships_in_production));
}

bool Planet::sendShips(ushort ships_num) {
    if(ships_num > ships) return false;

    ships -= ships_num;
    ships_in_production -= static_cast<float>(ships_num);

    return true;
}

Universe::Universe(const Config& cfg) {
        m_cfg = cfg;
}

ushort Universe::calcDistance(Planet* src, Planet* dst) {
    auto x2 = std::abs(src->position.first - dst->position.first);
    auto y2 = std::abs(src->position.second - dst->position.second);
    return static_cast<ushort>(std::ceil(std::sqrt(x2*x2 + y2*y2)));
}

Transport* Universe::makeTransport(Planet* src, Planet* dst, ushort ships) {
    // sprawdzic emplace_back ? czy potrzebuje konstruktor
    if(!src->sendShips(ships)) {
        return nullptr;
    }
    m_transports.push_back({calcDistance(src, dst), ships, dst, src});
    return &m_transports.back();
}

Planet* Universe::getPlanetByPos(const Position& pos) {
    auto it = m_planets.find(pos);
    return (it != m_planets.end()) ? &it->second: nullptr;
}

Planet* Universe::getPlanetAt(const std::string& loc) {
    return getPlanetByPos(convertToPosition(loc));
}

Position Universe::generatePosition(std::uniform_int_distribution<>& gx, std::uniform_int_distribution<>& gy, std::mt19937& gen) {
    Position pos {gx(gen), gy(gen)};
    for(ushort maxTry=0; m_planets.find(pos) != m_planets.end() || maxTry > 5; pos.first = gx(gen), pos.second = gy(gen), maxTry++);
    // moze zwrocic juz istniejaca lokalizacje
    return pos;
}

void Universe::assignPlayers() {
    size_t idx = 0;
    ushort ids[] = {1, 2}; // na razie tylko dwoch playerow 1 - user, 2 - cpu

    for(auto& p: m_planets) {
        p.second.owner = ids[idx++];
        p.second.production = 0.5f;
        p.second.ships = 10;
        p.second.ships_in_production = 10.0f;

        if(idx == 2) break;
    }
}

std::vector<Planet> Universe::getUserPlanets(ushort user_id) {
    std::vector<Planet> user_planets;
    for(auto& p: m_planets) {
        user_planets.push_back(p.second);
    }
    return user_planets;
}

void Universe::reset() {
    m_planets.clear();
    m_transports.clear();
    m_events.clear();
    m_round = 1;
    m_playing = true;
}

void Universe::generatePlanets() {
    reset();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> genx(1, m_cfg.xsize);
    std::uniform_int_distribution<> geny(1, m_cfg.ysize);
    std::uniform_real_distribution<> genprod(m_cfg.production_min, m_cfg.production_max);

    for (ushort i = 0; i < m_cfg.planets_num; i++) {
        Planet p;
        p.production = genprod(gen);
        p.position = generatePosition(genx, geny, gen);
        p.name = std::to_string(p.position.first) + "x" + std::to_string(p.position.second);

        m_planets[p.position] = p;
    }

    assignPlayers();
}

void Universe::land(Transport& t) { //jakas lepsza nazwa
    if(t.source == t.destination) {
        t.destination->ships += t.ships;

        m_events.push_back(Event(Event::Type::REINFORCEMENTS, &t));
        return;
    }

    ushort dif = t.ships - t.destination->ships;
    if(dif > 0) {
        t.destination->owner = t.source->owner;
        t.destination->ships = dif;

        m_events.push_back(Event(Event::Type::ATTACK_OK, &t));
    } else {
        m_events.push_back(Event(Event::Type::ATTACK_FAIL, &t));
    }
}

void Universe::nextRound() {
    for(auto& t: m_transports) {
        t.distance -= 1;
        if(t.distance <= 0) {
            land(t);
        }
    }

    std::remove_if(m_transports.begin(), m_transports.end(), [](Transport& t){ return t.distance <= 0; });

    for(auto& p: m_planets) {
        p.second.nextRound();
    }

    checkGameEnded();

    m_round++;
}

Event::Event(Event::Type type, Transport* t) {
    switch(type) {
    case Event::Type::REINFORCEMENTS:
        message = std::to_string(t->ships) + " ships arrived on " + t->destination->name;
        break;
    case Event::Type::ATTACK_OK:
        message = "Planet " + t->destination->name + " taken over";
        break;
    case Event::Type::ATTACK_FAIL:
        message = "Attack on " + t->destination->name + " failed";
        break;
    case Event::Type::WIN:
        message = "Congratulation, you won!";
        break;
    case Event::Type::DOMINATION:
        message = "You have defated all opponents but there are still some planets to conquer";
        break;
    case Event::Type::DEFEAT:
        message = "You have been defeated";
        break;
    
    default:
        break;
    }
}

Position Universe::convertToPosition(const std::string& location) {
    //Format should be column (one letter), row (a number)
    //e.g. A3, G10

    Position pos {std::numeric_limits<ushort>::max(), std::numeric_limits<ushort>::max()};

    auto col = std::toupper(location[0]);
    if(col >= 'A' && col <= 'Z') {
        pos.first = static_cast<ushort>(col - 'A' + 1);
    }

    auto row = std::atoi(location.substr(1).c_str());

    if(std::to_string(row) == location.substr(1)) {
        pos.second = row;
    }

    return pos;
}

std::string Universe::positionToString(const Position& pos) {
    std::ostringstream msg;
    msg << static_cast<char>('A' + pos.first - 1) << std::to_string(pos.second);
    return msg.str();
}

std::string Universe::getInitMsg() {
    std::ostringstream msg;
    auto user_planets = getUserPlanets(1);
    if(user_planets.size() == 0) {
        msg << "Type 'newgame' to start a new game or 'load' to load saved one";
    } else {
        auto start_pos = positionToString(user_planets[0].position);
        msg << "Round #1\nWelcome Konqueror, your starting planet is at " << start_pos;
    }

    return msg.str();
}

void Universe::checkGameEnded() {
    //todo
}