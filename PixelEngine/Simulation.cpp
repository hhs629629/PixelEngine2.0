#include "stdafx.h"
#include "Simulation.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

Simulation::Simulation(int _width, int _height, Uint32 _pixelFormat)
{
	width	= _width;
	height	= _height;
	size	= static_cast<Uint64>(_width) * static_cast<Uint64>(_height);

	pixelFormat = SDL_AllocFormat(_pixelFormat);

	mt = std::mt19937(std::time(0)); 
	std::uniform_real_distribution<double> doubleDist(0.0, 1.0); 
	std::uniform_int_distribution<int> xorSeedDist(100'000'000);

	computeBuffer	= new Material[size];
	drawBuffer		= new Uint32[size];
	Simulation::reset();
	batchNoise		= new Uint16[size];
	iterationNoise	= new Uint32[size];
	for(int i = 0; i < size; ++i)
	{
		batchNoise[i]		= i % (size / RAND_BATCH_SIZE) == 0 ? i / (size / RAND_BATCH_SIZE) : batchNoise[i - 1];
		iterationNoise[i]	= i;
	}
	std::shuffle(batchNoise, batchNoise + size, mt);
	std::shuffle(iterationNoise, iterationNoise + size, mt);

	updatedCells = new boost::dynamic_bitset<Uint64>(size);

	boost::property_tree::ptree root;
	boost::property_tree::read_json(MATERIAL_FILE_PATH, root);
	for(auto it = root.begin(); it != root.end(); ++it)
	{
		int dist = std::distance(root.begin(), it);
		if(dist >= static_cast<int>(Material::TOTAL_MATERIALS) - 1) { break; }
		MaterialSpecs &mat = allSpecs[dist + 1];
		mat.name = it->first;
		auto color = it->second.get_child("minColor");
		mat.minColor = {color.get<Uint8>("h"), color.get<Uint8>("s"), color.get<Uint8>("v")};
		color = it->second.get_child("maxColor");
		mat.maxColor = {color.get<Uint8>("h"), color.get<Uint8>("s"), color.get<Uint8>("v")};
		mat.minSpeed = it->second.get<Uint8>("minSpeed");
		mat.maxSpeed = it->second.get<Uint8>("maxSpeed");
		mat.density = it->second.get<Uint8>("density");
		mat.deathChance = it->second.get<Uint8>("deathChance");
		mat.solid = it->second.get<bool>("solid");
		mat.flaming = it->second.get<bool>("flaming");
		mat.flammable = it->second.get<bool>("flammable");
		mat.melting = it->second.get<bool>("melting");
		mat.meltable = it->second.get<bool>("meltable");
		int i = 0;
		for(auto arr : it->second.get_child("behavior"))
		{
			int j = 0;
			for(auto dir : arr.second)
			{
				mat.behavior[i][j] = static_cast<Direction>(dir.second.get_value<int>());
				++j;
			}
			mat.behaviorCounts[i] = j;
			++i;
		}
		mat.behaviorSetCount = i;
	}
}

Simulation::~Simulation()
{
	delete[] computeBuffer;
	delete[] drawBuffer;
	delete[] batchNoise;
	delete[] iterationNoise;
	delete updatedCells;
	SDL_FreeFormat(pixelFormat);
}

std::string Simulation::getMaterialString() const
{
	std::string result = std::string();
	for(int i = 1; i < static_cast<int>(Material::TOTAL_MATERIALS); ++i)
	{
		result += allSpecs[i].name;
		result += ' ';
	}
	return result;
}

void Simulation::update()
{
	Uint32 *randBatch = new Uint32[RAND_BATCH_SIZE];
	for(int i = 0; i < RAND_BATCH_SIZE; ++i) 
	{ 
		randBatch[i] = xorshift128();
	}

	updatedCells->reset();
	for(int i = 0; i < size; ++i)
	{
		Uint32 index = iterationNoise[i];
		if(computeBuffer[index] == Material::EMPTY || updatedCells->test(index)) { continue; }

		const MaterialSpecs *matSpecs = &allSpecs[static_cast<int>(computeBuffer[index])];
		if(matSpecs->behaviorSetCount == 0) { continue; }

		Uint32 randi = randBatch[batchNoise[index]];
		auto preGenRandRange = [&](Uint8 _min, Uint8 _max)
		{
			if(_min == _max) { return _min; }
			Uint8 result = _min + randi % (_max + 1 - _min);
			randi *= 0.1;
			return result;
		};

		if(matSpecs->deathChance > 0)
		{
			if(preGenRandRange(1, matSpecs->deathChance) == 1)
			{
				setCell(index, Material::EMPTY);
				continue;
			}
		}

		bool moved = false;
		Uint8 speed = preGenRandRange(matSpecs->minSpeed, matSpecs->maxSpeed);

		for(int j = 0; j < matSpecs->behaviorSetCount; ++j)
		{
			if(moved) { break; }

			Uint8 directionIndex = preGenRandRange(0, matSpecs->behaviorCounts[j] - 1);
			for(int k = 0; k < matSpecs->behaviorCounts[j]; ++k)
			{
				Direction direction = matSpecs->behavior[j][directionIndex];
				Uint32 lastIndex = index;
				bool destroyed = false;
				for(int l = 0; l < speed; ++l)
				{
					Uint32 newIndex = getRelative(lastIndex, direction);
					if(newIndex == -1) { break; }
					if(computeBuffer[newIndex] != Material::EMPTY)
					{
						const MaterialSpecs *collisionSpecs = &allSpecs[static_cast<int>(computeBuffer[newIndex])];
						if(matSpecs->flaming && collisionSpecs->flammable)
						{
							setCell(newIndex, Material::FIRE);
							break;
						}
						if(matSpecs->melting && collisionSpecs->meltable)
						{
							setCell(newIndex, Material::LAVA);
							break;
						}
						if(computeBuffer[index] == Material::WATER)
						{
							if(collisionSpecs->flaming || collisionSpecs->melting)
							{
								setCell(index, Material::STEAM);
								destroyed = true;

								if(computeBuffer[newIndex] == Material::LAVA && preGenRandRange(0, 1) == 0) 
								{ 
									setCell(newIndex, Material::GRAVEL); 
								}
								else
								{
									setCell(newIndex, Material::EMPTY);
								}

								break;
							}
						}
						if(!collisionSpecs->solid && 
							(collisionSpecs->density < matSpecs->density ||
							collisionSpecs->density > matSpecs->density && direction < Direction::EAST))
						{
							lastIndex = newIndex;
							break;
						}
						break;
					}
					lastIndex = newIndex;
				}
				if(lastIndex != index && !destroyed)
				{
					swapCell(index, lastIndex);
					moved = true;
					break;
				}
				if(++directionIndex >= matSpecs->behaviorCounts[j]) { directionIndex = 0; }
			}
		}
		if(!moved && !matSpecs->solid)
		{
			Uint8 direction = preGenRandRange(0, static_cast<int>(Direction::TOTAL_DIRECTIONS) - 1);
			for(int j = 0; j < static_cast<int>(Direction::TOTAL_DIRECTIONS); ++j)
			{
				Uint32 location = getRelative(index, static_cast<Direction>(direction));
				if(location == -1) { continue; }
				Material buffMat = computeBuffer[location];
				if(!allSpecs[static_cast<int>(buffMat)].solid && allSpecs[static_cast<int>(buffMat)].density == matSpecs->density)
				{
					swapCell(index, location);
					break;
				}
				if(direction >= static_cast<int>(Direction::TOTAL_DIRECTIONS)) { direction = 0; }
			}
		}
	}
	delete[] randBatch;
}

void Simulation::reset(Material _mat, const SDL_Color *_col)
{
	memset(computeBuffer, static_cast<int>(_mat), size * sizeof(Uint8));
	memset(drawBuffer, SDL_MapRGBA(pixelFormat, _col->r, _col->g, _col->b, _col->a), size * sizeof(Uint32));
}

void Simulation::setCellLine(SDL_Point _start, SDL_Point _end, Uint16 _rad, Material _mat)
{
	setCellRadius(_start, _rad, _mat);
	if(!(_start.x == _end.x && _start.y == _end.y))
	{
		setCellRadius(_end, _rad, _mat);
		float angle = -atan2(_end.x - _start.x, _end.y - _start.y);
		Sint32 tanDiffX = round(cos(angle) * _rad);
		Sint32 tanDiffY = round(sin(angle) * _rad);
		Sint32 *tanLineX, *tanLineY, *mainLineX, *mainLineY;
		Uint16 tanLen = Graphics::bresenhams(_start.x + tanDiffX, _start.y + tanDiffY, _start.x - tanDiffX, _start.y - tanDiffY, tanLineX, tanLineY, true);
		Uint16 mainLen = Graphics::bresenhams(_start.x + tanDiffX, _start.y + tanDiffY, _end.x + tanDiffX, _end.y + tanDiffY, mainLineX, mainLineY, false);
		for(int i = 0; i < tanLen; ++i)
		{
			for(int j = 0; j < mainLen; ++j)
			{
				Sint32 diffX = (_start.x + tanDiffX) - tanLineX[i];
				Sint32 diffY = (_start.y + tanDiffY) - tanLineY[i];
				setCellIfValid(mainLineX[j] - diffX, mainLineY[j] - diffY, _mat);
			}
		}
		delete[] mainLineX;
		delete[] mainLineY;
	}
}

Uint32 Simulation::getRelative(Uint32 _index, Direction _dir) const
{
	Sint8 horizontal = 0;
	horizontal += _dir == Direction::NORTH_EAST || _dir == Direction::EAST || _dir == Direction::SOUTH_EAST;
	horizontal -= _dir == Direction::SOUTH_WEST || _dir == Direction::WEST || _dir == Direction::NORTH_WEST;
	Sint8 vertical = 0;
	vertical += _dir == Direction::SOUTH_EAST || _dir == Direction::SOUTH || _dir == Direction::SOUTH_WEST;
	vertical -= _dir == Direction::NORTH_WEST || _dir == Direction::NORTH || _dir == Direction::NORTH_EAST;
	
	if(horizontal != 0)
	{
		Uint32 original = _index;
		_index += horizontal;
		if(_index / width != original / width) { return -1; }
	}
	if(vertical != 0)
	{
		_index += vertical * width;
		if(_index >= size || _index < 0) { return -1; }
	}
	return _index;
}

SDL_Color Simulation::HsvToRgb(const HsvColor *_hsv) const
{
	SDL_Color rgba = {0, 0, 0, 255};
	
	if(_hsv->s == 0)
	{
		rgba.r = _hsv->v;
		rgba.g = _hsv->v;
		rgba.b = _hsv->v;
		return rgba;
	}

	Uint8 region, remainder, p, q, t;
	region = _hsv->h / 43;
	remainder = (_hsv->h - (region * 43)) * 6;
	p = (_hsv->v * (255 - _hsv->s)) >> 8;
	q = (_hsv->v * (255 - ((_hsv->s * remainder) >> 8))) >> 8;
	t = (_hsv->v * (255 - ((_hsv->s * (255 - remainder)) >> 8))) >> 8;

	switch(region)
	{
	case 0:
		rgba.r = _hsv->v; rgba.g = t; rgba.b = p;
		break;
	case 1:
		rgba.r = q; rgba.g = _hsv->v; rgba.b = p;
		break;
	case 2:
		rgba.r = p; rgba.g = _hsv->v; rgba.b = t;
		break;
	case 3:
		rgba.r = p; rgba.g = q; rgba.b = _hsv->v;
		break;
	case 4:
		rgba.r = t; rgba.g = p; rgba.b = _hsv->v;
		break;
	default:
		rgba.r = _hsv->v; rgba.g = p; rgba.b = q;
		break;
	}

	return rgba;
}

void Simulation::setCell(Uint32 _index, Material _mat)
{
	computeBuffer[_index] = _mat;

	if(_mat != Material::EMPTY)
	{
		auto randLerp = [&](Uint8 min, Uint8 max) { return static_cast<Uint8>(min + round((max - min) * doubleDist(mt))); };
		const MaterialSpecs *specs = &allSpecs[static_cast<int>(_mat)];
		HsvColor interpolatedHsv = {
			randLerp(specs->minColor.h, specs->maxColor.h),
			randLerp(specs->minColor.s, specs->maxColor.s),
			randLerp(specs->minColor.v, specs->maxColor.v)
		};
		SDL_Color rgba = HsvToRgb(&interpolatedHsv);
		drawBuffer[_index] = SDL_MapRGBA(pixelFormat, rgba.r, rgba.g, rgba.b, rgba.a);
	}
	else
	{
		drawBuffer[_index] = SDL_MapRGBA(pixelFormat, EMPTY_COLOR.r, EMPTY_COLOR.g, EMPTY_COLOR.b, EMPTY_COLOR.a);
	}

	updatedCells->set(_index);
}

void Simulation::setCellIfValid(Sint32 _x, Sint32 _y, Material _mat)
{
	Uint32 index = _x + _y * width;
	if(_y >= 0 && _y < height && _x >= 0 && _x < width && (_mat == Material::EMPTY || computeBuffer[index] == Material::EMPTY))
	{
		setCell(index, _mat);
	}
}

void Simulation::setCellRadius(SDL_Point _pos, Uint16 _rad, Material _mat)
{
	for(int i = 0; i < _rad * 2; ++i)
	{
		Uint16 slice = sqrt(pow(_rad, 2) - pow(abs(_rad - i), 2));
		for(int j = -slice; j < slice; ++j)
		{
			Sint32 x = _pos.x + j;
			Sint32 y = _pos.y - _rad + i;
			setCellIfValid(x, y, _mat);
		}
	}
}

void Simulation::swapCell(Uint32 _current, Uint32 _next)
{
	Material tempMat = computeBuffer[_next];
	computeBuffer[_next] = computeBuffer[_current];
	computeBuffer[_current] = tempMat;
	Uint32 tempCol = drawBuffer[_next];
	drawBuffer[_next] = drawBuffer[_current];
	drawBuffer[_current] = tempCol;
	updatedCells->set(_next);
}

// xorshift 난수 생성 알고리즘
Uint32 Simulation::xorshift128()
{
	static Uint32 a = xorSeedDist(mt), b = xorSeedDist(mt), c = xorSeedDist(mt), d = xorSeedDist(mt);
	Uint32 t = d;
	Uint32 s = a;

	d = c;
	c = b;
	b = s;

	t ^= t << 11;
	t ^= t >> 8;
	a = t ^ s ^ (s >> 19);

	return a;
}