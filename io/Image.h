#pragma once

#include "defs/Defs.h"

namespace io
{
	struct Image
	{
		int width, height, channels;
		void* data;
	};

	Image
	image_read(const char* path, IMAGE_FORMAT format);

	void
	image_write(const Image& img, const char* path, IMAGE_FORMAT format);

	void
	image_free(Image& img);
};