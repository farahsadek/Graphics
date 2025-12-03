#pragma once

// Overall scoring and timing state for the game
struct ScoreState {
    int score;
    int collisions;

    int coinsCollected;       // NEW
    int powerupsCollected;    // NEW

    float timeRemaining;
    float parkingAccuracy;

    bool shieldActive;
    float shieldTimer;
};

extern ScoreState gScore;

// Initialization
void initScore(float initialTimeSeconds);

// Score / collisions
void addScore(int amount);
void applyCollisionPenalty(int amount);

// Time handling
void addTime(float seconds);
void updateTimer(float dt);
bool isTimeOver();

// Shield handling
void activateShield(float durationSeconds);
void updateShield(float dt);

// Parking accuracy
void setParkingAccuracy(float accuracyPercent);
