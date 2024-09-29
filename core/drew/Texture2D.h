#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <iostream>
#include "../ew/external/stb_image.h"
#include "../ew/external/glad.h"

class Texture2D {

public:

    Texture2D(const char* filePath, int filterMode, int wrapMode);
    ~Texture2D();

    void Bind(unsigned int slot = 0);

private:

    unsigned int texture;

    int width, height;
};

#endif TEXTURE2D_H