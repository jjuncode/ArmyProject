#pragma once
#include <SFML/Graphics/Image.hpp>
#include <string>
#include <unordered_map>
#include <memory>
#include "../struct.h"

class Texture {
private:
    std::string m_name; // Texture name
    sf::Image m_image;  // image

    std::vector<sf::Color> m_vec_color;

    static std::unordered_map<std::size_t, std::unique_ptr<Texture>> map_textures;

public:
    static void CreateTexture(std::string&& _name);
    static const Texture& GetTexture(const std::size_t& _key);

    const sf::Color GetPixel(Vec2 _uv) const;
    const std::string &GetName() const { return m_name; };

private:
    Texture(std::string &&_name);
    bool LoadImage(std::string &&_name);
};
