#include "IO/Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "IO/Stb_Image.h"

namespace io
{
	Image
	image_read(const char* path)
	{
		Image self{};
		self.data = stbi_load(path, &self.width, &self.height, &self.channels, 0);
		return self;
	}

	void
	image_free(Image& img)
	{
		stbi_image_free(img.data);
	}
};