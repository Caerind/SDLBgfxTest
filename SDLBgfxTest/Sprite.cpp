#include "Sprite.hpp"

#include "BgfxWrapper.hpp"

namespace NAMESPACE_NAME
{

bgfx::VertexLayout Sprite::Vertex::kLayout;
const U16 Sprite::kIndices[6] = { 0, 2, 1, 0, 3, 2 };
Shader Sprite::kShader;
bgfx::UniformHandle Sprite::kUniformTexture;
bgfx::IndexBufferHandle Sprite::kIndexBuffer;

Sprite::Sprite()
	: mVertices()
	, mTextureRect()
	, mBuffer(BGFX_INVALID_HANDLE)
	, mTexture(nullptr)
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
		mTexture = &texture;
		SetTextureRect(Recti(0, 0, texture.GetWidth(), texture.GetHeight()));
	}
}

const Texture* Sprite::GetTexture() const
{
	return mTexture;
}

void Sprite::SetTextureRect(const Recti& textureRect)
{
	if (mTextureRect != textureRect)
	{
		mTextureRect = textureRect;
		Update();
	}
}

const Recti& Sprite::GetTextureRect() const
{
	return mTextureRect;
}

Rectf Sprite::GetLocalBounds() const
{
	return Rectf(0.0f, 0.0f, 1.0f, -1.0f);
}

Rectf Sprite::GetGlobalBounds() const
{
	// TODO : Transform
	return GetLocalBounds();
}

void Sprite::Render() const
{
	if (bgfx::isValid(mBuffer) && mTexture != nullptr)
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
	const Rectf localBounds = GetLocalBounds();
	const F32 localBoundsRight = localBounds.left + localBounds.width;
	const F32 localBoundsBottom = localBounds.top + localBounds.height;

	// TODO : Find how to handle texture errors here
	assert(mTexture != nullptr);
	assert(mTexture->GetWidth() > 0);
	const F32 oneOverTexWidth = 1.0f / static_cast<F32>(mTexture->GetWidth());
	assert(mTexture->GetHeight() > 0);
	const F32 oneOverTexHeight = 1.0f / static_cast<F32>(mTexture->GetHeight());
	const F32 left = static_cast<F32>(mTextureRect.left) * oneOverTexWidth;
	const F32 right = static_cast<F32>(mTextureRect.left + mTextureRect.width) * oneOverTexWidth;
	const F32 top = static_cast<F32>(mTextureRect.top) * oneOverTexHeight;
	const F32 bottom = static_cast<F32>(mTextureRect.top + mTextureRect.height) * oneOverTexHeight;

	mVertices[0].pos = Vector2f(localBounds.left, localBounds.top);
	mVertices[0].texCoords = Vector2f(left, top);
	mVertices[1].pos = Vector2f(localBoundsRight, localBounds.top);
	mVertices[1].texCoords = Vector2f(right, top);
	mVertices[2].pos = Vector2f(localBoundsRight, localBoundsBottom);
	mVertices[2].texCoords = Vector2f(right, bottom);
	mVertices[3].pos = Vector2f(localBounds.left, localBoundsBottom);
	mVertices[3].texCoords = Vector2f(left, bottom);

	// TODO : Use dynamic vertex buffer instead ?
	// TODO : => Give the choise to the user using template boolean parameter
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