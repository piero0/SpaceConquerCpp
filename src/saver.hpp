#pragma once
#include "universe.hpp"
#include <fstream>

class Saver {
    enum class Section {
        START,
        CONFIG,
        PLANETS,
        TRANSPORTS,
        EVENTS,
        STATE,
        END
    };

    Universe* m_u = nullptr;

    void loadPlanet(std::string line);
    void loadConfig(std::string line);
    void loadTransport(std::string line);
    void loadEvent(std::string line);
    void loadState(std::string line);

    std::vector<std::string> splitArgs(std::string& line, size_t max_splits=0);

    public:
        bool save(Universe* u, std::string filename);
        bool load(Universe* u, std::string filename);
};
