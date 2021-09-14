#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <cmath>
#include <limits>
#include <random>
#include <iostream>

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
    Position position {0, 0};
    float production = 1.0f;
    float sheeps_in_production = 0.0f;
    ushort sheeps = 0;
    ushort owner = 0;

    void nextRound();
};

struct Transport { // moze jakas lepsza nazwa ?? 
    ushort distance;
    ushort sheeps;
    Planet* destination;
};

class Universe {
    std::unordered_map<Position, Planet, PositionHash> m_planets;
    std::vector<Transport> m_transports;
    Config m_cfg;

    public:
        Universe(const Config& cfg);
        ushort calcDistance(Planet* src, Planet* dst);
        void makeTransport(Planet* src, Planet* dst, ushort sheeps);
        Planet* getPlanetByPos(const Position& pos);
        Position convertToPosition(const std::string& pos);
        void generatePlanets();
        Position generatePosition(std::uniform_int_distribution<>& gx, std::uniform_int_distribution<>& gy, std::mt19937& gen);
        void assignPlayers();
        const Config& getConfig() const { return m_cfg; }
};
