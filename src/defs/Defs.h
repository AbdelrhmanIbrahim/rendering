#pragma once

#include "math/Vector.h"

#include "world/component/Vertex.h"

#define HANDLE(NAME) typedef struct NAME##__ { int unused; } *NAME;
#define to_radian(degree) degree * 0.01745329251f
#define to_degree(radian) radian * 57.2957795131f
#define WIN_WIDTH 1000
#define WIN_HEIGHT 700
#define SHADOW_WIDTH 1024
#define SHADOW_HEIGHT 1024

constexpr int MAX_NUMBER_LIGHT_TYPE = 10;

enum class DEPTH_TEST
{
	LE,
	L,
	G
};

enum class FRAMEBUFFER_ATTACHMENT
{
	COLOR0,
	COLOR1,
	DEPTH_STENCIL
};

enum class EXTERNAL_TEXTURE_FORMAT
{
	RG,
	RGB,
	RGBA,
	DEPTH_STENCIL
};

enum class INTERNAL_TEXTURE_FORMAT
{
	RG16F,
	RGB,
	RGBA,
	RGB16F,
	DEPTH_STENCIL
};

enum class TEXTURE_FILTERING
{
	NEAREST,
	LINEAR,
	LINEAR_MIPMAP
};

enum class TEXTURE_SAMPLING
{
	REPEAT,
	MIRROR_REPEAT,
	CLAMP_TO_EDGE,
	CLAMP_TO_BORDER
};

enum class DATA_TYPE
{
	UBYTE,
	FLOAT,
	UINT_24_8
};

enum class TARGET
{
	TEXTURE_2D,
	CUBEMAP,
};

enum class Rendering
{
	PHONG,
	PBR,
	HIDDENLINE,
	COLORED
};

enum class STORAGE
{
	STATIC,
	DYNAMIC
};

enum class IMAGE_FORMAT
{
	BMP,
	PNG,
	JPG,
	HDR
};

enum class UNIFORM_TYPE
{
	FLOAT,
	VEC3,
	VEC4,
	MAT4,
	TEXTURE2D
};

enum class SHADER_STAGE
{
	VERTEX,
	GEOMETRY,
	PIXEL
};

//remove later
struct Unifrom_Float
{
	const char* uniform;
	float value;
};

//colored quad
constexpr static world::CVertex ctl{ math::Vec3f{ -1,  1, 1 }, math::Vec4f{0.7, 0.7, 0.7, 1} };
constexpr static world::CVertex cbl{ math::Vec3f{ -1, -1, 1 }, math::Vec4f{0.2, 0.2, 0.2, 1} };
constexpr static world::CVertex cbr{ math::Vec3f{  1, -1, 1 }, math::Vec4f{0.2, 0.2, 0.2, 1} };
constexpr static world::CVertex ctr{ math::Vec3f{  1,  1, 1 }, math::Vec4f{0.7, 0.7, 0.7, 1} };
constexpr static world::CVertex cbg_quad[6]
{
	ctl, cbl, cbr,
	ctl, cbr, ctr
};

//bg quad
constexpr static world::TVertex ttl{ math::Vec3f{ -1,  1, 1 }, math::Vec3f{}, math::Vec2f{0, 1} };
constexpr static world::TVertex tbl{ math::Vec3f{ -1, -1, 1 }, math::Vec3f{}, math::Vec2f{0, 0} };
constexpr static world::TVertex tbr{ math::Vec3f{  1, -1, 1 }, math::Vec3f{}, math::Vec2f{1, 0} };
constexpr static world::TVertex ttr{ math::Vec3f{  1,  1, 1 }, math::Vec3f{}, math::Vec2f{1, 1} };
constexpr static world::TVertex tbg_quad[6]
{
	ttl, tbl, tbr,
	ttl, tbr, ttr
};

//front quad
constexpr static world::TVertex fttl{ math::Vec3f{ -1,  1, -1 }, math::Vec3f{}, math::Vec2f{0, 1} };
constexpr static world::TVertex ftbl{ math::Vec3f{ -1, -1, -1 }, math::Vec3f{}, math::Vec2f{0, 0} };
constexpr static world::TVertex ftbr{ math::Vec3f{  1, -1, -1 }, math::Vec3f{}, math::Vec2f{1, 0} };
constexpr static world::TVertex fttr{ math::Vec3f{  1,  1, -1 }, math::Vec3f{}, math::Vec2f{1, 1} };
constexpr static world::TVertex tfr_quad[6]
{
	fttl, ftbl, ftbr,
	fttl, ftbr, fttr
};

//unit cube can be used in some algorithms like equirectangle mapping, irradiance map, etc..
constexpr static world::TVertex unit_cube[36] =
{
	//back
	world::TVertex{-1.0f, -1.0f, -1.0f},
	world::TVertex{1.0f, 1.0f, -1.0f},
	world::TVertex{1.0f, -1.0f, -1.0f},
	world::TVertex{1.0f, 1.0f, -1.0f},
	world::TVertex{-1.0f, -1.0f, -1.0f},
	world::TVertex{-1.0f, 1.0f, -1.0},

	//front
	world::TVertex{-1.0f, -1.0f, 1.0},
	world::TVertex{1.0f, -1.0f, 1.0f},
	world::TVertex{1.0f, 1.0f, 1.0f,},
	world::TVertex{1.0f, 1.0f, 1.0f,},
	world::TVertex{-1.0f, 1.0f, 1.0f},
	world::TVertex{-1.0f, -1.0f, 1.0},

	//left
	world::TVertex{-1.0f, 1.0f, 1.0f},
	world::TVertex{-1.0f, 1.0f, -1.0f},
	world::TVertex{-1.0f, -1.0f, -1.0f},
	world::TVertex{-1.0f, -1.0f, -1.0f},
	world::TVertex{-1.0f, -1.0f, 1.0},
	world::TVertex{-1.0f, 1.0f, 1.0f},

	//right
	world::TVertex{1.0f, 1.0f, 1.0f,},
	world::TVertex{1.0f, -1.0f, -1.0},
	world::TVertex{1.0f, 1.0f, -1.0f},
	world::TVertex{1.0f, -1.0f, -1.0f},
	world::TVertex{1.0f, 1.0f, 1.0f,},
	world::TVertex{1.0f, -1.0f, 1.0f},

	//bottom
	world::TVertex{-1.0f, -1.0f, -1.0f},
	world::TVertex{1.0f, -1.0f, -1.0f},
	world::TVertex{1.0f, -1.0f, 1.0f},
	world::TVertex{1.0f, -1.0f, 1.0f},
	world::TVertex{-1.0f, -1.0f, 1.0f},
	world::TVertex{-1.0f, -1.0f, -1.0f},

	//top
	world::TVertex{-1.0f, 1.0f, -1.0f},
	world::TVertex{1.0f, 1.0f, 1.0f,},
	world::TVertex{1.0f, 1.0f, -1.0f},
	world::TVertex{1.0f, 1.0f, 1.0f,},
	world::TVertex{-1.0f, 1.0f, -1.0f},
	world::TVertex{-1.0f, 1.0f, 1.0f}
};

constexpr static world::TVertex quad_ndc[6] =
{
	world::TVertex{-1.0f,  1.0f, 0.0f,  math::Vec3f{0,0,1}, 0.0f, 1.0f},
	world::TVertex{-1.0f, -1.0f, 0.0f,  math::Vec3f{0,0,1}, 0.0f, 0.0f},
	world::TVertex{ 1.0f,  1.0f, 0.0f,  math::Vec3f{0,0,1}, 1.0f, 1.0f},

	world::TVertex{ 1.0f,  1.0f, 0.0f,  math::Vec3f{0,0,1}, 1.0f, 1.0f},
	world::TVertex{-1.0f, -1.0f, 0.0f,  math::Vec3f{0,0,1}, 0.0f, 0.0f},
	world::TVertex{ 1.0f, -1.0f, 0.0f,  math::Vec3f{0,0,1}, 1.0f, 0.0f}
};
