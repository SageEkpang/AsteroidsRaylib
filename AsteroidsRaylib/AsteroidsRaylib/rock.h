#ifndef ROCK_H
#define ROCK_H

#include "GameObject.h"
#include <vector>
#include <chrono>

#define ROCK_MOVEMENT_LARGE 80
#define ROCK_MOVEMENT_MEDIUM 100
#define ROCK_MOVEMENT_SMALL 120

#define ROCK_SCORE_LARGE 20
#define ROCK_SCORE_MEDIUM 50
#define ROCK_SCORE_SMALL 100

#define ROCK_SIZE_LARGE 180
#define ROCK_SIZE_MEDIUM (ROCK_SIZE_LARGE / 2) 
#define ROCK_SIZE_SMALL (ROCK_SIZE_MEDIUM / 2)


struct RockSpawnPoint
{
    Vector2 position;
    float scale;

    RockSpawnPoint(Vector2 position, float scale)
    {
        this->position = position;
        this->scale = scale;
    }

};

class Rock : public GameObject
{
private:

    // ROCK VARIABLE(s)
    Texture2D m_RockTexture;
    Vector2 m_Direction;

    bool m_IsActive = true;

public:

    // CLASS FUNCTION(s)
    Rock();
    Rock(Vector2 position, float scale);
    ~Rock() override;

    // BASE FUNCTION(s)
    void Update(const float deltaTime) override;
    void Draw() override;

    // GETTER FUNCTION(s)
    inline bool GetIsActive() { return m_IsActive; }
    inline float GetRadius() { return m_Scale / 2; }
    inline Texture2D& GetTexture() { return m_RockTexture; }

    // SETTER FUNCTION(s)
    inline void SetIsActive(bool active) { m_IsActive = active; }

};


#endif