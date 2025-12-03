#include "Collectibles.h"
#include "ScoreSystem.h"
#include "Audio.h"
//#include "Player.h"       // for gPlayerCar position
#include <glut.h>
#include <cmath>

std::vector<Collectible> gCollectiblesLevel1;
std::vector<Collectible> gCollectiblesLevel2;

// -------------------- Initialization --------------------

void initCollectiblesLevel1() {
    gCollectiblesLevel1.clear();

    gCollectiblesLevel1.push_back({ CollectibleType::TIP_COIN,  1.0f, 0.0f, -2.0f, false, 0.0f });
    gCollectiblesLevel1.push_back({ CollectibleType::TIP_COIN, -2.0f, 0.0f, -5.0f, false, 0.0f });
    gCollectiblesLevel1.push_back({ CollectibleType::NITRO,     3.0f, 0.0f, -4.0f, false, 0.0f });
    gCollectiblesLevel1.push_back({ CollectibleType::CLOCK,    -4.0f, 0.0f, -6.0f, false, 0.0f });
}

void initCollectiblesLevel2() {
    gCollectiblesLevel2.clear();

    gCollectiblesLevel2.push_back({ CollectibleType::SEASHELL,  2.0f, 0.0f, -6.0f, false, 0.0f });
    gCollectiblesLevel2.push_back({ CollectibleType::SEASHELL, -3.0f, 0.0f, -8.0f, false, 0.0f });
    gCollectiblesLevel2.push_back({ CollectibleType::FAWANEES,  4.0f, 0.0f, -3.0f, false, 0.0f });
    gCollectiblesLevel2.push_back({ CollectibleType::SHIELD,    0.0f, 0.0f, -5.0f, false, 0.0f });
    gCollectiblesLevel2.push_back({ CollectibleType::NEON,      1.0f, 0.0f, -9.0f, false, 0.0f });
}

// -------------------- Update & Draw --------------------

static void updateCollectible(Collectible& c, float dt) {
    if (c.collected) return;
    c.spinAngle += 90.0f * dt;
    if (c.spinAngle > 360.0f) c.spinAngle -= 360.0f;
}

void updateCollectibles(float dt) {
    for (auto& c : gCollectiblesLevel1) updateCollectible(c, dt);
    for (auto& c : gCollectiblesLevel2) updateCollectible(c, dt);
}

static void drawSingleCollectible(const Collectible& c) {
    if (c.collected) return;

    glPushMatrix();
    glTranslatef(c.x, c.y, c.z);
    glRotatef(c.spinAngle, 0.0f, 1.0f, 0.0f);

    // TODO: replace with proper 3D models for each type
    glutSolidTeapot(0.3);  // placeholder
    glPopMatrix();
}

void drawCollectiblesLevel1() {
    for (const auto& c : gCollectiblesLevel1)
        drawSingleCollectible(c);
}

void drawCollectiblesLevel2() {
    for (const auto& c : gCollectiblesLevel2)
        drawSingleCollectible(c);
}

// -------------------- Pickup Logic --------------------

void applyCollectibleEffect(CollectibleType type) {
    switch (type)
    {
    case CollectibleType::TIP_COIN:
        addScore(10);
        gScore.coinsCollected++;
        playSound(SoundId::COIN);
        break;

    case CollectibleType::SEASHELL:
        addScore(20);
        gScore.coinsCollected++;
        playSound(SoundId::COIN);
        break;

    case CollectibleType::FAWANEES:
        addScore(15);
        gScore.coinsCollected++;
        gScore.powerupsCollected++;
        playSound(SoundId::COIN);
        break;

    case CollectibleType::CLOCK:
        addTime(10.0f);
        gScore.powerupsCollected++;
        playSound(SoundId::CLOCK);
        break;

    case CollectibleType::NITRO:
        // TODO: set nitro flags in Player.cpp (e.g., gPlayerCar.nitroActive = true;)
        gScore.powerupsCollected++;
        playSound(SoundId::NITRO);
        break;

    case CollectibleType::SHIELD:
        activateShield(5.0f);
        gScore.powerupsCollected++;
        playSound(SoundId::COIN);
        break;

    case CollectibleType::NEON:
        addScore(10);
        gScore.powerupsCollected++;
        playSound(SoundId::COIN);
        break;
    }
}

//void checkCollectiblePickup() {
//    auto processVector = [](std::vector<Collectible>& vec) {
//        for (auto& c : vec) {
//            if (c.collected) continue;
//
//            float dx = gPlayerCar.x - c.x;
//            float dz = gPlayerCar.z - c.z;
//            float distSq = dx * dx + dz * dz;
//            float pickupRadius = 1.0f;
//
//            if (distSq <= pickupRadius * pickupRadius) {
//                c.collected = true;
//                applyCollectibleEffect(c.type);
//            }
//        }
//        };
//
//    processVector(gCollectiblesLevel1);
//    processVector(gCollectiblesLevel2);
//}
