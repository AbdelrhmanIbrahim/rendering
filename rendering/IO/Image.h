#pragma once

namespace io
{
	struct Image
	{
		int width, height, channels;
		unsigned char* data;
	};

	Image
	image_read(const char* path);

	void
	image_free(Image& img);
};