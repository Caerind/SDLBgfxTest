#pragma once

#include "../EngineIntegration.hpp"

#include "../Math/Vector2.hpp"
#include "Texture.hpp"

namespace NAMESPACE_NAME
{

class Tileset
{
public:
	Tileset();
	~Tileset();

	void SetTexture(const Texture& texture);
	const Texture* GetTexture() const;

	void SetTileSize(const Vector2u& tileSize);
	const Vector2u& GetTileSize() const;

	void SetGridSize(const Vector2u& gridSize);
	const Vector2u& GetGridSize() const;
	U32 GetTileCount() const;

	void SetSpacing(U32 spacing);
	U32 GetSpacing() const;

	void SetMargin(U32 margin);
	U32 GetMargin() const;

	Vector2f GetTexCoords(U32 tileID) const;

private:
	const Texture* mTexture;
	Vector2u mTileSize;
	Vector2u mGridSize;
	U32 mSpacing;
	U32 mMargin;
};

} // namespace NAMESPACE_NAME