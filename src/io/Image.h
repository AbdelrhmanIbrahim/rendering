#pragma once

#include "defs/Defs.h"

#include "math/Vector.h"

namespace io
{
	struct Image
	{
		int width, height, channels;
		void* data;
	};

	Image
	image_new(int channels, math::Vec2f size);

	Image
	image_read(const char* path, IMAGE_FORMAT format);

	void
	image_write(const Image& img, const char* path, IMAGE_FORMAT format);

	void
	image_resize(Image& img, math::Vec2f size);

	void
	image_free(Image& img);
};