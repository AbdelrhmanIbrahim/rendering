#include "world/system/retrieving/Pick.h"
#include "world/component/Camera.h"
#include "world/component/Mesh.h"
#include "world/component/Transform.h"

#include "math/Vector.h"
#include "math/Matrix.h"

#include "io/Image.h"

#include "gl/glgpu.h"

using namespace glgpu;
using namespace io;

namespace world
{
	namespace system
	{
		//gpu picking
		struct IPick_System
		{
			glgpu::Framebuffer fb;
			glgpu::Texture tex;
			io::Image pixels;
		};

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
			IPick_System* sys = new IPick_System;
			sys->fb = framebuffer_create();
			sys->tex = texture2d_create(math::vec2f{ 1,1 }, INTERNAL_TEXTURE_FORMAT::RGBA, EXTERNAL_TEXTURE_FORMAT::RGBA, DATA_TYPE::UBYTE, false);
			sys->pixels = image_new(4, math::vec2f{ 1,1 });

			return sys;
		}

		Pick_Info
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
				math::vec2f size = texture2d_size(sys->tex);
				if (viewport != size)
				{
					texture2d_resize(sys->tex, viewport, INTERNAL_TEXTURE_FORMAT::RGBA, EXTERNAL_TEXTURE_FORMAT::RGBA, DATA_TYPE::UBYTE);
					image_resize(sys->pixels, viewport);
				}

				//bind fb to render colored with entity ids into
				framebuffer_bind(sys->fb);
				framebuffer_attach(sys->fb, sys->tex, FRAMEBUFFER_ATTACHMENT::COLOR0);
				{
					frame_start(1, 1, 1);
					math::Mat4f view_proj = camera_view_proj(cam);
					rndr::colored_set(colored, viewport);
					for (int i = 0; i < b_meshes.size; ++i)
					{
						math::vec3f rgb = _id_to_rgb(e_mesh[i].id);
						rndr::colored_draw(colored, view_proj, &b_meshes[i], &b_transforms[i], math::vec4f{ rgb[0] / 255, rgb[1] / 255, rgb[2] / 255, 1.0f });
					}
				}
				framebuffer_unbind();

				//read pixel where mouse_pos
				{
					texture2d_unpack(sys->tex, sys->pixels, EXTERNAL_TEXTURE_FORMAT::RGBA, DATA_TYPE::UBYTE);
					int index = 4 * (sys->pixels.width * (sys->pixels.height - i.mouse_y) + i.mouse_x);
					unsigned char* bytes = (unsigned char*)sys->pixels.data;
					unsigned char r = bytes[index];
					unsigned char g = bytes[index+1];
					unsigned char b = bytes[index+2];
					int id = _rgb_to_id(r, g, b);

					//background
					if (id != 0xFFFFFF)
						return Pick_Info{ PICKING::OBJECT, id};
					else
						return Pick_Info{ PICKING::BACKGROUND };
				}
			}

			return Pick_Info{PICKING::NONE};
		}

		void
		pick_sys_free(Pick_System sys)
		{
			texture_free(sys->tex);
			framebuffer_free(sys->fb);
			image_free(sys->pixels);
		}
	};
};
