#pragma once

// Initialization & per-frame lighting management
void initLighting();
void setupLights();      // called each frame before drawing 3D
void updateLights(float dt);

// You can optionally separate per-level
void setupLevel1Lights();
void setupLevel2Lights();
