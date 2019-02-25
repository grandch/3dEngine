#include "Input.h"

Input::Input(): x(0), y(0), xRel(0), yRel(0), ending(false  )
{
    for(int i = 0; i < SDL_NUM_SCANCODES; i++)
    {
        keyboard[i] = false;
    }

    for(int i = 0; i < 8; i++)
    {
        mouse[i] = false;
    }
}

Input::~Input()
{}


void Input::updateEvent()
{
    xRel = 0;
    yRel = 0;

    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            //keyboard
            case SDL_KEYDOWN:
                keyboard[event.key.keysym.scancode] = true;
            break;

            case SDL_KEYUP:
                keyboard[event.key.keysym.scancode] = false;
            break;

            //mouse button
            case SDL_MOUSEBUTTONDOWN:
                mouse[event.button.button] = true;
            break;

            case SDL_MOUSEBUTTONUP:
                mouse[event.button.button] = false;
            break;

            //mouse movement
            case SDL_MOUSEMOTION:

                x = event.motion.x;
                y = event.motion.y;

                xRel = event.motion.xrel;
                yRel = event.motion.yrel;

            break;

            //closebutton
            case SDL_WINDOWEVENT:

                if(event.window.event == SDL_WINDOWEVENT_CLOSE)
                {
                    ending = true;
                }

            break;

            default:
            break;
        }
    }
}

bool Input::end()
{
    return ending;
}


bool Input::getKey(const SDL_Scancode key) const
{
    return keyboard[key];
}

bool Input::getMouseButton(const Uint8 button) const
{
    return mouse[button];
}

bool Input::getMouseMovement() const
{
    if(xRel == 0 && yRel == 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}


int Input::getX() const
{
    return x;
}

int Input::getY() const
{
    return y;
}

int Input::getXRel() const
{
    return xRel;
}

int Input::getYRel() const
{
    return yRel;
}


void Input::showCursor(bool show) const
{
    if(show)
    {
        SDL_ShowCursor(SDL_ENABLE);
    }
    else
    {
        SDL_ShowCursor(SDL_DISABLE);
    }
}

void Input::cursorCapture(bool capture) const
{
    if(capture)
    {
        SDL_SetRelativeMouseMode(SDL_TRUE);
    }
    else
    {
        SDL_SetRelativeMouseMode(SDL_FALSE);
    }   
}