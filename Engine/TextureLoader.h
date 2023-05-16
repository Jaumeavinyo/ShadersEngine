#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stb_image.h>
#include <stb_image_write.h>

#include "Code/engine.h"

u32 LoadTexture2D(App* app, const char* filepath);
Image LoadImage(const char* filename);
void FreeImage(Image image);
GLuint CreateTexture2DFromImage(Image image);
