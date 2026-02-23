#include "rock.h"

Rock::Rock()
{

}

Rock::Rock(Vector2 position, float scale)
{
    m_Position = position;
    m_Scale = scale;

    m_RockTexture = LoadTexture("Images/Rock.png");
    m_RockTexture.width = scale;
    m_RockTexture.height = scale;

    m_Rotation = rand() % 359;

    m_Direction = Vector2Rotate(Vector2{ 0, -1 }, m_Rotation);
}

Rock::~Rock()
{

}

void Rock::Update(const float deltaTime)
{
    if (m_Scale == ROCK_SIZE_LARGE)
    {
        m_Position.x += m_Direction.x * (ROCK_MOVEMENT_LARGE * deltaTime);
        m_Position.y += m_Direction.y * (ROCK_MOVEMENT_LARGE * deltaTime);
    }

    if (m_Scale == ROCK_SIZE_MEDIUM)
    {
        m_Position.x += m_Direction.x * (ROCK_MOVEMENT_MEDIUM * deltaTime);
        m_Position.y += m_Direction.y * (ROCK_MOVEMENT_MEDIUM * deltaTime);
    }

    if (m_Scale == ROCK_SIZE_SMALL)
    {
        m_Position.x += m_Direction.x * (ROCK_MOVEMENT_SMALL * deltaTime);
        m_Position.y += m_Direction.y * (ROCK_MOVEMENT_SMALL * deltaTime);
    }
}

void Rock::Draw()
{
    m_Rotation = m_Rotation > 360 ? 0 : m_Rotation += 50.f * (1 / m_Scale);

    DrawTexturePro(m_RockTexture, Rectangle{ 0, 0, m_Scale, m_Scale }, Rectangle{ m_Position.x, m_Position.y, m_Scale, m_Scale }, Vector2{ m_Scale / 2.f, m_Scale / 2.f }, m_Rotation, BROWN);
}
