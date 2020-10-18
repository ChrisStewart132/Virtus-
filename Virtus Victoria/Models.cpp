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
			std::cout << "created:" << unitList[unitList.size() - 1].unitTypePtr->name << std::endl;
			for (uint32_t j = 0; j < unitTypeList[i].vertices.size(); j++) {
				vertices.push_back(unitTypeList[i].vertices[j]);//iterate unit types vertices, push vertices into main vertex buffer;
			}
			//printf("unitlist size: %i\n", unitList.size());
			//printf("cube vertices size: %i\n", unitList[unitList.size() - 1].hitbox.cubeVertices.size());
			for (uint32_t j = 0; j < unitList[unitList.size() - 1].hitbox.cubeVertices.size(); j++) {//add cubeMesh created in unit class to vertex2 for rendering
				vertices2.push_back(unitList[unitList.size()-1].hitbox.cubeVertices[j]);//iterate unit types vertices, push vertices into main vertex buffer;
				//std::cout << unitList[unitList.size() - 1].hitbox.cubeVertices[j].pos.x << "," << unitList[unitList.size() - 1].hitbox.cubeVertices[j].pos.y << "," << unitList[unitList.size() - 1].hitbox.cubeVertices[j].pos.z << std::endl;
			}
			success = true;
		}
	}
	if (!success) {
		std::cout << string << " load fail:" << success << std::endl;
	}
	//std::cout << vertices.size() << std::endl;
}

void Models::loadCube()

{
	static bool initialized = false;
	static unitType cubeType;
	if (!initialized) {
		//cube model type
		cubeType.name = "cube";
		cubeType.mass = 1000;
		cubeType.textureIndex = 4;

		std::vector<Vertex> cubeVertices;
		glm::vec3 max, min, location;
		glm::vec3 cubeVertexColour = { 1.0f,1.0f,1.0f };
		float r = 1.0f/2;
		max.x = r;
		min.x = -r;
		max.y = r;
		min.y = -r;
		max.z = r;
		min.z = -r;
		//top
		glm::vec3 LEFT_TOP_FRONT = { min.x,max.y,min.z };
		glm::vec3 RIGHT_TOP_FRONT = { max.x,max.y,min.z };
		glm::vec3 LEFT_TOP_BACK = { min.x,max.y,max.z };
		glm::vec3 RIGHT_TOP_BACK = { max.x,max.y,max.z };
		//bottom
		glm::vec3 LEFT_BOT_FRONT = { min.x,min.y,min.z };
		glm::vec3 RIGHT_BOT_FRONT = { max.x,min.y,min.z };
		glm::vec3 LEFT_BOT_BACK = { min.x,min.y,max.z };
		glm::vec3 RIGHT_BOT_BACK = { max.x,min.y,max.z };

		//36 vertices (cube) each nhitbox
		for (int j = 0; j < 18; j++) {
			Vertex vertex;
			if (j == 0) {//
				vertex.pos = LEFT_BOT_BACK;//back
			}
			else if (j == 1) {
				vertex.pos = RIGHT_BOT_BACK;
			}
			else if (j == 2) {
				vertex.pos = RIGHT_TOP_BACK;
			}				//				
			else if (j == 3) {
				vertex.pos = RIGHT_TOP_BACK;//back
			}
			else if (j == 4) {
				vertex.pos = LEFT_TOP_BACK;
			}
			else if (j == 5) {
				vertex.pos = LEFT_BOT_BACK;
			}//						 		
			else if (j == 6) {
				vertex.pos = RIGHT_BOT_BACK;//right
			}
			else if (j == 7) {
				vertex.pos = RIGHT_BOT_FRONT;
			}
			else if (j == 8) {
				vertex.pos = RIGHT_TOP_FRONT;
			}
			else if (j == 9) {
				vertex.pos = RIGHT_TOP_FRONT;//right
			}
			else if (j == 10) {
				vertex.pos = RIGHT_TOP_BACK;
			}
			else if (j == 11) {
				vertex.pos = RIGHT_BOT_BACK;
			}//						 		
			else if (j == 12) {
				vertex.pos = LEFT_TOP_BACK;//top
			}
			else if (j == 13) {
				vertex.pos = RIGHT_TOP_BACK;
			}
			else if (j == 14) {
				vertex.pos = RIGHT_TOP_FRONT;
			}
			else if (j == 15) {
				vertex.pos = RIGHT_TOP_FRONT;//top
			}
			else if (j == 16) {
				vertex.pos = LEFT_TOP_FRONT;
			}
			else if (j == 17) {
				vertex.pos = LEFT_TOP_BACK;
			}
			vertex.colour = vertex.pos;// cubeVertexColour;//back, right, top 		
			vertex.texCoord = { 0.0f,0.0f };
			vertex.normal = { 0.5f,0.5f,0.5f };
			cubeType.vertices.push_back(vertex);
		}
		for (int j = 18; j < 36; j++) {//front, left, bottom
			Vertex vertex;
			if (j == 18) {//
				vertex.pos = LEFT_BOT_FRONT;//front
			}
			else if (j == 19) {
				vertex.pos = LEFT_TOP_FRONT;//
			}
			else if (j == 20) {
				vertex.pos = RIGHT_TOP_FRONT;//
			}
			else if (j == 21) {
				vertex.pos = RIGHT_TOP_FRONT;//front
			}
			else if (j == 22) {
				vertex.pos = RIGHT_BOT_FRONT;//
			}
			else if (j == 23) {
				vertex.pos = LEFT_BOT_FRONT;//
			}//								
			else if (j == 24) {
				vertex.pos = LEFT_BOT_BACK;//left
			}
			else if (j == 25) {
				vertex.pos = LEFT_TOP_FRONT;//
			}
			else if (j == 26) {
				vertex.pos = LEFT_BOT_FRONT;//
			}
			else if (j == 27) {
				vertex.pos = LEFT_TOP_FRONT;//left
			}
			else if (j == 28) {
				vertex.pos = LEFT_BOT_BACK;//
			}
			else if (j == 29) {
				vertex.pos = LEFT_TOP_BACK;//
			}
			else if (j == 30) {
				vertex.pos = LEFT_BOT_BACK;//bottom
			}
			else if (j == 31) {
				vertex.pos = RIGHT_BOT_FRONT;
			}
			else if (j == 32) {
				vertex.pos = RIGHT_BOT_BACK;
			}
			else if (j == 33) {
				vertex.pos = RIGHT_BOT_FRONT;//bottom
			}
			else if (j == 34) {
				vertex.pos = LEFT_BOT_BACK;
			}
			else if (j == 35) {
				vertex.pos = LEFT_BOT_FRONT;
			}
			vertex.colour = vertex.pos;//cubeVertexColour;
			vertex.texCoord = { 0.0f,0.0f };
			vertex.normal = { 0.5f,0.5f,0.5f };
			cubeType.vertices.push_back(vertex);
		}

		initialized = true;
	}
	uint32_t prevVertexBufferSize = vertices.size();
	unit cube(&cubeType, prevVertexBufferSize);
	unitList.push_back(cube);
	for (uint32_t j = 0; j < unitList[unitList.size() - 1].hitbox.cubeVertices.size(); j++) {//move cubeMesh created in unit class to vertex2 for rendering
		vertices2.push_back(unitList[unitList.size() - 1].hitbox.cubeVertices[j]);//iterate unit types vertices, push vertices into main vertex buffer;
	}
	for (uint32_t j = 0; j < cubeType.vertices.size(); j++) {
		vertices.push_back(cubeType.vertices[j]);//iterate unit types vertices, push vertices into main vertex buffer;
	}
}

void Models::loadHitboxes()
{
	//printf("unitlist size: %i\n", unitList.size());
	for (uint32_t i = 0; i < unitList.size(); i++) {
		//std::cout << "loading unit: " << i << " hitbox\n";
		for (uint32_t j = 0; j < 36; j++) {
			vertices.push_back(unitList[i].hitbox.cubeVertices[j]);//iterate units, push vertices into main vertex buffer;
		}
	}
	//printf("at start vertices has size of: %i\n", vertices.size());
	//printf("at start vertices2 has size of: %i\n", vertices2.size());
	//printf("is this used? loadHitboxes()\n");//seems not really
}

void Models::updateHitboxes()
{
	//printf("is this used? updateHitboxes()\n");//yes every frame..
	for (uint32_t u = 0; u < unitList.size(); u++) {//for each unit
		uint32_t start = vertices.size() - (unitList.size() * 36) + (36*u);//start vertex of unit hitbox
		//for (uint32_t i = start; i < start+36; i++) {//unit hitbox vertices	
			//vertices[i].pos = unitList[u].hitbox.cubeVertices[(i - start)].pos;//iterate units vertices, update vertices to unit vertices;	
		//}
		
		start = (u * 36);
		//std::cout << u << "," << start << "," << vertices2.size() << "," << unitList[u].unitTypePtr->name <<std::endl;
		for (uint32_t i = start; i < start+36; i++) {//unit hitbox vertices	
			vertices2[i].pos = unitList[u].hitbox.cubeVertices[i-start].pos;//iterate units vertices, update vertices to unit vertices;	
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

	/*loadModel("a4");
	unitList[3].move(glm::vec3(1.0f, 1.0f, -10.0f));
	unitList[3].antiGravity = true;*/

	//loadModel("20mm gun");
	////unitList[4].move(glm::vec3(0.5f, 1.5f, 10.f));
	//unitList[4].move(glm::vec3(0.5f, 1.5, 10.f));
	//unitList[4].antiGravity = true;

	//loadModel("t-54");
	//unitList[4].move(glm::vec3(10.0f, 0.0f, -20.0f));

	const uint32_t c = 0;//n^3 higher for more models
	const float s = 2.5f;//spacing
	for (int i = 0; i < c; i++) {
		for (int j = 0; j < c; j++) {
			loadCube();
			unitList[3 + (i* c) + j].move(glm::vec3(1.0f - (j * s), 0.0f, -1.0f - (i * s)));
		}
	}
}

