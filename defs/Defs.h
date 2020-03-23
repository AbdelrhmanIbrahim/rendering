#pragma once

#define HANDLE(NAME) typedef struct NAME##__ { int unused; } *NAME;
#define to_radian(degree) degree * 0.01745329251f
#define to_degree(radian) radian * 57.2957795131f
#define WIN_WIDTH 1000
#define WIN_HEIGHT 700
#define SHADOW_WIDTH 1024
#define SHADOW_HEIGHT 1024

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

//remove later
struct Unifrom_Float
{
	const char* uniform;
	float value;
};