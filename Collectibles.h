#pragma once
#include <vector>

// Types of collectibles / power-ups in both levels
enum class CollectibleType {
    TIP_COIN,   
    NITRO,      
    CLOCK,     
    SEASHELL,   
    FAWANEES,   
    SHIELD,     
    NEON        
};

struct Collectible {
    CollectibleType type;
    float x, y, z;
    bool  collected;
    float spinAngle;  // for rotation animation
};

// Global lists for each level
extern std::vector<Collectible> gCollectiblesLevel1;
extern std::vector<Collectible> gCollectiblesLevel2;

// Setup
void initCollectiblesLevel1();
void initCollectiblesLevel2();

// Per-frame updates & drawing
void updateCollectibles(float dt);
void drawCollectiblesLevel1();
void drawCollectiblesLevel2();

// Player interaction
void checkCollectiblePickup();

// Effect on score/time/etc when something is picked
void applyCollectibleEffect(CollectibleType type);
