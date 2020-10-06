#pragma once

#include "../EngineIntegration.hpp"

#include <bgfx/bgfx.h>

#include "../Math/Vector3.hpp"

namespace NAMESPACE_NAME
{

class Texture
{
public:
	Texture();
	~Texture();

	bool Initialize(const char* filename, U64 flags = BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE);
	void Destroy();

	bool IsValid() const;

	U32 GetMemSize() const;
	Vector3u GetSize() const;
	U32 GetWidth() const;
	U32 GetHeight() const;
	U32 GetDepth() const;
	U32 GetLayers() const;
	U32 GetMips() const;
	U32 GetBitsPerPixel() const;
	bool IsCubeMap() const;

	// TODO : Remove/Clean
	bgfx::TextureHandle GetHandle() const { return mTexture; }

private:
	static void ImageReleaseCallback(void* ptr, void* userData);

private:
	bgfx::TextureHandle mTexture;
	bgfx::TextureInfo mInfo;
};

} // namespace NAMESPACE_NAME