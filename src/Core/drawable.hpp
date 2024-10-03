#pragma once

#include "entity.hpp"
#include <ctype.h>

namespace Core {

class Drawable :public AbstractEntity{
public:
    virtual void draw(uint32_t layer) override{
        if(this->layer == layer){
            drawGfx();
        }
    }

    virtual void drawGfx() = 0;
};

}
