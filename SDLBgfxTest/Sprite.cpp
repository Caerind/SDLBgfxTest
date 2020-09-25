#include "Sprite.hpp"

#include "BgfxWrapper.hpp"

namespace NAMESPACE_NAME
{
	
const bgfx::VertexLayout Sprite::Vertex::kLayout = []()
{
	bgfx::VertexLayout layout;
	layout.begin();
	layout.add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float);
	layout.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true);
	layout.end();
	return layout;
}();

const Sprite::Vertex Sprite::kVertices[4] =
{
	{ 0.0f, 0.0f, 0xff000000 },
	{ 1.0f, 0.0f, 0xff000000 },
	{ 1.0f, 1.0f, 0xff000000 },
	{ 0.0f, 1.0f, 0xff000000 }
}; 

const U16 Sprite::kIndices[6] =
{
	0, 1, 2,
	0, 2, 3
};

Shader Sprite::kShader;
bgfx::VertexBufferHandle Sprite::kVertexBuffer;
bgfx::IndexBufferHandle Sprite::kIndexBuffer;

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
}

void Sprite::Render()
{
	bgfx::setVertexBuffer(BgfxWrapper::kClearView, kVertexBuffer);
	bgfx::setIndexBuffer(kIndexBuffer);
	bgfx::submit(BgfxWrapper::kClearView, kShader.GetHandle());
}

bool Sprite::InitializeSprites()
{
	// TODO : Sprite shaders
	/*
	if (!kShader.Initialize("", ""))
	{
		return false;
	}
	*/

	kVertexBuffer = bgfx::createVertexBuffer(bgfx::makeRef(Sprite::kVertices, sizeof(Sprite::kVertices)), Sprite::Vertex::kLayout);
	kIndexBuffer = bgfx::createIndexBuffer(bgfx::makeRef(Sprite::kIndices, sizeof(Sprite::kIndices)));
	if (!bgfx::isValid(kVertexBuffer) || !bgfx::isValid(kIndexBuffer))
	{
		return false;
	}

	return true;
}

} // namespace NAMESPACE_NAME