#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <iterator>

struct bitHeader {
	uint8_t B;//0+1
	uint8_t M;//1+1
	uint64_t fileSize;//2+8
	uint32_t dataOffset;//10+4
	//14-17
	uint32_t width;//18+4
	uint32_t height;//22+4
	//26-27
	uint32_t depth;//28
};

std::vector<char> readBMP(const std::string& file)
{
	static constexpr size_t HEADER_SIZE = 54;
	std::ifstream bmp(file, std::ios::binary);
	std::array<char, HEADER_SIZE> header;
	bmp.read(header.data(), header.size());

	auto start = (uint16_t)(header[0]);//first 2 bytes un-used?
	
	
	
	bitHeader bitmapHeader;
	std::memcpy(&bitmapHeader.B, &header[0], sizeof(bitmapHeader.B));
	std::memcpy(&bitmapHeader.M, &header[1], sizeof(bitmapHeader.M));
	std::memcpy(&bitmapHeader.fileSize, &header[2], sizeof(bitmapHeader.fileSize));
	std::memcpy(&bitmapHeader.dataOffset, &header[10], sizeof(bitmapHeader.dataOffset));
	std::memcpy(&bitmapHeader.width, &header[18], sizeof(bitmapHeader.width));
	std::memcpy(&bitmapHeader.height, &header[22], sizeof(bitmapHeader.height));
	std::memcpy(&bitmapHeader.depth, &header[28], sizeof(bitmapHeader.depth));
	

	/*auto fileSize = *reinterpret_cast<uint32_t*>(&header[2]);
	auto dataOffset = *reinterpret_cast<uint32_t*>(&header[10]);
	auto width = *reinterpret_cast<uint32_t*>(&header[18]);
	auto height = *reinterpret_cast<uint32_t*>(&header[22]);
	auto depth = *reinterpret_cast<uint16_t*>(&header[28]);*/

	std::cout << "fileType: " << bitmapHeader.B << bitmapHeader.M << std::endl;
	std::cout << "fileSize: " << bitmapHeader.fileSize << std::endl;
	std::cout << "dataOffset: " << bitmapHeader.dataOffset << std::endl;
	std::cout << "width: " << bitmapHeader.width << std::endl;
	std::cout << "height: " << bitmapHeader.height << std::endl;
	std::cout << "depth: " << bitmapHeader.depth << "-bit" << std::endl;


	//static constexpr size_t DATA_SIZE = 1024;
	//std::ifstream bmp2(file, std::ios::binary);
	//std::array<char, DATA_SIZE> BGR;
	//bmp2.read(BGR.data(), BGR.size());

	//int offset = bitmapHeader.dataOffset;
	//uint32_t test;
	//std::memcpy(&test, &BGR[offset], sizeof(test));
	////std::cout << "test: " << test << std::endl;

	//for (int i = 0; i < DATA_SIZE- bitmapHeader.dataOffset; i++) {
	//	std::memcpy(&test, &BGR[offset], sizeof(test));
	//	std::cout << "test: " << test << " at: "<< i << std::endl;
	//	offset++;//keep printing 4 bytes 1 byte at a time from header depth to try and find the first colour byte
	//}

	//std::vector<char> img;


	std::vector<char> img(bitmapHeader.dataOffset - HEADER_SIZE);
	bmp.read(img.data(), img.size());

	auto dataSize = ((bitmapHeader.width * 3 + 3) & (~3)) * bitmapHeader.height;
	img.resize(dataSize);
	bmp.read(img.data(), img.size());

	char temp = 0;

	for (auto i = dataSize - 4; i >= 0; i -= 3)//
	{
		temp = img[i];
		img[i] = img[i + 2];
		img[i + 2] = temp;
		std::cout << "R: " << int(img[i] & 0xff) << " G: " << int(img[i + 1] & 0xff) << " B: " << int(img[i + 2] & 0xff) << std::endl;
	}

	return img;
}