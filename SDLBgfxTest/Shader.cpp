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
	}

	size_t shaderLen = strlen(shaderPath);
	size_t fileLen = strlen(filename);
	char* filePath = (char*)malloc(shaderLen + fileLen);
	memcpy(filePath, shaderPath, shaderLen);
	memcpy(&filePath[shaderLen], filename, fileLen);

	FILE* file = fopen(filename, "rb");
	fseek(file, 0, SEEK_END);
	long fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	const bgfx::Memory* mem = bgfx::alloc(fileSize + 1);
	fread(mem->data, 1, fileSize, file);
	mem->data[mem->size - 1] = '\0';
	fclose(file);

	return bgfx::createShader(mem);
}

} // namespace NAMESPACE_NAME