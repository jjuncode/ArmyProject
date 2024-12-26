class Component{
    public:
        class Object* m_owner;

        virtual void Update();
        virtual void Render();

        virtual ~Component();
};