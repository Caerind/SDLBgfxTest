#pragma once

#include "../EngineIntegration.hpp"

#include <vector>

#include <bgfx/bgfx.h>

#include "../Math/Rect.hpp"
#include "Shader.hpp"
#include "Tileset.hpp"

namespace NAMESPACE_NAME
{

class Tilemap
{
public:
	Tilemap();
	~Tilemap();

	void SetTileset(const Tileset& tileset);
	const Tileset* GetTileset() const;

	void SetSize(const Vector2u& size);
	const Vector2u& GetSize() const;

	void SetTile(const Vector2u& tileCoords, U32 tileID);
	U32 GetTile(const Vector2u& tileCoords) const;

	Rectf GetLocalBounds() const;
	Rectf GetGlobalBounds() const;

	void Render(const bgfx::ViewId& viewId) const;

private:
	void UpdateTexCoords();
	void UpdateTileTexCoords(U32 tileIndex);
	void UpdateIndexBuffer();
	void UpdateVertexBuffer();

private:
	struct Vertex
	{
		Vector2f pos;
		Vector2f texCoords;

		static bgfx::VertexLayout kLayout;
	};
	Vector2u mSize;
	std::vector<U32> mTiles;
	std::vector<Vertex> mVertices;
	bgfx::VertexBufferHandle mVertexBuffer;
	std::vector<U16> mIndices;
	bgfx::IndexBufferHandle mIndexBuffer;
	const Tileset* mTileset;

private:
	friend class BgfxWrapper;

	static const U16 kIndices[6];
	static Shader kShader;
	static bgfx::UniformHandle kUniformTexture;
	static bool InitializeTilemaps();
	static bool ReleaseTilemaps();
};

} // namespace NAMESPACE_NAME