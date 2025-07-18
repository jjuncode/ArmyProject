#include "pch.h"
#include "Core.h"
#include "Mgr/SceneMgr.h"
#include "PlayScene.h"
#include "Mgr/InputMgr.h"

sf::RenderWindow* Core::m_window{};
Vec2 Core::m_window_size{};

void Core::Init(int x, int y)
{
    // 윈도우 생성
    SetWindowSize(Vec2(x,y));

    static sf::RenderWindow window(sf::VideoMode(m_window_size.x, m_window_size.y), "Test");
    Core::SetWindowContext(&window);

    // 뷰 생성 ( 좌하단 0,0설정 ) 
    static sf::View view{window.getDefaultView()};
    view.setSize(m_window_size.x, m_window_size.y); 
    window.setView(view);

    Renderer::CreateRenderingBuffer();
    SceneMgr::LoadResource();
    SceneMgr::ChangeScene<PlayScene>();
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
        sf::Event event;
        // 이벤트 처리

        while (window_context->pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window_context->close();
            else{
                InputMgr::UpdateInputState(event);
            }
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
			SceneMgr::Update(time_frame);
            InputMgr::UpdateKeyState();
            InputMgr::UpdateMouseState();
			time_acc -= time_frame;
		}
         SceneMgr::Render();

    }

    SceneMgr::Exit();
}