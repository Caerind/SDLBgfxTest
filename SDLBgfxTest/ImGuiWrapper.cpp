#include "ImGuiWrapper.hpp"

#ifdef ENGINE_IMGUI

namespace NAMESPACE_NAME
{

bool ImGuiWrapper::Init()
{
    ImGuiWrapper& imgui = GetInstance();

    assert(!imgui.mInitialized);

    imgui.mInitialized = true;

    return true;
}

bool ImGuiWrapper::IsInitialized()
{
    return GetInstance().mInitialized;
}

bool ImGuiWrapper::Release()
{
    ImGuiWrapper& imgui = GetInstance();

    assert(imgui.mInitialized);

    imgui.mInitialized = false;

    return true;
}

ImGuiWrapper& ImGuiWrapper::GetInstance()
{
    static ImGuiWrapper instance;
    return instance;
}

ImGuiWrapper::ImGuiWrapper()
    : mInitialized(false)
{
}

ImGuiWrapper::~ImGuiWrapper()
{
    assert(!mInitialized);
}

} // namespace NAMESPACE_NAME

#endif // ENGINE_IMGUI