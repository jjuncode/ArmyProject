#pragma once 

template<typename T>
class MemoryPool{
    private:
        std::stack<T*> m_pool;

    public:
        std::shared_ptr<T> Acquire(){
            if (!m_pool.empty()){
                // if exist 
                T* obj {m_pool.top()};
                m_pool.pop();

                return std::shared_ptr<T>(obj, [this](T* ptr){Release(ptr);} ); 
            }
            else{
                // non exist
                return std::shared_ptr<T>(new , [this](T* ptr){Release(ptr);});
            }
        };

        void Release(T* obj){
            obj.Reset();
            m_pool.push(obj);
        }
}