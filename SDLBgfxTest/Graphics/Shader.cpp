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
	Destroy();
}

bool Shader::Initialize(const char* vertexModule, const char* fragmentModule /*=nullptr*/)
{
	mVertexModule = CreateModule(vertexModule);
	if (!bgfx::isValid(mVertexModule))
	{
		return false;
	}

	if (fragmentModule != nullptr)
	{
		mFragmentModule = CreateModule(fragmentModule);
		if (!bgfx::isValid(mFragmentModule))
		{
			return false;
		}
	}

	mShaderProgram = bgfx::createProgram(mVertexModule, mFragmentModule, true);
	return bgfx::isValid(mShaderProgram);
}

void Shader::Destroy()
{
	if (bgfx::isValid(mShaderProgram))
	{
		bgfx::destroy(mShaderProgram);
		mShaderProgram = BGFX_INVALID_HANDLE;
	}
	if (bgfx::isValid(mVertexModule))
	{
		bgfx::destroy(mVertexModule);
		mVertexModule = BGFX_INVALID_HANDLE;
	}
	if (bgfx::isValid(mFragmentModule))
	{
		bgfx::destroy(mFragmentModule);
		mFragmentModule = BGFX_INVALID_HANDLE;
	}
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

void Shader::Submit(bgfx::ViewId viewId)
{
	assert(IsValid());
	bgfx::submit(viewId, mShaderProgram);
}

bgfx::ShaderHandle Shader::CreateModule(const char* filename)
{
	if (filename == nullptr || strlen(filename) == 0)
	{
		return BGFX_INVALID_HANDLE;
	}

	const char* shaderPath = "";
	switch (bgfx::getRendererType())
	{
	case bgfx::RendererType::Noop:
		return BGFX_INVALID_HANDLE;
	case bgfx::RendererType::Direct3D9:
		shaderPath = "shaders/dx9/";
		break;
	case bgfx::RendererType::Direct3D11:
	case bgfx::RendererType::Direct3D12:
		shaderPath = "shaders/dx11/";
		break;
	case bgfx::RendererType::Gnm:
		shaderPath = "shaders/pssl/";
		break;
	case bgfx::RendererType::Metal:
		shaderPath = "shaders/metal/";
		break;
	case bgfx::RendererType::OpenGL:
		shaderPath = "shaders/glsl/";
		break;
	case bgfx::RendererType::OpenGLES:
		shaderPath = "shaders/essl/";
		break;
	case bgfx::RendererType::Vulkan:
		shaderPath = "shaders/spirv/";
		break;
	// TODO : Add missing cases
	default: 
		assert(false);
		break;
	}

	std::size_t shaderLen = strlen(shaderPath);
	std::size_t fileLen = strlen(filename);
	char* filepath = (char*)malloc(shaderLen + fileLen + 1); // TODO : Leaking memory ?
	memcpy(filepath, shaderPath, shaderLen);
	memcpy(&filepath[shaderLen], filename, fileLen);
	filepath[shaderLen + fileLen] = '\0';

	FILE* file = fopen(filepath, "rb");
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