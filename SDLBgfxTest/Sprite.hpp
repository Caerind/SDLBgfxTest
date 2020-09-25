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

	//void SetTexture(TexturePtr texture);
	//TexturePtr GetTexture() const;

	void SetTextureRect(I32 x, I32 y, I32 w, I32 h);
	void GetTextureRect(I32& x, I32& y, I32& w, I32& h) const;

	void GetLocalBounds(F32& minX, F32& minY, F32& maxX, F32& maxY) const;
	void GetGlobalBounds(F32& minX, F32& minY, F32& maxX, F32& maxY) const;

	void Render() const;

private:
	void Update();

private:
	struct Vertex
	{
		F32 x;
		F32 y;
		F32 z;
		F32 u;
		F32 v;

		static bgfx::VertexLayout kLayout;
	};

	F32 mMatrix[16];
	Vertex mVertices[4];
	//TexturePtr mTexture;
	I32 mRectX;
	I32 mRectY;
	I32 mRectW;
	I32 mRectH;
	bgfx::VertexBufferHandle mBuffer; // TODO : Move this outside to batch

private:
	friend class BgfxWrapper;

	static const U16 kIndices[6];
	static Shader kShader;
	static bgfx::IndexBufferHandle kIndexBuffer;
	static bool InitializeSprites();
	static bool ReleaseSprites();
};

} // namespace NAMESPACE_NAME