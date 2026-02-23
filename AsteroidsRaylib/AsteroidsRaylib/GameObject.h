#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <raylib.h>
#include <raymath.h>

class GameObject
{
protected:

    Vector2 m_Position;
    float m_Scale;
    float m_Rotation;

public:

    virtual ~GameObject() {}

    // BASE CLASS FUNCTION(s)
    virtual void Update(const float deltaTime) {}
    virtual void Draw() {}


    // GETTER FUNCTION(s)
    inline Vector2 GetPosition() { return m_Position; };
    inline float GetScale() { return m_Scale; };
    inline float GetRotation() { return m_Rotation; }

    // SETTER FUNCTION(s)
    inline void SetPosition(Vector2 position) { m_Position = position; }
    inline void SetPositionX(float x_position) { m_Position.x = x_position; }
    inline void SetPositionY(float y_position) { m_Position.y = y_position; }

    inline void SetScale(float scale) { m_Scale = scale; }

    inline void SetRotation(float rotation) { m_Rotation = rotation; }

};

#endif