#pragma once

#include <cstdint>
#include <raylib.h>

// == TYPE ALIAS ==
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef Vector2 vec2;
typedef Vector3 vec3;
typedef Vector4 vev4;

typedef Matrix mat4;


// == STRUCTS ==

struct WindowData {
	WindowData(i32 width, i32 height, const char* title, i32 targetFps = 60) {
		this->width = width;
		this->height = height;
		this->title = title;
		this->targetFPS = targetFps;
	}

	i32 targetFPS;
	i32 width;
	i32 height;
	const char* title;
};

struct RenderData {
	RenderData() {
		windowData = nullptr;
	}

	WindowData* windowData;
};

// == RENDER STRUCTS ==

struct Square {
	vec2 pos;
	vec2 size;
	Color color;
};
