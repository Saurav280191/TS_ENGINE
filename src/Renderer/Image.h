#pragma once
#include "tspch.h"

namespace TS_ENGINE {

	struct Image
	{
		std::vector<uint8_t> pixels;
		int width;
		int height;
		
		void WritePixelsToFile(const std::string& filePath);
	};

}
