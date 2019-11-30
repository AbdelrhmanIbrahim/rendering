#pragma once

#define HANDLE(NAME) typedef struct NAME##__ { int unused; } *NAME;
#define to_radian(degree) degree * 0.01745329251f
#define to_degree(radian) radian * 57.2957795131f
#define WIN_WIDTH 1400
#define WIN_HEIGHT 1000