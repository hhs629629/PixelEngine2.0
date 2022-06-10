
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
    
        auto q = time_span * k * (this->get_temperature() - other.get_temperature());
    
        this->energy -= q;
        other.energy += q;

        auto after = this->get_temperature() - other.get_temperature();

        if (q * after < 0) {
            const auto c1 = this->mat->heat_capacity;
            const auto c2 = other.mat->heat_capacity;

            const auto c = c1 + c2;

            const auto energy = this->energy + other.energy;

            this->energy = energy / c * c1;

            other.energy = energy - this->energy;
        }
    }
}
