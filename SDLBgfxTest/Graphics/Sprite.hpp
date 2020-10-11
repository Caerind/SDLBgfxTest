#pragma once

#include "../EngineIntegration.hpp"

#include <bgfx/bgfx.h>

#include "../Math/Rect.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

namespace NAMESPACE_NAME
{

class Sprite
{
public:
	Sprite();
	~Sprite();

	void SetTexture(const Texture& texture);
	const Texture* GetTexture() const;

	void SetTextureRect(const Recti& textureRect);
	const Recti& GetTextureRect() const;

	Rectf GetLocalBounds() const;
	Rectf GetGlobalBounds() const;

	void Render(const bgfx::ViewId& viewId) const;

private:
	void Update();

private:
	struct Vertex
	{
		Vector2f pos;
		Vector2f texCoords;

		static bgfx::VertexLayout kLayout;
	}; 

	Vertex mVertices[4];
	Recti mTextureRect;
	bgfx::VertexBufferHandle mBuffer;
	const Texture* mTexture;

private:
	friend class BgfxWrapper;

	static const U16 kIndices[6];
	static Shader kShader;
	static bgfx::UniformHandle kUniformTexture;
	static bgfx::IndexBufferHandle kIndexBuffer;
	static bool InitializeSprites();
	static bool ReleaseSprites();
};

} // namespace NAMESPACE_NAME