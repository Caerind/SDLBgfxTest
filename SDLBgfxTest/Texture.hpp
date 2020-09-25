#pragma once

#include <bgfx/bgfx.h>

#include "EngineIntegration.hpp"

namespace NAMESPACE_NAME
{

class Texture
{
public:
	Texture();
	~Texture();

	bool Initialize(const char* filename, uint64_t flags = BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE);
	void Destroy();

	bool IsValid() const;

	I32 GetWidth() const;
	I32 GetHeight() const;

	// TODO : Remove/Clean if possible
	bgfx::TextureHandle GetHandle() const { return mTexture; }

private:
	static void ImageReleaseCallback(void* ptr, void* userData);

private:
	bgfx::TextureHandle mTexture;
	I32 mWidth;
	I32 mHeight;
};

} // namespace NAMESPACE_NAME