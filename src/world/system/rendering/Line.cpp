#include "world/system/rendering/Line.h"
#include "world/component/Camera.h"

#include "math/Vector.h"

#include "world/component/Vertex.h"

#include <vector>

namespace world
{
	namespace system
	{
		constexpr static int length = 12;
		static std::vector<CVertex> lines;

		//internals
		void
		_grid_init()
		{
			//grid
			lines.reserve(4 * length + 6);
			for (float x = -length/2; x <= length/2; ++x)
			{
				lines.emplace_back(CVertex{ math::vec3f{x, 0, -length / 2}, math::vec4f{1,1,0,1} });
				lines.emplace_back(CVertex{ math::vec3f{x, 0, length / 2}, math::vec4f{1,1,0,1} });
			}

			for (float z = -length/2; z <= length/2; ++z)
			{
				lines.emplace_back(CVertex{ math::vec3f{-length / 2, 0, z}, math::vec4f{1,1,0,1} });
				lines.emplace_back(CVertex{ math::vec3f{length / 2, 0, z}, math::vec4f{1,1,0,1} });
			}

			//world ref axes
			lines.emplace_back(CVertex{ math::vec3f{0, 0.01, 0}, math::vec4f{1,0,0,1} });
			lines.emplace_back(CVertex{ math::vec3f{1, 0, 0}, math::vec4f{1,0,0,1} });

			lines.emplace_back(CVertex{ math::vec3f{0, 0.01, 0}, math::vec4f{0,1,0,1} });
			lines.emplace_back(CVertex{ math::vec3f{0, 1, 0}, math::vec4f{0,1,0,1} });

			lines.emplace_back(CVertex{ math::vec3f{0, 0.01, 0}, math::vec4f{0,0,1,1} });
			lines.emplace_back(CVertex{ math::vec3f{0, 0, 1}, math::vec4f{0,0,1,1} });
		}

		void
		line_sys_run(rndr::Line line, ecs::World& w)
		{
			if(lines.empty())
				_grid_init();

			//fetch system req components
			auto cam = ecs::world_active_components<world::Camera>(w)[0];

			//grid with axes
			for (int i = 0; i < lines.size(); i += 2)
				rndr::line_append(line, lines[i], lines[i+1]);

			rndr::line_set(line, camera_view_proj(cam));
			rndr::line_draw(line);
		}
	};
};