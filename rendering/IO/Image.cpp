#include "IO/Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "IO/Stb_Image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "IO/Stb_Image_Write.h"

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
	image_write(const Image& img, const char* path, FORMAT format)
	{
		stbi_flip_vertically_on_write(true);
		switch (format)
		{
		case io::FORMAT::BMP:
			stbi_write_bmp(path, img.width, img.height, img.channels, img.data);
			break;
		case io::FORMAT::PNG:
			stbi_write_png(path, img.width, img.height, img.channels, img.data, img.width * img.channels);
			break;
		case io::FORMAT::JPG:
			stbi_write_jpg(path, img.width, img.height, 4, img.data, 100);
			break;
		default:
			assert("unsupported image format" && false);
			break;
		}
	}

	void
	image_free(Image& img)
	{
		stbi_image_free(img.data);
	}
};