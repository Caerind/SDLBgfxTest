#include "Sprite.hpp"

#include "BgfxWrapper.hpp"

namespace NAMESPACE_NAME
{

bgfx::VertexLayout Sprite::Vertex::kLayout;
const U16 Sprite::kIndices[6] = { 0, 1, 2, 0, 2, 3 };
Shader Sprite::kShader;
bgfx::IndexBufferHandle Sprite::kIndexBuffer;

Sprite::Sprite()
	: mMatrix()
	, mVertices()
	, mRectX(0)
	, mRectY(0)
	, mRectW(0)
	, mRectH(0)
	, mBuffer(BGFX_INVALID_HANDLE)
{
	mMatrix[0] = 1.0f;
	mMatrix[1] = 0.0f;
	mMatrix[2] = 0.0f;
	mMatrix[3] = 0.0f;
	mMatrix[4] = 0.0f;
	mMatrix[5] = 1.0f;
	mMatrix[6] = 0.0f;
	mMatrix[7] = 0.0f;
	mMatrix[8] = 0.0f;
	mMatrix[9] = 0.0f;
	mMatrix[10] = 1.0f;
	mMatrix[11] = 0.0f;
	mMatrix[12] = 0.0f;
	mMatrix[13] = 0.0f;
	mMatrix[14] = 0.0f;
	mMatrix[15] = 1.0f;
}

Sprite::~Sprite()
{
	if (bgfx::isValid(mBuffer))
	{
		bgfx::destroy(mBuffer);
	}
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

void Sprite::GetLocalBounds(F32& minX, F32& minY, F32& maxX, F32& maxY) const
{ 
	minX = 0.0f;
	minY = 0.0f;
	maxX = static_cast<F32>(std::abs(mRectW));
	maxY = static_cast<F32>(std::abs(mRectH));
}

void Sprite::GetGlobalBounds(F32& minX, F32& minY, F32& maxX, F32& maxY) const
{
	GetLocalBounds(minX, minY, maxX, maxY);
	// TODO : Apply transform from matrix
}

void Sprite::Render() const
{
	if (bgfx::isValid(mBuffer))
	{
		bgfx::setTransform(mMatrix);
		bgfx::setVertexBuffer(BgfxWrapper::kClearView, mBuffer);
		bgfx::setIndexBuffer(kIndexBuffer);
		bgfx::setState(BGFX_STATE_DEFAULT);
		bgfx::submit(BgfxWrapper::kClearView, kShader.GetHandle());
	}
}

void Sprite::Update()
{
	F32 minX, minY, maxX, maxY;
	GetLocalBounds(minX, minY, maxX, maxY);
	const F32 left = static_cast<F32>(mRectX);
	const F32 right = left + mRectW;
	const F32 top = static_cast<F32>(mRectY);
	const F32 bottom = top + mRectH;

	mVertices[0].x = minX;
	mVertices[0].y = minY;
	mVertices[0].z = 0.0f;
	mVertices[0].u = left;
	mVertices[0].v = top;

	mVertices[1].x = maxX;
	mVertices[1].y = minY;
	mVertices[1].z = 0.0f;
	mVertices[1].u = right;
	mVertices[1].v = top;

	mVertices[2].x = maxX;
	mVertices[2].y = maxY;
	mVertices[2].z = 0.0f;
	mVertices[2].u = right;
	mVertices[2].v = bottom;

	mVertices[3].x = minX;
	mVertices[3].y = maxY;
	mVertices[3].z = 0.0f;
	mVertices[3].u = left;
	mVertices[3].v = bottom;

	// TODO : Does is need to be recreated each time or not ?
	if (!bgfx::isValid(mBuffer))
	{
		mBuffer = bgfx::createVertexBuffer(bgfx::makeRef(mVertices, sizeof(mVertices)), Vertex::kLayout);
	}
}

bool Sprite::InitializeSprites()
{
	Vertex::kLayout
		.begin()
		.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
		.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float, true)
		.end();

	if (!kShader.Initialize("shaders/dx11/sprite.vs.bin", "shaders/dx11/sprite.fs.bin"))
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
	kShader.Destroy();
	bgfx::destroy(kIndexBuffer);
	return true;
}

} // namespace NAMESPACE_NAME