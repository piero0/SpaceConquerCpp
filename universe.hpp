#pragma once

#include <vector>

using ushort = unsigned short;

struct Position {
    ushort x,y;
};

class Planet {
    ushort production;
    ushort sheeps;
    Position position;
};

struct Transport { // moze jakas lepsza nazwa ?? 
    ushort distance;
    ushort sheeps;
    Planet* destination;
};
