#pragma once

namespace io
{
	struct Image
	{
		int width, height, channels;
		unsigned char* data;
	};

	enum class FORMAT
	{
		BMP,
		PNG,
		JPG
	};

	Image
	image_read(const char* path);

	void
	image_write(const Image& img, const char* path, FORMAT format);

	void
	image_free(Image& img);
};