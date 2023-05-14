#pragma once
#include "Code/platform.h"
#include <glad/glad.h>
#include "Texture.h"
#include "Code/engine.h"

Image LoadImage(const char* filename);
void FreeImage(Image image);
GLuint CreateTexture2DFromImage(Image image);
u32 LoadTexture2D(App* app, const char* filepath);