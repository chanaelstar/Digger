// #include "texture.hpp"

// #define STB_IMAGE_IMPLEMENTATION
// #include "tools/stb_image.h"

// using namespace glbasimac;

// GLBI_Texture createOneTexture(const char *filename)
// {
//     int x{};
//     int y{};
//     int n{};

//     stbi_set_flip_vertically_on_load(true);
//     unsigned char *pixels{stbi_load(filename, &x, &y, &n, 0)};
//     std::cout << "Image " << filename << (pixels != nullptr ? "" : " not") << " loaded." << std::endl;

//     GLBI_Texture texture{};
//     texture.createTexture();
//     texture.attachTexture();
//     texture.setParameters(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//     texture.loadImage(x, y, n, pixels);
//     texture.detachTexture();
//     stbi_image_free(pixels);

//     return texture;
// };