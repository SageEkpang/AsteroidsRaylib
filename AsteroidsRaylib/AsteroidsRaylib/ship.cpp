#include "ship.h"

Ship::Ship()
{
    m_Position = Vector2{};
    m_Scale = 1.f;
    m_Rotation = 0.f;

    m_ShipTexture = LoadTexture("Images/Ship.png");
    m_ShipTexture.width = 1.f;
    m_ShipTexture.height = 1.f;
}

Ship::Ship(Vector2 position, float scale, float rotation)
{
    m_Position = position;
    m_Scale = scale;
    m_Rotation = rotation;

    m_ShipTexture = LoadTexture("Images/Ship.png");
    m_ShipTexture.width = scale;
    m_ShipTexture.height = scale;

    m_BulletTexture = LoadTexture("Images/Bullet.png");
    m_BulletTexture.width = BULLET_SIZE;
    m_BulletTexture.height = BULLET_SIZE;

    m_AfterBurnerTexture = LoadTexture("Images/AfterBurn.png");
    m_AfterBurnerTexture.width = SHIP_AFTERBURN_SIZE;
    m_AfterBurnerTexture.height = SHIP_AFTERBURN_SIZE;

    m_ShipFlying = LoadSound("Sounds/thrust.wav");
    m_Shooting = LoadSound("Sounds/fire.wav");

    m_ShipDirection = { 0, -1 };

    m_ShipMoving = false;
    m_IsDead = false;

}

Ship::~Ship()
{
    UnloadTexture(m_ShipTexture);
    UnloadTexture(m_BulletTexture);
    UnloadTexture(m_AfterBurnerTexture);
}

void Ship::Update(const float deltaTime)
{
    if (m_IsDead == true) { return; }

    m_AfterBurnerCounter += deltaTime;
    if (m_AfterBurnerCounter > m_AfterBurnerTimer)
    {
        m_AfterBurner = !m_AfterBurner;
        m_AfterBurnerCounter = 0;
    }

    MoveCharacter(deltaTime);
    ShootBullet();
    UpdateBullet(deltaTime);
}

void Ship::Draw()
{
    if (m_IsDead == true) { return; }

    DrawTexturePro(m_ShipTexture, Rectangle{ 0, 0, m_Scale, m_Scale }, Rectangle{ m_Position.x, m_Position.y, m_Scale, m_Scale }, Vector2{ m_Scale / 2.f, m_Scale / 2.f }, m_Rotation, WHITE);

    if (m_ShipMoving == true)
    {
        DrawTexturePro(m_AfterBurnerTexture, Rectangle{ 0, 0, SHIP_AFTERBURN_SIZE, SHIP_AFTERBURN_SIZE }, Rectangle{ m_Position.x, m_Position.y, SHIP_AFTERBURN_SIZE, SHIP_AFTERBURN_SIZE + (float)((int)m_AfterBurner * 4) }, Vector2{ (SHIP_AFTERBURN_SIZE / 2), (SHIP_AFTERBURN_SIZE / 2) - SHIP_AFTERBURN_SIZE - 2 }, m_Rotation, ORANGE);
    }

    DrawBullet();
}

void Ship::MoveCharacter(const float deltaTime)
{
    // rotation
    if (IsKeyDown(KEY_LEFT))
    {
        m_Rotation -= SHIP_ROTATION_SPEED;
        m_ShipDirection = Vector2Rotate(Vector2{ 0, -1 }, m_Rotation);
    }
    if (IsKeyDown(KEY_RIGHT))
    {
        m_Rotation += SHIP_ROTATION_SPEED;
        m_ShipDirection = Vector2Rotate(Vector2{ 0, -1 }, m_Rotation);
    }

    m_Rotation = m_Rotation > 360.f ? 0 : m_Rotation;
    m_Rotation = m_Rotation < 0 ? 360 : m_Rotation;

    // Add Force to the Rocket
    m_ShipMoving = false;

    if (IsKeyDown(KEY_UP))
    {
        // m_Velocity
        if (IsSoundPlaying(m_ShipFlying) == false)
        {
            SetSoundVolume(m_ShipFlying, 0.2);
            PlaySound(m_ShipFlying);
        }

        m_Velocity.x += SHIP_MOVEMENT_SPEED * m_ShipDirection.x;
        m_Velocity.y += SHIP_MOVEMENT_SPEED * m_ShipDirection.y;
        m_ShipMoving = true;
    }
    else if (IsKeyUp(KEY_UP))
    {
        StopSound(m_ShipFlying);
    }

    m_Acceleration.x += m_NetForce.x;
    m_Acceleration.y += m_NetForce.y;

    Vector2 t_Position = m_Position;
    m_Velocity.x += m_Acceleration.x * deltaTime;
    m_Velocity.y += m_Acceleration.y * deltaTime;

    t_Position.x += m_Velocity.x * deltaTime;
    t_Position.y += m_Velocity.y * deltaTime;

    m_Position = t_Position;

    m_NetForce = Vector2{ 0, 0 };
    m_Acceleration = Vector2{ 0, 0 };

}

void Ship::ShootBullet()
{
    if (IsKeyPressed(KEY_SPACE))
    {
        SetSoundVolume(m_Shooting, 0.2);
        PlaySound(m_Shooting);
        Vector2 t_Direction = m_ShipDirection;
        m_Bullets.push_back(Bullet(m_Position, t_Direction, BULLET_SIZE));
    }
}

void Ship::UpdateBullet(const float deltaTime)
{
    for (auto& v : m_Bullets)
    {
        if (v.isActive == false) { continue; }
        v.position.x += v.direction.x * (BULLET_SPEED * deltaTime);
        v.position.y += v.direction.y * (BULLET_SPEED * deltaTime);
    }
}

void Ship::DrawBullet()
{
    for (auto& Bullet : m_Bullets)
    {
        if (Bullet.isActive == false) { continue; }
        DrawTextureV(m_BulletTexture, Bullet.position, WHITE);
    }
}
