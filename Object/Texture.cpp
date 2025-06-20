#include "Texture.h"
#include <iostream>
#include "SFML/Graphics.hpp"
#include <cassert>

std::unordered_map<std::size_t, std::unique_ptr<Texture>> Texture::map_textures;

Texture::Texture(std::string &&_name)
    :m_name{"NULL"}
    ,m_image{}
    ,m_vec_color{}
{
    m_name = std::move(_name);

    try {
      LoadImage(std::move(m_name));
    }
    catch (const std::exception &e){
        std::cerr << "Error loading texture: " << e.what() << std::endl;
    }
}

void Texture::CreateTexture(std::string&& _name)
{
    std::size_t key = std::hash<std::string>()(_name);

    if ( map_textures.find(key) != map_textures.end()) {
        std::cerr << "Texture already exists: " << _name << std::endl;
        return; // Texture already exists

    }
    else{
        auto texture = std::unique_ptr<Texture>(new Texture(std::move(_name)));
        std::cout<< "Creating texture: " << texture->GetName() << std::endl;
        map_textures[key] = std::move(texture);
    }

}

const Texture& Texture::GetTexture(const std::size_t& _key) 
{
    if ( map_textures.find(_key) != map_textures.end()) {
        auto p_texture = map_textures[_key].get();
        return *(p_texture);
    }
    else{
        if ( _key == NO_KEY ) {
            // No Texture 
            static Texture default_texture{"NULL"};  // 기본 텍스처 객체
            return default_texture;
        }
        else
            assert(false && "Texture not found");
    }
}

const sf::Color Texture::GetPixel(Vec2 _uv) const
{
    auto size = m_image.getSize();
    
    _uv.y = 1-_uv.y;

    unsigned int x = static_cast<unsigned int>(_uv.x * size.x);
    unsigned int y = static_cast<unsigned int>(_uv.y * size.y);

    int index = size.x * y + x;
//    int index = size.x * (size.y - (1 + y)) + x;
   return m_vec_color[index];
}

bool Texture::LoadImage(std::string &&_name)
{
    if (!m_image.loadFromFile("Resource/" + m_name)) {
        std::cerr << "Failed to load texture: " << m_name << std::endl;
        throw std::runtime_error("Texture loading failed");
        return false;
    }
    else{
        auto size = m_image.getSize();
        m_vec_color.reserve(size.x * size.y);
        for (unsigned int y = 0; y < size.y; ++y){
            for (unsigned int x = 0; x < size.x; ++x) {
                m_vec_color.push_back(m_image.getPixel(x, y));
            }
        }
        std::cout<<"Texutre Load SUCCESS!"<< std::endl;
        return true;
    }
}

const bool Texture::IsValid() const
{
    if ( m_name == "NULL" ){
        return false;
    }
    return true;
}
