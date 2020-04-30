#include "world/system/retrieving/Pick.h"
#include "world/component/Camera.h"
#include "world/component/Mesh.h"
#include "world/component/Transform.h"

#include "defs/Defs.h"

#include "utils/Defer.h"

using namespace glgpu;

namespace world
{
	namespace system
	{
		//internals
		math::vec3f
		_id_to_rgb(int id)
		{
			int r = (id & 0x00FF0000)>>16 ; int g = (id & 0x0000FF00)>>8; int b = id & 0x000000FF;
			return math::vec3f{(float)r, (float)g, (float)b};
		}

		int
		_rgb_to_id(int r, int g, int b)
		{
			return (r << 16) | (g << 8) | b;
		}

		//API
		Pick_System
		pick_sys_new()
		{
			Pick_System sys;
			sys.fb = framebuffer_create();
			sys.tex = texture2d_create(math::vec2f{ 1,1 }, INTERNAL_TEXTURE_FORMAT::RGBA, EXTERNAL_TEXTURE_FORMAT::RGBA, DATA_TYPE::UBYTE, false);
			
			return sys;
		}

		void
		pick_system_run(Pick_System sys, ecs::World& w, io::Input& i, rndr::Colored colored)
		{
			if (i.mouse[0] == true)
			{
				//fetch system req components
				auto cam = ecs::world_active_components<world::Camera>(w)[0];
				auto b_meshes = ecs::world_active_components<world::Mesh>(w);
				auto b_transforms = ecs::world_active_components<world::Transform>(w);
				auto e_mesh = ecs::world_active_entities<world::Mesh>(w);

				//reallocate tex if needed
				math::vec2f viewport = camera_viewport(cam);
				math::vec2f size = texture2d_size(sys.tex);
				if (viewport != size)
				{
					texture_free(sys.tex);
					sys.tex = texture2d_create(viewport, INTERNAL_TEXTURE_FORMAT::RGBA, EXTERNAL_TEXTURE_FORMAT::RGBA, DATA_TYPE::UBYTE, false);
				}

				//bind fb to draw in
				framebuffer_attach(sys.fb, sys.tex, FRAMEBUFFER_ATTACHMENT::COLOR0);
				framebuffer_bind(sys.fb);
				{
					//render colored with entity ids
					{
						frame_start();
						math::Mat4f view_proj = camera_view_proj(cam);
						rndr::colored_set(colored, viewport);
						for (int i = 0; i < b_meshes.size; ++i)
						{
							math::vec3f rgb = _id_to_rgb(e_mesh[i].id);
							rndr::colored_draw(colored, view_proj, &b_meshes[i], &b_transforms[i], math::vec4f{ rgb[0] / 255, rgb[1] / 255, rgb[2] / 255, 1.0f });
						}
					}

					//read pixel where mouse_pos
					{
						//i.mouse_x, i.mouse_y;
					}
				}
				framebuffer_unbind();
			}
		}

		void
		pick_sys_free(Pick_System sys)
		{
			glgpu::texture_free(sys.tex);
			glgpu::framebuffer_free(sys.fb);
		}
	};
};
