#pragma once

class Core
{
public:
	static void Init();
	static void Loop();

    static void Test();

	static sf::RenderWindow* GetWindowContext() { return m_window; }


private:
	static void SetWindowContext(sf::RenderWindow* _window ) { m_window = _window; }

	Core() {}
	static sf::RenderWindow* m_window;

};