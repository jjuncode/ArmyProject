#include "CameraScript.h"

#include "../Mgr/SceneMgr.h"
#include "../Mgr/InputMgr.h"
#include "../Object/Transform.h"

#include "../Core.h"

void CameraScript::Execute(float dt)
{
   // FollowTargetPos(dt);

    auto mouse_pos = InputMgr::GetMousePos();
    auto mouse_state = InputMgr::GetMouseState();

    static Vec2 prev_mouse_pos{};

    auto &camera_transform = SceneMgr::GetObject(GetOwnerID()).GetTransform();
    auto camera_forward = camera_transform.GetForward();

    if (mouse_state == MouseState::kLeftTap){
        // move start
        prev_mouse_pos = mouse_pos;
    }
    else if (mouse_state == MouseState::kLeftHold){
        Vec2 dir = mouse_pos - prev_mouse_pos;
        auto length = Vec::Length(dir);
        dir = Vec::Normalize(dir);
        dir.y = -dir.y;

        if ( length > 1.f ){
            // set direction
            // right is opposite of right axis
            auto right_dir =Vec::Normalize( Vec::Cross(Vec3(0,1,0),camera_forward));

            if ( dir.x > 0 ) {
                // look at right
                camera_forward += right_dir * dt; 
            }
            else if ( dir.x < 0 ) {
                // look at left
                camera_forward -= right_dir * dt; 
            }

            if (dir.y > 0){
                // look at up
                camera_forward += Vec3(0, 1, 0) * dt;
            }
            else if (dir.y < 0){
                // look at down
                camera_forward -= Vec3(0, 1, 0) * dt;
            }
            camera_forward = Vec::Normalize(camera_forward);
            // float yaw_delta = dir.x * dt * 3.f;    // 좌우
            // float pitch_delta = dir.y * dt * 3.f; // 위아래 (화면 y축은 반대라서 - 붙임)

            // Mat3 rotation_matrix_yaw{
            //     Vec3(cosf(yaw_delta), 0, sinf(yaw_delta)),
            //     Vec3(0, 1, 0),
            //     Vec3(-sinf(yaw_delta), 0, cosf(yaw_delta))};
            // Mat3 rotation_matrix_pitch{
            //     Vec3(1, 0, 0),
            //     Vec3(0, cosf(pitch_delta), -sinf(pitch_delta)),
            //     Vec3(0, sinf(pitch_delta), cosf(pitch_delta))};

            // auto forward = rotation_matrix_pitch * rotation_matrix_yaw * camera_forward;

            auto right = Vec::Normalize(Vec::Cross(Vec3(0,1,0), camera_forward));
            auto up = Vec::Cross(camera_forward, right);

            camera_transform.SetForward(camera_forward);
            camera_transform.SetRight(right);
            camera_transform.SetUp(up);
        }
        prev_mouse_pos = mouse_pos;
    }

    if ( InputMgr::IsTap(sf::Keyboard::Key::W) || InputMgr::IsHold(sf::Keyboard::Key::W) ) {
        camera_transform.AddPos(camera_transform.GetForward() * dt * m_speed);
    }
    if ( InputMgr::IsTap(sf::Keyboard::Key::S) || InputMgr::IsHold(sf::Keyboard::Key::S) ) {
        camera_transform.AddPos(-camera_transform.GetForward() * dt * m_speed);
    }
    if ( InputMgr::IsTap(sf::Keyboard::Key::A) || InputMgr::IsHold(sf::Keyboard::Key::A) ) {
        camera_transform.AddPos(-camera_transform.GetRight() * dt * m_speed);
    }
    if ( InputMgr::IsTap(sf::Keyboard::Key::D) || InputMgr::IsHold(sf::Keyboard::Key::D) ) {
        camera_transform.AddPos(camera_transform.GetRight() * dt * m_speed);
    }
}

Vec3 CameraScript::GetMainCameraPos()
{
    auto &camera_transform = SceneMgr::GetObject(GetOwnerID()).GetTransform();
    return camera_transform.GetPos();
}

void CameraScript::FollowTargetPos(float dt)
{
    auto &target_transform = SceneMgr::GetObject(m_target).GetTransform();
    auto& camera_transform = SceneMgr::GetObject(GetOwnerID()).GetTransform();

    auto target_pos = target_transform.GetPos();
    auto camera_pos = camera_transform.GetPos();

    auto distn = target_pos - camera_pos;
    if ( Vec::LengthSquare(distn) < 1.f ) {
        camera_pos = target_pos;
    }
    else{
        auto dir = Vec::Normalize(distn);
        camera_pos += dir* dt * m_speed;
    }

    camera_transform.SetPos(camera_pos);
}

const Mat4 CameraScript::GetViewMatrix() const
{
    auto& transform = SceneMgr::GetObject(GetOwnerID()).GetTransform();

    auto pos = transform.GetPos();

    Mat4 t_inverse{
        Vec4(1, 0, 0, -pos.x),
        Vec4(0, 1, 0, -pos.y),
        Vec4(0, 0, 1, -pos.z),
        Vec4(0, 0, 0, 1),
    };

    auto right = transform.GetRight();
    auto up = transform.GetUp();
    auto forward = transform.GetForward();

    Mat4 r_inverse{
        Vec4(right.x, right.y, right.z, 0),
        Vec4(up.x, up.y, up.z, 0),
        Vec4(-forward.x, -forward.y, -forward.z, 0),
        Vec4(0,0,0,1)
    };

    Mat4 view_matrix = r_inverse * t_inverse;
    return view_matrix; // View Matrix
}

const Mat4 CameraScript::GetProjectionMatrix() const
{
    float focal_length = 1/tanf(Vec::GetRadian(m_fov * 0.5f));
    auto window = Core::GetWindowSize();
    auto aspect = window.x / window.y;

    auto denom = m_near - m_far;

    Mat4 projection_matrix {
        Vec4( focal_length/aspect, 0, 0, 0),
        Vec4(0,  focal_length, 0, 0),
        Vec4( 0, 0, (m_near+m_far) / denom, 2*m_near*m_far / denom),
        Vec4(0, 0, -1, 0)
    };

    return projection_matrix;
}
