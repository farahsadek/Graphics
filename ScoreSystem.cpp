#include "ScoreSystem.h"

// Global variable definition
ScoreState gScore;

void initScore(float initialTimeSeconds) {
    gScore.score = 0;
    gScore.collisions = 0;
    gScore.coinsCollected = 0;   // init
    gScore.powerupsCollected = 0;   // init

    gScore.timeRemaining = initialTimeSeconds;
    gScore.parkingAccuracy = 0.0f;

    gScore.shieldActive = false;
    gScore.shieldTimer = 0.0f;
}

void addScore(int amount) {
    gScore.score += amount;
    if (gScore.score < 0) gScore.score = 0;
}

void applyCollisionPenalty(int amount) {
    if (gScore.shieldActive) {
        // shield active → ignore penalty
        return;
    }

    gScore.score -= amount;
    if (gScore.score < 0) gScore.score = 0;

    gScore.collisions += 1;
}

void addTime(float seconds) {
    gScore.timeRemaining += seconds;
    if (gScore.timeRemaining < 0.0f) {
        gScore.timeRemaining = 0.0f;
    }
}

void updateTimer(float dt) {
    gScore.timeRemaining -= dt;
    if (gScore.timeRemaining < 0.0f) {
        gScore.timeRemaining = 0.0f;
    }
}

bool isTimeOver() {
    return gScore.timeRemaining <= 0.0f;
}

void activateShield(float durationSeconds) {
    gScore.shieldActive = true;
    gScore.shieldTimer = durationSeconds;
}

void updateShield(float dt) {
    if (!gScore.shieldActive) return;

    gScore.shieldTimer -= dt;
    if (gScore.shieldTimer <= 0.0f) {
        gScore.shieldActive = false;
        gScore.shieldTimer = 0.0f;
    }
}

void setParkingAccuracy(float accuracyPercent) {
    if (accuracyPercent < 0.0f)   accuracyPercent = 0.0f;
    if (accuracyPercent > 100.0f) accuracyPercent = 100.0f;
    gScore.parkingAccuracy = accuracyPercent;
}
