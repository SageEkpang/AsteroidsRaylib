#include <raylib.h>
#include <raymath.h>

#include <iostream>
#include <random>
#include "GameObject.h"
#include "ship.h"
#include "rock.h"

int main()
{
    InitWindow(800, 800, "Asteroids");
    InitAudioDevice();

    // NOTE: Init Game Objectsd
    Ship* m_Ship = new Ship(Vector2{ 400, 400 }, 30.f, 0.f);

    Sound m_ShipCrashing = LoadSound("Sounds/ShipDying.wav");
    Sound m_Track = LoadSound("Sounds/Track.wav");
    Sound m_HitRock = LoadSound("Sounds/RockExplosion.wav");

    std::vector<Rock> m_Rocks;
    std::vector<RockSpawnPoint> m_SpawnPointForMediumRocks;
    std::vector<RockSpawnPoint> m_SpawnPointForSmallRocks;

    int m_RocksToSpawn = 3;
    int m_Lives = 3;
    int m_Score = 0;

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        if (m_Lives == 0)
        {
            StopSound(m_Track);
            DrawText(TextFormat("Score: %d", m_Score), 250, 220, 60, WHITE);
            DrawText("Play Again: Enter", 150, 320, 60, BLUE);

            if (IsKeyPressed(KEY_ENTER))
            {
                m_Lives = 3;
                m_Score = 0;
                m_RocksToSpawn = 3;
                m_Rocks.clear();
                m_SpawnPointForMediumRocks.clear();
                m_SpawnPointForSmallRocks.clear();
                m_Ship->GetBulletArray().clear();
            }
        }
        else
        {
            if (IsSoundPlaying(m_Track) == false)
            {
                SetSoundVolume(m_Track, 0.1);
                PlaySound(m_Track);
            }

            // NOTE: Fill the Array with rocks its empty
            if (m_Rocks.empty())
            {
                for (int i = 0; i < m_RocksToSpawn; ++i)
                {
                    // NOTE: Make sure the spawn points are around the rock and can not spawn where the rocket is
                    int theta = GetRandomValue(0, 360);
                    int radius = GetRandomValue(0, 700 - 300) + 300;

                    float PositionX = 400.f + radius * cosf(theta * DEG2RAD);
                    float PositionY = 400.f + radius * sinf(theta * DEG2RAD);

                    m_Rocks.push_back(Rock(Vector2{ PositionX, PositionY }, ROCK_SIZE_LARGE));
                }

                ++m_RocksToSpawn;
            }

            float t_DeltaTime = GetFrameTime();

            // NOTE: Ship Update
            m_Ship->Update(t_DeltaTime);
            m_Ship->Draw();

            // Check the Borders
            if (m_Ship->GetPosition().x < (0 - m_Ship->GetScale())) { m_Ship->SetPositionX(800 + m_Ship->GetScale()); }
            else if (m_Ship->GetPosition().x > (800 + m_Ship->GetScale())) { m_Ship->SetPositionX(0 - m_Ship->GetScale()); }

            if (m_Ship->GetPosition().y < (0 - m_Ship->GetScale())) { m_Ship->SetPositionY(800 + m_Ship->GetScale()); }
            else if (m_Ship->GetPosition().y > (800 + m_Ship->GetScale())) { m_Ship->SetPositionY(0 - m_Ship->GetScale()); }

            for (auto& rock : m_Rocks)
            {
                // NOTE: Skip Any rocks that have been destroyed
                if (rock.GetIsActive() == false) { continue; }

                // Rock Updating & Draw
                rock.Update(t_DeltaTime);
                rock.Draw();

                if (rock.GetPosition().x < (0 - rock.GetScale())) { rock.SetPositionX(800 + rock.GetScale()); }
                else if (rock.GetPosition().x > (800 + rock.GetScale())) { rock.SetPositionX(0 - rock.GetScale()); }

                if (rock.GetPosition().y < (0 - rock.GetScale())) { rock.SetPositionY(800 + rock.GetScale()); }
                else if (rock.GetPosition().y > (800 + rock.GetScale())) { rock.SetPositionY(0 - rock.GetScale()); }

                // NOTE: Check Ship Collision
                if (CheckCollisionCircles(m_Ship->GetPosition(), m_Ship->GetRadius(), rock.GetPosition(), rock.GetRadius()))
                {
                    // m_Ship->SetDead(true);
                    --m_Lives;

                    if (IsSoundPlaying(m_ShipCrashing) == false)
                    {
                        SetSoundVolume(m_ShipCrashing, 0.2f);
                        PlaySound(m_ShipCrashing);
                    }

                    m_Ship->SetPosition(Vector2{ 400, 400 });
                    m_Ship->SetVelocity(Vector2{ 0, 0 });

                    if (rock.GetScale() == ROCK_SIZE_LARGE) { m_Score += ROCK_SCORE_LARGE; }
                    else if (rock.GetScale() == ROCK_SIZE_MEDIUM) { m_Score += ROCK_SCORE_MEDIUM; }
                    else if (rock.GetScale() == ROCK_SIZE_SMALL) { m_Score += ROCK_SCORE_SMALL; }
                }

                for (auto& bullet : m_Ship->GetBulletArray())
                {
                    if (bullet.isActive == false) { continue; }
                    if (rock.GetIsActive() == false) { continue; }

                    if (CheckCollisionCircles(bullet.position, bullet.radius, rock.GetPosition(), rock.GetRadius()))
                    {
                        rock.SetIsActive(false);

                        bullet.SetIsActive(false);
                        bullet.position = Vector2{ -100000, 100000 };

                        SetSoundVolume(m_HitRock, 0.2f);
                        PlaySound(m_HitRock);

                        if (rock.GetScale() == ROCK_SIZE_LARGE) { m_Score += ROCK_SCORE_LARGE; m_SpawnPointForMediumRocks.push_back(RockSpawnPoint(rock.GetPosition(), rock.GetScale() / 2)); }
                        else if (rock.GetScale() == ROCK_SIZE_MEDIUM) { m_Score += ROCK_SCORE_MEDIUM; m_SpawnPointForSmallRocks.push_back(RockSpawnPoint(rock.GetPosition(), rock.GetScale() / 2)); }
                        else if (rock.GetScale() == ROCK_SIZE_SMALL) { m_Score += ROCK_SCORE_SMALL; }
                    }
                }

            }

            // NOTE: Push Back the rocks that need to be entered into the Rock Array
            for (size_t i = 0; i < m_SpawnPointForMediumRocks.size(); ++i)
            {
                for (int j = 0; j < 2; ++j) { m_Rocks.push_back(Rock(m_SpawnPointForMediumRocks[i].position, m_SpawnPointForMediumRocks[i].scale)); }
            }
            m_SpawnPointForMediumRocks.clear();

            for (size_t i = 0; i < m_SpawnPointForSmallRocks.size(); ++i)
            {
                for (int j = 0; j < 2; ++j) { m_Rocks.push_back(Rock(m_SpawnPointForSmallRocks[i].position, m_SpawnPointForSmallRocks[i].scale)); }
            }
            m_SpawnPointForSmallRocks.clear();

            // NOTE: Remove Rocks that need to be removed
            for (size_t i = 0; i < m_Rocks.size(); )
            {
                if (m_Rocks[i].GetIsActive() == false)
                {
                    UnloadTexture(m_Rocks[i].GetTexture());
                    m_Rocks.erase(m_Rocks.begin() + i);
                }
                else { ++i; }
            }

            for (size_t i = 0; i < m_Ship->GetBulletArray().size(); )
            {
                Bullet t_tempBullet = m_Ship->GetBulletArray()[i];

                if (t_tempBullet.position.x > 800 || t_tempBullet.position.x < 0 ||
                    t_tempBullet.position.y > 800 || t_tempBullet.position.y < 0 ||
                    m_Ship->GetBulletArray()[i].isActive == false)
                {
                    m_Ship->GetBulletArray().erase(m_Ship->GetBulletArray().begin() + i);
                }
                else
                {
                    ++i;
                }
            }

            // NOTE: Score and Lives
            DrawText(TextFormat("Score: %d", m_Score), 10, 10, 30, WHITE);
            for (int i = 0; i < m_Lives; ++i) { DrawTexture(m_Ship->GetTexture(), (i * 30) + 10, 40, WHITE); }

        }

        EndDrawing();
    }

    delete m_Ship;
    m_Rocks.clear();
    m_SpawnPointForMediumRocks.clear();
    m_SpawnPointForSmallRocks.clear();
    m_Ship->GetBulletArray().clear();

    return 0;
}