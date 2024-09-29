#include "Texture2D.h"

Texture2D::Texture2D(const char* filePath, int filterMode, int wrapMode) {
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);

	unsigned char* fileData = stbi_load(filePath, &width, &height, nullptr, 4);
	
	if (fileData) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, fileData);
		glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "Texture loaded from " + std::string(filePath) + "\n";
	}
	else
		throw std::runtime_error("Failed to load texture: " + std::string(filePath));
	stbi_image_free(fileData);
}

Texture2D::~Texture2D() {
	glDeleteTextures(1, &texture);
	std::cout << "Texture deleted\n";
}

void Texture2D::Bind(unsigned int slot) {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, texture);
}
