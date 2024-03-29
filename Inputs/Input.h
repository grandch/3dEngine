#ifndef DEF_INPUT
#define DEF_INPUT

#include <SDL2/SDL.h>


class Input
{
    public:

        Input();
        ~Input();

        void updateEvent();
        bool end();

        bool getKey(const SDL_Scancode key) const;
        bool getMouseButton(const Uint8 button) const;
        bool getMouseMovement() const;
        bool getWheelMovement() const;

        int getX() const;
        int getY() const;

        int getMouseWheelY() const;

        int getXRel() const;
        int getYRel() const;

        void showCursor(bool show) const;
        void cursorCapture(bool capture) const;

    private:

        SDL_Event m_event;
        bool m_keyboard[SDL_NUM_SCANCODES];
        bool m_mouse[8];

        int m_x, m_y;
        int m_xRel, m_yRel;
        int m_mouseWheelY;

        bool m_end;
};

#endif