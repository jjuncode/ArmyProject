#pragma once
#include "struct.h"
#include "SFML/Graphics.hpp"

class Core{
	private:
		static sf::RenderWindow* m_window;
		static Vec2 m_window_size;

	public:
		static void Init();
		static void Loop();

	public:
		static sf::RenderWindow *GetWindowContext() { return m_window; }
		static Vec2 GetWindowSize() {return m_window_size; }

	private:
		static void SetWindowContext(sf::RenderWindow *_window) { m_window = _window; }
		static void SetWindowSize(Vec2 _size){m_window_size = _size;}
		Core() {}
};