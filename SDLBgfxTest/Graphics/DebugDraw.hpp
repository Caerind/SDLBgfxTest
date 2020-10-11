#pragma once

#include "../EngineIntegration.hpp"

#include <bgfx/bgfx.h>

#include "../Math/Color.hpp"
#include "../Math/Vector3.hpp"
#include "Shader.hpp"

namespace NAMESPACE_NAME
{

class DebugDraw
{
public:
	DebugDraw();
	~DebugDraw();

	void DrawLine(const Vector3f& p1, const Vector3f& p2, const Color& color = Colors::Magenta);
	void DrawBox(const Vector3f& min, const Vector3f& max, const Color& color = Colors::Magenta);
	void DrawCross(const Vector3f& position);

	void Render(const bgfx::ViewId& viewId);

private:
	void AddVertex(const Vector3f& pos, const Color& color);

private:
	struct Vertex
	{
		Vector3f pos;
		Color color;

		static bgfx::VertexLayout kLayout;
	};

	static constexpr U32 kMaxVertices = 1000;

	Vertex mVertices[kMaxVertices];
	U32 mVertexCount;
	bgfx::VertexBufferHandle mBuffer;

private:
	friend class BgfxWrapper;

	static Shader kShader;
	static bool InitializeDebugDraws();
	static bool ReleaseDebugDraws();
};

} // namespace NAMESPACE_NAME