//
//  Button.hpp
//  ComputerGraphics
//
//  Created by Sara Gonzalez on 22/1/26.
//
/*
#ifndef Button_h
#define Button_h

#include <stdio.h>
#include "framework.h"
#include "image.h"


enum ButtonType
{
    BTN_PENCIL,
    BTN_LINE,
    BTN_RECT,
    BTN_TRIANGLE,
    BTN_ERASER,
    BTN_CLEAR,
    BTN_LOAD,
    BTN_SAVE
};


class Button
{
    public:
        Image image;        // Button image
        Vector2 position;
        ButtonType type;   // Action

        Button();
        Button(const Image& img, Vector2 pos, ButtonType t);

        void Draw(Image& framebuffer);
        bool IsMouseInside(Vector2 mousePosition) const;
};


#endif /* Button_h */

