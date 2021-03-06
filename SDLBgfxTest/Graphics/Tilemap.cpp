#include "Tilemap.hpp"

namespace NAMESPACE_NAME
{
	
bgfx::VertexLayout Tilemap::Vertex::kLayout;
const U16 Tilemap::kIndices[6] = { 0, 2, 1, 0, 3, 2 };
Shader Tilemap::kShader;
bgfx::UniformHandle Tilemap::kUniformTexture;

Tilemap::Tilemap()
	: mSize(0, 0)
	, mTiles()
	, mVertices()
	, mVertexBuffer(BGFX_INVALID_HANDLE)
	, mIndexBuffer(BGFX_INVALID_HANDLE)
	, mTileset(nullptr)
{
}

Tilemap::~Tilemap()
{
	if (bgfx::isValid(mVertexBuffer))
	{
		bgfx::destroy(mVertexBuffer);
	}
	if (bgfx::isValid(mIndexBuffer))
	{
		bgfx::destroy(mIndexBuffer);
	}
}

void Tilemap::SetTileset(const Tileset& tileset)
{
	mTileset = &tileset;

	UpdateTexCoords();
	UpdateVertexBuffer();
}

const Tileset* Tilemap::GetTileset() const
{
	return mTileset;
}

void Tilemap::SetSize(const Vector2u& size)
{
	if (mSize != size)
	{
		mSize = size;

		mTiles.resize(mSize.x * mSize.y, 0);
		mVertices.resize(mSize.x * mSize.y * 4, Vertex());

		Vector2u v;
		for (v.y = 0; v.y < mSize.y; ++v.y)
		{
			for (v.x = 0; v.x < mSize.x; ++v.x)
			{
				const U32 vertexIndex = 4 * (v.x + mSize.x * v.y);
				mVertices[vertexIndex + 0].pos.Set(v.x + 0.0f, -1.0f * (v.y + 0.0f));
				mVertices[vertexIndex + 1].pos.Set(v.x + 1.0f, -1.0f * (v.y + 0.0f));
				mVertices[vertexIndex + 2].pos.Set(v.x + 1.0f, -1.0f * (v.y + 1.0f));
				mVertices[vertexIndex + 3].pos.Set(v.x + 0.0f, -1.0f * (v.y + 1.0f));
			}
		}

		UpdateTexCoords();
		UpdateVertexBuffer();
		UpdateIndexBuffer();
	}
}

const Vector2u& Tilemap::GetSize() const
{
	return mSize;
}

void Tilemap::SetTile(const Vector2u& tileCoords, U32 tileID)
{
	assert(tileCoords.x < mSize.x && tileCoords.y < mSize.y);
	const U32 tileIndex = tileCoords.x + mSize.x * tileCoords.y;
	if (mTiles[tileIndex] != tileID)
	{
		mTiles[tileIndex] = tileID;

		UpdateTileTexCoords(tileIndex);
		UpdateVertexBuffer();
	}
}

U32 Tilemap::GetTile(const Vector2u& tileCoords) const
{
	assert(tileCoords.x < mSize.x&& tileCoords.y < mSize.y);
	const U32 tileIndex = tileCoords.x + mSize.x * tileCoords.y;
	return mTiles[tileIndex];
}

Rectf Tilemap::GetLocalBounds() const
{
	return Rectf(0.0f, 0.0f, 1.0f * mSize.x, -1.0f * mSize.y);
}

Rectf Tilemap::GetGlobalBounds() const
{
	// TODO : Transform
	return GetLocalBounds();
}

void Tilemap::Render(const bgfx::ViewId& viewId) const
{
	if (bgfx::isValid(mVertexBuffer) && bgfx::isValid(mIndexBuffer) && mTileset != nullptr)
	{
		assert(mTileset->GetTexture() != nullptr);
		bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_BLEND_ALPHA_TO_COVERAGE);
		bgfx::setIndexBuffer(mIndexBuffer);
		bgfx::setVertexBuffer(0, mVertexBuffer);
		bgfx::setTexture(0, kUniformTexture, mTileset->GetTexture()->GetHandle());
		kShader.Submit(viewId);
	}
}

void Tilemap::UpdateTexCoords()
{
	if (mTileset != nullptr)
	{
		Vector2u v;
		for (v.y = 0; v.y < mSize.y; ++v.y)
		{
			for (v.x = 0; v.x < mSize.x; ++v.x)
			{
				const U32 tileIndex = v.x + mSize.x * v.y;
				UpdateTileTexCoords(tileIndex);
			}
		}
	}
}

void Tilemap::UpdateTileTexCoords(U32 tileIndex)
{
	if (mTileset != nullptr)
	{
		const Vector2f texCoords = mTileset->GetTexCoords(mTiles[tileIndex]);

		const Vector2f tileSize = mTileset->GetTileSize();
		assert(mTileset->GetTexture() != nullptr);
		assert(mTileset->GetTexture()->GetWidth() > 0);
		assert(mTileset->GetTexture()->GetHeight() > 0);
		const F32 oneOverTexWidth = 1.0f / static_cast<F32>(mTileset->GetTexture()->GetWidth());
		const F32 oneOverTexHeight = 1.0f / static_cast<F32>(mTileset->GetTexture()->GetHeight());
		const F32 left = texCoords.x * oneOverTexWidth;
		const F32 right = (texCoords.x + tileSize.x) * oneOverTexWidth;
		const F32 top = texCoords.y * oneOverTexHeight;
		const F32 bottom = (texCoords.y + tileSize.y) * oneOverTexHeight;

		const U32 vertexIndex = tileIndex * 4;
		mVertices[vertexIndex + 0].texCoords.Set(left, top);
		mVertices[vertexIndex + 1].texCoords.Set(right, top);
		mVertices[vertexIndex + 2].texCoords.Set(right, bottom);
		mVertices[vertexIndex + 3].texCoords.Set(left, bottom);
	}
}

void Tilemap::UpdateVertexBuffer()
{
	// TODO : Use dynamic vertex buffer instead ?
	// TODO : => Give the choise to the user using template boolean parameter
	if (bgfx::isValid(mVertexBuffer))
	{
		bgfx::destroy(mVertexBuffer);
		mVertexBuffer = BGFX_INVALID_HANDLE;
	}

	if (mVertices.size() > 0 && mTileset != nullptr)
	{
		mVertexBuffer = bgfx::createVertexBuffer(bgfx::makeRef(mVertices.data(), 4 * mSize.x * mSize.y * sizeof(Vertex)), Vertex::kLayout);
		assert(bgfx::isValid(mVertexBuffer));
	}
}

void Tilemap::UpdateIndexBuffer()
{
	if (bgfx::isValid(mIndexBuffer))
	{
		bgfx::destroy(mIndexBuffer);
		mIndexBuffer = BGFX_INVALID_HANDLE;
	}

	if (mSize.x > 0 && mSize.y > 0)
	{
		const U32 indiceCount = mSize.x * mSize.y * 6;
		mIndices.resize(indiceCount);
		U16 k = 0;
		for (U32 i = 0, j = 0; i < indiceCount; ++i, ++j)
		{
			if (j == 6)
			{
				j = 0;
				k += 4;
			}
			mIndices[i] = kIndices[j] + k;
		}
		mIndexBuffer = bgfx::createIndexBuffer(bgfx::makeRef(mIndices.data(), indiceCount * sizeof(U16)));
		assert(bgfx::isValid(mIndexBuffer));
	}
	else
	{
		mIndices.clear();
	}
}

bool Tilemap::InitializeTilemaps()
{
	Vertex::kLayout
		.begin()
		.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
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

	return true;
}

bool Tilemap::ReleaseTilemaps()
{
	bgfx::destroy(kUniformTexture);
	kShader.Destroy();
	return true;
}

} // namespace NAMESPACE_NAME