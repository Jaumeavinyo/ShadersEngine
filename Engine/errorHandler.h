#ifndef ERROR_HANDLER_
#define ERROR_HANDLER_

#define glCheckError() glCheckError_(__FILE__, __LINE__)


#include <glad/glad.h>

#include <cstdio>
#include <iostream>


GLenum glCheckError_(const char* file, int line);



#endif ERROR_HANDLER_
