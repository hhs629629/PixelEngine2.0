
#ifndef INC_Structure
#include "Structure.hpp"
#endif // !INC_Structure


#include <iostream>

class PlacedStructure {//Å¬·¡½º
public:
    Structure base;
    int x;
    int y;

    PlacedStructure(Structure base, int x, int y);
    //PlacedStructure(const PlacedStructure &p2) { this->base = Structure(base); this->x = x; this->y = y; }
};
