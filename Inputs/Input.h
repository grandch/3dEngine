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

        int getX() const;
        int getY() const;

        int getXRel() const;
        int getYRel() const;

        void showCursor(bool show) const;
        void cursorCapture(bool capture) const;

    private:

        SDL_Event event;
        bool keyboard[SDL_NUM_SCANCODES];
        bool mouse[8];

        int x, y;
        int xRel, yRel;

        bool ending;
};

#endif