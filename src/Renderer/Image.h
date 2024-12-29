#pragma once
#include "tspch.h"

namespace TS_ENGINE {

	struct Image
	{
		std::vector<uint8_t> pixels;
		int width = 0;
		int height = 0;
		
		void WritePixelsToFile(const std::string& filePath);
	};
}