#pragma once

#include <bgfx/bgfx.h>

#include "EngineIntegration.hpp"
#include "Shader.hpp"

namespace NAMESPACE_NAME
{

class Sprite
{
public:
	Sprite();
	~Sprite();

	void Render();

private:
	friend class BgfxWrapper;

	struct Vertex
	{
		F32 x;
		F32 y;
		U32 abgr;

		static const bgfx::VertexLayout kLayout;
	};
	static const Vertex kVertices[4]; 
	static const U16 kIndices[6];
	static Shader kShader;
	static bgfx::VertexBufferHandle kVertexBuffer;
	static bgfx::IndexBufferHandle kIndexBuffer;
	static bool InitializeSprites();
};

} // namespace NAMESPACE_NAME