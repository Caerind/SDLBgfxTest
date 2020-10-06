#pragma once

#include "../EngineIntegration.hpp"

#include <bgfx/bgfx.h>

#include "../Window/Window.hpp"

namespace NAMESPACE_NAME
{
class BgfxWrapper
{
public:
    static bool Init(Window& window);
    static bool IsInitialized();
    static bool Release();

#ifdef ENGINE_DEBUG
    static void ToggleDisplayStats();
    static void SetDisplayStats(bool display);
    static bool IsDisplayingStats();
#endif // ENGINE_DEBUG

private: 
    static BgfxWrapper& GetInstance();

    BgfxWrapper();
    ~BgfxWrapper();

	bool mInitialized;
#ifdef ENGINE_DEBUG
	bool mDisplayStats;
#endif // ENGINE_DEBUG
};

} // namespace NAMESPACE_NAME