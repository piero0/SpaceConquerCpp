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
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

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

enum class Owner {
    STRAY,
    YOU,
    CPU1
};

struct Planet {
    std::string name {""};
    Position position {0, 0};
    float production = 1.0f;
    float ships_in_production = 0.0f;
    ushort ships = 0;
    Owner owner = Owner::STRAY;

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
        TRANSPORT_CREATED,
        WIN,
        DOMINATION,
        DEFEAT
    };

    Event::Type type;
    std::string message;

    Event() {}
    Event(Event::Type type, Transport* t);
};

class Universe {
    std::unordered_map<Position, Planet, PositionHash> m_planets;
    std::vector<Transport> m_transports;
    std::vector<Event> m_events;
    Config m_cfg;
    ushort m_round = 1;
    bool m_playing = true;
    bool m_free_play = false; //playing after defeating all oponents

    public:
        Universe(const Config& cfg);
        void reset();
        void checkGameEnded();

        ushort calcDistance(Planet* src, Planet* dst);
        Transport* makeTransport(Planet* src, Planet* dst, ushort ships);

        Position convertToPosition(const std::string& location);
        std::string positionToString(const Position& pos);

        Planet* getPlanetByPos(const Position& pos);
        Planet* getPlanetAt(const std::string& loc);
        std::vector<Planet> getUserPlanets(Owner owner);

        void generatePlanets();
        Position generatePosition(std::uniform_int_distribution<>& gx, std::uniform_int_distribution<>& gy, std::mt19937& gen);
        void assignPlayers();
        
        Config* getConfig() { return &m_cfg; }
        std::vector<Event>& getEvents() { return m_events; }

        void land(Transport& t);
        void nextRound();
        ushort getRoundCount() const {return m_round;}
        void setRoundCount(ushort rnd) {m_round = rnd;}
        bool isFreePlay() const {return m_free_play;}
        void setFreePlay(bool s) {m_free_play = s;}
        
        bool isPlaying() const {return m_playing;}
        void setPlaying(bool state) {m_playing = state;}
        
        std::unordered_map<Position, Planet, PositionHash>& getPlanets() { return m_planets; }
        std::vector<Transport>& getTransports() { return m_transports; }

        std::string getInitMsg();

        ushort getMaxSize() const { return 26; }
};
