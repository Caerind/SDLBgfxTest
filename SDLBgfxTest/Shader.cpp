#include "Shader.hpp"

namespace NAMESPACE_NAME
{

Shader::Shader()
	: mShaderProgram(BGFX_INVALID_HANDLE)
	, mVertexModule(BGFX_INVALID_HANDLE)
	, mFragmentModule(BGFX_INVALID_HANDLE)
{
}

Shader::~Shader()
{
}

bool Shader::Initialize(const char* vertexModule /*=nullptr*/, const char* fragmentModule /*=nullptr*/)
{
	mVertexModule = CreateModule(vertexModule);
	if (fragmentModule != nullptr)
	{
		mFragmentModule = CreateModule(fragmentModule);
	}

	if (!bgfx::isValid(mVertexModule))
	{
		return false;
	}

	mShaderProgram = bgfx::createProgram(mVertexModule, mFragmentModule, true);
	return bgfx::isValid(mShaderProgram);
}

bool Shader::IsValid() const
{
	return bgfx::isValid(mShaderProgram);
}

bool Shader::HasVertexModule() const
{
	return bgfx::isValid(mVertexModule);
}

bool Shader::HasFragmentModule() const
{
	return bgfx::isValid(mFragmentModule);
}

bgfx::ShaderHandle Shader::CreateModule(const char* filename)
{
	if (filename == nullptr || strlen(filename) == 0)
	{
		return BGFX_INVALID_HANDLE;
	}

	FILE* file = fopen(filename, "rb");
	if (file == nullptr)
	{
		return BGFX_INVALID_HANDLE;
	}

	fseek(file, 0, SEEK_END);
	std::size_t fileSize = static_cast<std::size_t>(ftell(file));
	fseek(file, 0, SEEK_SET);

	const bgfx::Memory* mem = bgfx::alloc(fileSize + 1);
	fread(mem->data, 1, fileSize, file);
	mem->data[mem->size - 1] = '\0';
	fclose(file);

	bgfx::ShaderHandle shaderHandle = bgfx::createShader(mem);
	if (bgfx::isValid(shaderHandle))
	{
		bgfx::setName(shaderHandle, filename);
	}
	return shaderHandle;
}

} // namespace NAMESPACE_NAME