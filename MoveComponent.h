#include "Component.h"
#include "struct.h"

class MoveComponent : public Component{
    public:
        Vec2 m_pos;

        void Update() override;
}