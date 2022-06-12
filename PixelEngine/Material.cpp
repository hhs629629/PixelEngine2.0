
#include "Material.hpp"

Material::Material(int id, std::string name, int physicsType, int slipperyness, Uint8 alpha, float density, int iterations, int emit, Uint32 emitColor, Uint32 color, double heat_capacity, double conductivity) {
    this->name = name;
    this->id = id;
    this->physicsType = physicsType;
    this->slipperyness = slipperyness;
    this->alpha = alpha;
    this->density = density;
    this->iterations = iterations;
    this->emit = emit;
    this->emitColor = emitColor;
    this->color = color;
    this->heat_capacity = heat_capacity;
    this->conductivity = conductivity;
}
