#ifndef SHIP_H
#define SHIP_H

#include "GameObject.h"
#include <vector>

#define SHIP_ROTATION_SPEED 5
#define SHIP_MOVEMENT_SPEED 5
#define SHIP_AFTERBURN_SIZE 20
#define SHIP_AFTERBURN_OSC 0.1
#define SHIP_RADIUS 10

#define BULLET_SPEED 600
#define BULLET_SIZE 5

struct Bullet
{
    bool isActive;
    Vector2 direction;
    Vector2 position;
    float radius;

    Bullet(Vector2 position, Vector2 direction, float radius)
    {
        this->position = position;
        this->direction = direction;
        this->radius = radius;
        isActive = true;
    }

    void SetIsActive(bool active) { isActive = active; }

};

class Ship : public GameObject
{
private:

    // SHIP VARIABLE(s)

    Sound m_ShipFlying;
    Sound m_Shooting;

    Texture2D m_ShipTexture;
    Vector2 m_ShipDirection;
    bool m_ShipMoving = false;
    bool m_IsDead = false;

    Texture2D m_AfterBurnerTexture;
    bool m_AfterBurner = false;
    float m_AfterBurnerTimer = SHIP_AFTERBURN_OSC;
    float m_AfterBurnerCounter = 0;

    // BULLET VARIABLE(s)
    Texture2D m_BulletTexture;
    std::vector<Bullet> m_Bullets;

    // FORCE VARIABLE(s)
    Vector2 m_NetForce;
    Vector2 m_Acceleration;
    Vector2 m_Velocity;

public:

    // CLASS FUNCTION(s)
    Ship();
    Ship(Vector2 position, float scale, float rotation);
    ~Ship() override;

    // BASE FUNCTION(s)
    void Update(const float deltaTime) override;
    void Draw() override;


    // HELPER FUNCTION(s)
    void MoveCharacter(const float deltaTime);
    void ShootBullet();

    void UpdateBullet(const float deltaTime);
    void DrawBullet();


    // GETTER FUNCTION(s)
    inline Vector2 GetPosition() { return m_Position; }
    inline float GetScale() { return m_Scale; }
    inline float GetRotation() { return m_Rotation; }
    inline float GetRadius() { return SHIP_RADIUS; }
    inline Texture2D GetTexture() { return m_ShipTexture; }

    inline std::vector<Bullet>& GetBulletArray() { return m_Bullets; }

    // SETTER FUNCTION(s)
    inline void SetPosition(Vector2 position) { m_Position = position; }
    inline void SetVelocity(Vector2 velocity) { m_Velocity = velocity; }
    inline void SetPositionX(float positionX) { m_Position.x = positionX; }
    inline void SetPositionY(float positionY) { m_Position.y = positionY; }
    inline void SetDead(bool dead) { m_IsDead = dead; }

};

#endif