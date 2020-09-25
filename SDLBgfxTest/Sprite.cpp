#include "Sprite.hpp"

#include "BgfxWrapper.hpp"

namespace NAMESPACE_NAME
{

bgfx::VertexLayout Sprite::Vertex::kLayout;
const U16 Sprite::kIndices[6] = { 0, 1, 2, 0, 2, 3 };
Shader Sprite::kShader;
bgfx::UniformHandle Sprite::kUniformTexture;
bgfx::IndexBufferHandle Sprite::kIndexBuffer;

Sprite::Sprite()
	: mVertices()
	, mTexture(nullptr)
	, mRectX(0)
	, mRectY(0)
	, mRectW(0)
	, mRectH(0)
	, mBuffer(BGFX_INVALID_HANDLE)
{
}

Sprite::~Sprite()
{
	if (bgfx::isValid(mBuffer))
	{
		bgfx::destroy(mBuffer);
	}
}

void Sprite::SetTexture(const Texture& texture)
{
	if (texture.IsValid())
	{
		if (mTexture == nullptr)
		{
			mTexture = &texture;
			SetTextureRect(0, 0, texture.GetWidth(), texture.GetHeight());
		}
		else
		{
			mTexture = &texture;
		}
	}
}

const Texture* Sprite::GetTexture() const
{
	return mTexture;
}

void Sprite::SetTextureRect(I32 x, I32 y, I32 w, I32 h)
{
	mRectX = x;
	mRectY = y;
	mRectW = w;
	mRectH = h;
	Update();
}

void Sprite::GetTextureRect(I32& x, I32& y, I32& w, I32& h) const
{
	x = mRectX;
	y = mRectY;
	w = mRectW;
	h = mRectH;
}

void Sprite::GetBounds(F32& minX, F32& minY, F32& maxX, F32& maxY) const
{
	minX = 0.0f;
	minY = 0.0f;
	maxX = static_cast<F32>(std::abs(mRectW));
	maxY = static_cast<F32>(std::abs(mRectH));
}

void Sprite::Render() const
{
	if (bgfx::isValid(mBuffer) && mTexture != nullptr && mTexture->IsValid())
	{
		// Common to all sprites
		bgfx::setIndexBuffer(kIndexBuffer);
		bgfx::setState(BGFX_STATE_DEFAULT);

		// Specific to this sprite
		bgfx::setVertexBuffer(BgfxWrapper::kClearView, mBuffer);
		bgfx::setTexture(0, kUniformTexture, mTexture->GetHandle());
		bgfx::submit(BgfxWrapper::kClearView, kShader.GetHandle());
	}
}

void Sprite::Update()
{
	assert(mTexture != nullptr);

	assert(mTexture->GetWidth() > 0);
	const F32 oneOverTexWidth = 1.0f / static_cast<F32>(mTexture->GetWidth());

	assert(mTexture->GetHeight() > 0);
	const F32 oneOverTexHeight = 1.0f / static_cast<F32>(mTexture->GetHeight());

	F32 minX, minY, maxX, maxY;
	GetBounds(minX, minY, maxX, maxY);
	maxX *= oneOverTexWidth;
	maxY *= oneOverTexHeight;

	const F32 left = static_cast<F32>(mRectX) * oneOverTexWidth;
	const F32 right = (static_cast<F32>(mRectX) + mRectW) * oneOverTexWidth;
	const F32 top = static_cast<F32>(mRectY) * oneOverTexHeight;
	const F32 bottom = (static_cast<F32>(mRectY) + mRectH) * oneOverTexHeight;

	mVertices[0].x = minX;
	mVertices[0].y = minY;
	mVertices[0].u = left;
	mVertices[0].v = top;

	mVertices[1].x = maxX;
	mVertices[1].y = minY;
	mVertices[1].u = right;
	mVertices[1].v = top;

	mVertices[2].x = maxX;
	mVertices[2].y = maxY;
	mVertices[2].u = right;
	mVertices[2].v = bottom;

	mVertices[3].x = minX;
	mVertices[3].y = maxY;
	mVertices[3].u = left;
	mVertices[3].v = bottom;

	if (bgfx::isValid(mBuffer))
	{
		bgfx::destroy(mBuffer);
	}
	mBuffer = bgfx::createVertexBuffer(bgfx::makeRef(mVertices, sizeof(mVertices)), Vertex::kLayout);
}

bool Sprite::InitializeSprites()
{
	Vertex::kLayout
		.begin()
		.add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
		.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
		.end();

	if (!kShader.Initialize("sprite.vs.bin", "sprite.fs.bin"))
	{
		return false;
	}

	kUniformTexture = bgfx::createUniform("s_uniformTexture", bgfx::UniformType::Sampler);
	if (!bgfx::isValid(kUniformTexture))
	{
		return false;
	}

	kIndexBuffer = bgfx::createIndexBuffer(bgfx::makeRef(Sprite::kIndices, sizeof(Sprite::kIndices)));
	if (!bgfx::isValid(kIndexBuffer))
	{
		return false;
	}

	return true;
}

bool Sprite::ReleaseSprites()
{
	bgfx::destroy(kIndexBuffer);
	bgfx::destroy(kUniformTexture);
	kShader.Destroy();
	return true;
}

} // namespace NAMESPACE_NAME