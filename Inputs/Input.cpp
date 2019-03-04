#include "Input.h"

Input::Input(): m_x(0), m_y(0), m_xRel(0), m_yRel(0), m_end(false)
{
    for(int i = 0; i < SDL_NUM_SCANCODES; i++)
    {
        m_keyboard[i] = false;
    }

    for(int i = 0; i < 8; i++)
    {
        m_mouse[i] = false;
    }
}

Input::~Input()
{}


void Input::updateEvent()
{
    m_xRel = 0;
    m_yRel = 0;

    while(SDL_PollEvent(&m_event))
    {
        switch(m_event.type)
        {
            //keyboard
            case SDL_KEYDOWN:
                m_keyboard[m_event.key.keysym.scancode] = true;
            break;

            case SDL_KEYUP:
                m_keyboard[m_event.key.keysym.scancode] = false;
            break;

            //mouse button
            case SDL_MOUSEBUTTONDOWN:
                m_mouse[m_event.button.button] = true;
            break;

            case SDL_MOUSEBUTTONUP:
                m_mouse[m_event.button.button] = false;
            break;

            //mouse movement
            case SDL_MOUSEMOTION:

                m_x = m_event.motion.x;
                m_y = m_event.motion.y;

                m_xRel = m_event.motion.xrel;
                m_yRel = m_event.motion.yrel;

            break;

            //closebutton
            case SDL_WINDOWEVENT:

                if(m_event.window.event == SDL_WINDOWEVENT_CLOSE)
                {
                    m_end = true;
                }

            break;

            default:
            break;
        }
    }
}

bool Input::end()
{
    return m_end;
}


bool Input::getKey(const SDL_Scancode key) const
{
    return m_keyboard[key];
}

bool Input::getMouseButton(const Uint8 button) const
{
    return m_mouse[button];
}

bool Input::getMouseMovement() const
{
    if(m_xRel == 0 && m_yRel == 0)
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
    return m_x;
}

int Input::getY() const
{
    return m_y;
}

int Input::getXRel() const
{
    return m_xRel;
}

int Input::getYRel() const
{
    return m_yRel;
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