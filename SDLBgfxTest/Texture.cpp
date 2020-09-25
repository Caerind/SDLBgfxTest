#include "Texture.hpp"

#include <bx/allocator.h>
#include <bimg/decode.h>

namespace NAMESPACE_NAME
{

Texture::Texture()
	: mTexture(BGFX_INVALID_HANDLE)
{
}

Texture::~Texture()
{
	Destroy();
}

bool Texture::Initialize(const char* filename, uint64_t flags)
{
	if (bgfx::isValid(mTexture))
	{
		bgfx::destroy(mTexture);
	}

	mTexture = BGFX_INVALID_HANDLE;

	FILE* file = fopen(filename, "rb");
	if (file == nullptr)
	{
		return false;
	}
	fseek(file, 0, SEEK_END);
	std::size_t size = static_cast<std::size_t>(ftell(file));
	fseek(file, 0, SEEK_SET);
	char* data = (char*)malloc(size);
	if (data == nullptr)
	{
		return false;
	}
	fread(data, 1, size, file);
	fclose(file);

	static bx::DefaultAllocator defaultAllocator;
	bx::AllocatorI* allocator = &defaultAllocator;
	bimg::ImageContainer* imageContainer = bimg::imageParse(allocator, (const void*)data, (uint32_t)size);
	if (imageContainer == nullptr)
	{
		return false;
	}

	const bgfx::Memory* mem = bgfx::makeRef(imageContainer->m_data, imageContainer->m_size, ImageReleaseCallback, imageContainer);
	free(data);

	if (imageContainer->m_cubeMap)
	{
		mTexture = bgfx::createTextureCube(
			uint16_t(imageContainer->m_width)
			, 1 < imageContainer->m_numMips
			, imageContainer->m_numLayers
			, bgfx::TextureFormat::Enum(imageContainer->m_format)
			, flags
			, mem
		);
	}
	else if (1 < imageContainer->m_depth)
	{
		mTexture = bgfx::createTexture3D(
			uint16_t(imageContainer->m_width)
			, uint16_t(imageContainer->m_height)
			, uint16_t(imageContainer->m_depth)
			, 1 < imageContainer->m_numMips
			, bgfx::TextureFormat::Enum(imageContainer->m_format)
			, flags
			, mem
		);
	}
	else if (bgfx::isTextureValid(0, false, imageContainer->m_numLayers, bgfx::TextureFormat::Enum(imageContainer->m_format), flags))
	{
		mTexture = bgfx::createTexture2D(
			uint16_t(imageContainer->m_width)
			, uint16_t(imageContainer->m_height)
			, 1 < imageContainer->m_numMips
			, imageContainer->m_numLayers
			, bgfx::TextureFormat::Enum(imageContainer->m_format)
			, flags
			, mem
		);
	}

	if (bgfx::isValid(mTexture))
	{
		bgfx::setName(mTexture, filename);
		/*
		mFlags = flags;
		mOrientation = imageContainer->m_orientation;
		bgfx::calcTextureSize(
			mInfo
			, uint16_t(imageContainer->m_width)
			, uint16_t(imageContainer->m_height)
			, uint16_t(imageContainer->m_depth)
			, imageContainer->m_cubeMap
			, 1 < imageContainer->m_numMips
			, imageContainer->m_numLayers
			, bgfx::TextureFormat::Enum(imageContainer->m_format)
			);
		*/

		// TODO : This is not clean but ok for now
		mWidth = static_cast<I32>(imageContainer->m_width);
		mHeight = static_cast<I32>(imageContainer->m_height);

		return true;
	}
	else
	{

		mWidth = 0;
		mHeight = 0;

		return false;
	}
}

void Texture::Destroy()
{
	if (bgfx::isValid(mTexture))
	{
		bgfx::destroy(mTexture);
		mTexture = BGFX_INVALID_HANDLE;
	}
}

bool Texture::IsValid() const
{
	return bgfx::isValid(mTexture);
}

I32 Texture::GetWidth() const
{
	return mWidth;
}

I32 Texture::GetHeight() const
{
	return mHeight;
}

void Texture::ImageReleaseCallback(void* ptr, void* userData)
{
	BX_UNUSED(ptr);
	bimg::ImageContainer* imageContainer = (bimg::ImageContainer*)userData;
	bimg::imageFree(imageContainer);
}

} // namespace NAMESPACE_NAME