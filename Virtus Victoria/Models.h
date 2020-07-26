#pragma once

#include <vector>
#include <string>
#include <iostream>

#include "Vertex.h"
#include "unit.h"

//const std::string terrainPath = "models/terrain.obj";

class Models
{
private:

glm::vec3 defaultColour = { 0.5f,0.5f,0.5f };

void loadModelType(std::string MODEL_PATH, uint32_t _index);
void loadModelTypes();


void loadModels();

public:
Models(unitType* _unitTypeArray, const uint32_t _unitTypeCount);
~Models();

std::vector<unitType> unitTypeList;
std::vector<unit> unitList;
std::vector<Vertex> vertices;
//std::vector<uint32_t> indices; for index buffer


void transformUnits();
void loadModel(std::string string);
void loadCube();
void loadHitboxes();
void updateHitboxes();
};



