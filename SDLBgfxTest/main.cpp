#include <cassert>
#include <cstdio>

#include "SDL.hpp"
#include "Window.hpp"
#include "Bgfx.hpp"

int main(int argc, char** argv)
{
    SDL::Init();
    {
        Window window;
        if (!window.Create("SDL & bgfx", 800, 600))
        {
            return -1;
        }

        Bgfx::Init(window);

        int counter = 0;
        while (window.IsOpen())
        {
            SDL_Event event;
            while (SDL::PollEvent(event))
            {
                switch (event.type)
                {
				case SDL_QUIT:
				{
					window.Close();
					break;
				}
                case SDL_WINDOWEVENT: 
                {
                    switch (event.window.event) 
                    {
                    case SDL_WINDOWEVENT_CLOSE:
                        window.Close();
                        break;
                    }
                } break;
                }
            }
            if (!window.IsOpen())
            {
                break;
            }

            bgfx::touch(Bgfx::kClearView);
			bgfx::dbgTextClear();
			bgfx::frame();
        }

        Bgfx::Release();
    }
    SDL::Release();
    
    printf("Exit properly\n");
    return 0;
}