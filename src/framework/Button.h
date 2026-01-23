//
//  Button.hpp
//  ComputerGraphics
//
//  Created by Sara Gonzalez on 22/1/26.
//

#ifndef Button_h
#define Button_h

#include <stdio.h>
#include "framework.h"
#include "image.h"

class Button
{
    public:
        Image image;        // Button image
        Vector2 position;
        int type;   // Action

        void Draw(Image& framebuffer);
        bool IsMouseInside(Vector2 mousePosition) const;
};


#endif /* Button_h */

