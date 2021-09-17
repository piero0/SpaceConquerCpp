#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <cmath>
#include <limits>
#include <random>
#include <iostream>
#include <algorithm>
#include <queue>
#include <sstream>

using ushort = unsigned short;
using Position = std::pair<ushort, ushort>;

struct PositionHash {
    size_t operator()(const Position& p) const noexcept {
        return std::hash<ushort>{}(p.first) << sizeof(ushort) | std::hash<ushort>{}(p.second);
    }
};

struct Config {
    float production_min = 0.5f, production_max = 3.0f;
    ushort planets_num = 6;
    ushort xsize = 10, ysize = 10;
};

struct Planet {
    std::string name {""};
    Position position {0, 0};
    float production = 1.0f;
    float ships_in_production = 0.0f;
    ushort ships = 0;
    ushort owner = 0;

    void nextRound();
    bool sendShips(ushort ships_num);
};

struct Transport { // moze jakas lepsza nazwa ?? 
    ushort distance;
    ushort ships;
    Planet* destination;
    Planet* source;
};

struct Event {
    enum class Type {
        REINFORCEMENTS,
        ATTACK_FAIL,
        ATTACK_OK,
        WIN,
        DEFEAT
    };
    Type type;
    std::string message;

    Event(Event::Type type, Transport& t);
};

class Universe {
    std::unordered_map<Position, Planet, PositionHash> m_planets;
    std::vector<Transport> m_transports;
    std::queue<Event> m_events;
    Config m_cfg;
    ushort m_round = 1;
    bool m_playing = true;

    public:
        Universe(const Config& cfg);
        void reset();

        ushort calcDistance(Planet* src, Planet* dst);
        Transport* makeTransport(Planet* src, Planet* dst, ushort ships);

        Position convertToPosition(const std::string& location);
        std::string positionToString(const Position& pos);

        Planet* getPlanetByPos(const Position& pos);
        Planet* getPlanetAt(const std::string& loc);
        std::vector<Planet> getUserPlanets(ushort user_id);

        void generatePlanets();
        Position generatePosition(std::uniform_int_distribution<>& gx, std::uniform_int_distribution<>& gy, std::mt19937& gen);
        void assignPlayers();
        
        const Config& getConfig() const { return m_cfg; }
        std::queue<Event>& getEvents() { return m_events; }

        void land(Transport& t);
        void nextRound();
        ushort getRoundCount() const {return m_round;}
        
        bool isPlaying() const {return m_playing;}
        void setPlaying(bool state) {m_playing = state;}
        
        std::unordered_map<Position, Planet, PositionHash> getPlanets() { return m_planets; }
        std::vector<Transport> getTransports() { return m_transports; }

        std::string getInitMsg();
};
