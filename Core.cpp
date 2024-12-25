#include "pch.h"
#include "Core.h"
#include "Object.h"
#include "Scene.h"

sf::RenderWindow* Core::m_window{};

void Core::Init()
{
    // 윈도우 생성
    static sf::RenderWindow window(sf::VideoMode(800, 600), "Test");
    Core::SetWindowContext(&window);

    // 뷰 생성 ( 좌하단 0,0설정 ) 
    sf::View view;
    view.setSize(800, -600); 
    view.setCenter(0,-0);
    window.setView(view);

    Scene::Init();
}

void Core::Loop()
{
    auto window_context = Core::GetWindowContext();

    // 프레임 시간 간격 설정
    constexpr float time_frame = 1.f / 60.f;
    using Clock = std::chrono::high_resolution_clock;
    Clock::time_point time_prev = Clock::now();
    float time_acc = 0.f;

    // 메인 루프
    while (window_context->isOpen()) {
        // 이벤트 처리
        sf::Event event;
        while (window_context->pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window_context->close();
        }


        // 경과된 시간 계산
        Clock::time_point time_cur = Clock::now();
        std::chrono::duration<float> dt = time_cur - time_prev;
        time_prev = time_cur;

        // 시간 누적
        time_acc += dt.count();

		// 고정된 시간 간격으로 게임 업데이트
		while (time_acc >= time_frame) {
			// 게임 로직 업데이트
			Scene::Update();
			time_acc -= time_frame;

		}
        Scene::Render();

    }
}

void Core::Test()
{
    std::cout << "test"<<std::endl;
}
