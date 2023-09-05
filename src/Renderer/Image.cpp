#include "tspch.h"
#include "Image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include <stb_image_resize.h>

void TS_ENGINE::Image::WritePixelsToFile(const std::string& filePath)
{
	stbi_flip_vertically_on_write(1); // Flip the image vertically (optional)

	int newWidth = 400;
	int newHeight = 300;

	unsigned char* resizedImage = new unsigned char[newWidth * newHeight * 4];
	bool success = false;// stbir_resize_uint8(pixels.data(), mSpecification.Width, mSpecification.Height, 0, resizedImage, newWidth, newHeight, 0, 4);

	if (success)
	{
		if (!stbi_write_png(filePath.c_str(), newWidth, newHeight, 4, resizedImage, newWidth * 4))
		{
			TS_CORE_ERROR("Failed to save the PNG file!");
		}
	}
	else
	{
		TS_CORE_ERROR("Unable to resize image!");

		if (!stbi_write_png(filePath.c_str(), width, height, 4, pixels.data(), width * 4))
		{
			TS_CORE_ERROR("Failed to save the PNG file!");
		}
	}
}
