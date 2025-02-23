#include "GridObject.h"
#include "../Core.h"

void GridObject::Init(uint32_t offset)
{
    // offset - 간격
    Vec2 window_size {Core::GetWindowSize()};
    for (int i=0; i<window_size.y/offset; ++i ) {
        for (int j=0; j<window_size.x/ offset; ++j ){
            
        }
    }
}