#include "STL_Reader.h"

#include "math/Vector.h"

#include <fstream>

#include <unordered_map>
#include <type_traits>

#include <assert.h>

using namespace math;
using namespace geo;
using namespace std;

namespace io
{
	struct Point_Hash
	{
		unsigned int operator()(const vec3f& point) const
		{
			unsigned int h1 = hash<double>()(point.data[0]);
			unsigned int h2 = hash<double>()(point.data[1]);
			unsigned int h3 = hash<double>()(point.data[2]);
			return (h1 ^ (h2 << 1)) ^ h3;
		}
	};

	Indexed_Triangles
	stl_binary_read(const char * stl_path)
	{
		Indexed_Triangles self{};
		ifstream file(stl_path, ios::binary);
		unordered_map<vec3f, unsigned int, Point_Hash> unique_points_table;

		auto read_point = [](ifstream& file) -> vec3f
		{
			char f0[4];
			file.read(f0, 4);
			float x = *(float*)f0;
			file.read(f0, 4);
			float y = *(float*)f0;
			file.read(f0, 4);
			float z = *(float*)f0;
			return vec3f{ x,y,z };
		};

		auto insert_point = [&self, &unique_points_table](const vec3f& point, const vec3f& normal) -> void
		{
			auto itr = unique_points_table.find(point);
			if (itr == unique_points_table.end())
			{
				//revisit --wrong vertices normals--
				self.vertices.push_back(geo::Vertex{ point, normal, vec2f{} });
				unique_points_table.insert(std::make_pair(point, self.vertices.size() - 1));
				self.indices.push_back(self.vertices.size() - 1);
			}
			else
			{
				self.indices.push_back(itr->second);
			}
		};

		static const unsigned int header_size = 80;
		if (file)
		{
			char header[header_size];
			char count[4];
			file.read(header, header_size);
			file.read(count, 4);
			unsigned int tris_count = *(unsigned int*)count;
			self.vertices.reserve(3 * tris_count);
			self.indices.reserve(2 * tris_count);

			for (unsigned int i = 0; i < tris_count; ++i)
			{
				vec3f normal = read_point(file);
				insert_point(read_point(file), normal);
				insert_point(read_point(file), normal);
				insert_point(read_point(file), normal);
				char dummy[2];
				file.read(dummy, 2);
			}
		}
		else
			assert("can't read stl");

		return self;
	}
}