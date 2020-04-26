#pragma once

#include "world/component/Vertex.h"

#include <vector>

namespace io
{
	struct Indexed_Triangles
	{
		std::vector<world::Vertex> vertices;
		std::vector<unsigned int> indices;
	};

	Indexed_Triangles
	stl_binary_read(const char* stl_path);
}
