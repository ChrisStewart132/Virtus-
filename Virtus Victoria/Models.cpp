#include "Models.h"

#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc

#include "tiny_obj_loader.h"

Models::Models(unitType* _unitTypeArray,const uint32_t _unitTypeCount)
{
	for (uint32_t i = 0; i < _unitTypeCount; i++) {
		unitTypeList.push_back(_unitTypeArray[i]);
	}
	loadModelTypes();
	loadModels();
}

Models::~Models()
{
}

void Models::loadModelType(std::string MODEL_PATH, uint32_t _index)
{

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	//std::string warn;
	std::string err;

	
	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, MODEL_PATH.c_str())) {
		throw std::runtime_error(err);
	}
	

	//std::unordered_map<Vertex, uint32_t> uniqueVertices = {};

	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			Vertex vertex = {};

			vertex.pos = {
			attrib.vertices[3 * index.vertex_index + 0],
			attrib.vertices[3 * index.vertex_index + 1],
			attrib.vertices[3 * index.vertex_index + 2]
			};

			
			vertex.texCoord = {
			attrib.texcoords[2 * index.texcoord_index + 0],
			1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
			};

			vertex.normal = {
			attrib.normals[3 * index.normal_index + 0],
			attrib.normals[3 * index.normal_index + 1],
			attrib.normals[3 * index.normal_index + 2]
			};

			vertex.colour = defaultColour;
			unitTypeList[_index].vertices.push_back(vertex);

			/*
			if (uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
				}
				indices.push_back(uniqueVertices[vertex]);
				}
			}
			*/
		}
	}
}

void Models::loadModelTypes() 
{
	for (uint32_t i = 0; i < unitTypeList.size(); i++) {
		loadModelType(unitTypeList[i].modelPath, i);	
		unitTypeList[i].COM = { 0.0f,0.0f,0.0f };//.obj files loaded should have centre of mass at 0,0, while y is calculated based on half height
		float minHeight, maxHeight;
		for (uint32_t j = 0; j < unitTypeList[i].vertices.size(); j++) {
			if (j == 0) {
				minHeight = unitTypeList[i].vertices[j].pos.y;
				maxHeight = unitTypeList[i].vertices[j].pos.y;
			}
			else {
				if (minHeight > unitTypeList[i].vertices[j].pos.y) {
					minHeight = unitTypeList[i].vertices[j].pos.y;
				}
				if (maxHeight < unitTypeList[i].vertices[j].pos.y) {
					maxHeight = unitTypeList[i].vertices[j].pos.y;
				}
			}
		}
		unitTypeList[i].COM.y = (minHeight + maxHeight) / 2.0f;
		//unitTypeList[i].hitbox.calculateHitbox(unitTypeList[i].vertices);
	}
}

void Models::loadModel(std::string string)
{
	uint32_t prevVertexBufferSize = vertices.size();
	bool success = false;
	for (uint32_t i = 0; i < unitTypeList.size(); i++) {//for every unit type
		if (unitTypeList[i].name == string) {//if model found
			unit newUnit(&unitTypeList[i], prevVertexBufferSize);
			unitList.push_back(newUnit);
	
			for (uint32_t j = 0; j < unitTypeList[i].vertices.size(); j++) {
				vertices.push_back(unitTypeList[i].vertices[j]);//iterate unit types vertices, push vertices into main vertex buffer;
			}
			success = true;
		}
	}
	if (!success) {
		std::cout << string << " load fail:" << success << std::endl;
	}
	//std::cout << vertices.size() << std::endl;
}

void Models::loadHitboxes()
{
	for (uint32_t i = 0; i < unitList.size(); i++) {
		//std::cout << "loading unit: " << i << " hitbox\n";
		for (uint32_t j = 0; j < 36; j++) {
			vertices.push_back(unitList[i].hitbox.cubeVertices[j]);//iterate unit types vertices, push vertices into main vertex buffer;
		}
	}
}

void Models::updateHitboxes()
{
	for (uint32_t u = 0; u < unitList.size(); u++) {//for each unit
		uint32_t start = vertices.size() - (unitList.size() * 36) + (36*u);//start vertex of unit hitbox
		for (uint32_t i = start; i < start+36; i++) {//unit hitbox vertices	
			vertices[i].pos = unitList[u].hitbox.cubeVertices[(i - start)].pos;//iterate units vertices, update vertices to unit vertices;		
		}
	}
}

void Models::transformUnits()
{
	for (int i = 0; i < unitList.size(); i++) {
		if (unitList[i].translation == glm::vec3(0.0f) & unitList[i].rotation == glm::vec3(0.0f)) {}//dont transform objects which dont move
		else {
			unitList[i].transform(vertices);//rotate all units to face forward(-z) at 0,0; then rotate/transform unit along x,y axis
		}
	}
}

void Models::loadModels()
{
	loadModel("terrain");
	unitList[0].stationary = true;
	loadModel("20mm round");
	unitList[1].move(glm::vec3(0.0f, 10.0f, -15.0f));
	
	loadModel("pyramid");
	unitList[2].move(glm::vec3(-400.0f, 0.0f, -200.0f));

	const uint32_t c = 2;
	for (int i = 0; i < c; i++) {
		for (int j = 0; j < c; j++) {
			loadModel("t62");
			unitList[3 + (i* c) + j].move(glm::vec3(-20.0f - (j * 4), 20.0f, -20.0f - (i * 8)));
		}
	}
}

