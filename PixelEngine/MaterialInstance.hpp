#pragma once

#include <SDL2/SDL.h>

#ifndef INC_Materials
#include "Materials.hpp"
#endif // !INC_Materials


#define INC_MaterialInstance

#define MAX_TEMPERATURE 9999
#define MIN_TEMPERATURE -273

class MaterialInstance {
public:
    static int _curID;

    Material* mat;
    Uint32 color;
    double energy;
    uint32_t id = 0;
    bool moved = false;
    float fluidAmount = 2.0f;
    float fluidAmountDiff = 0.0f;
    uint8_t settleCount = 0;

    MaterialInstance(Material* mat, Uint32 color, double temperature);
    MaterialInstance(Material* mat, Uint32 color) : MaterialInstance(mat, color, 0) {};
    MaterialInstance() : MaterialInstance(&Materials::GENERIC_AIR, 0x000000, 0) {};
    bool operator==(const MaterialInstance& other);

    double get_temperature();
    void set_temperature(double);

    void conduct_heat_to(MaterialInstance& other, const double time_span);
};
