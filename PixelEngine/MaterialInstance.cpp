
#include "MaterialInstance.hpp"

int MaterialInstance::_curID = 1;

MaterialInstance::MaterialInstance(Material* mat, Uint32 color, double temperature)
{
    this->id = _curID++;
    this->mat = mat;
    this->color = color;
    this->energy = temperature * mat->heat_capacity;
}

bool MaterialInstance::operator==(const MaterialInstance & other) {
    return this->id == other.id;
}

double MaterialInstance::get_temperature()
{
    return this->energy/this->mat->heat_capacity;
}

void MaterialInstance::set_temperature(double temperature)
{
    this->energy = temperature * this->mat->heat_capacity;
}

void MaterialInstance::conduct_heat_to(MaterialInstance& other, double time_span)
{
    const auto k1 = this->mat->conductivity;
    const auto k2 = other.mat->conductivity;
    
    if (k2 != 0) {
        const auto k = k1 * k2 / (k1 + k2);
    
        const auto q = time_span * k * (this->get_temperature() - other.get_temperature());
    
        this->energy -= q;
        other.energy += q;
    }
}
