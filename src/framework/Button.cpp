//
//  Button.cpp
//  ComputerGraphics
//
//  Created by Sara Gonzalez on 22/1/26.
//

#include "Button.h"

void Button::Draw(Image& framebuffer) {
    framebuffer.DrawImage(image, position.x, position.y);
}

bool Button::IsMouseInside(Vector2 mousePosition) const {
    return mousePosition.x >= position.x && mousePosition.x < position.x + image.width && mousePosition.y >= position.y && mousePosition.y < position.y + image.height;
}
