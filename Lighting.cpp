#include "Lighting.h"
//#include "GameState.h"
#include <glut.h>
#include <cmath>

// Simple animation parameters
static float gSunIntensity = 1.0f;
static float gNeonPhase = 0.0f;
static float gMoonPhase = 0.0f;

void initLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0); 
    glEnable(GL_LIGHT1); 
    glEnable(GL_LIGHT2); 
    glEnable(GL_LIGHT3); 

    glEnable(GL_COLOR_MATERIAL);
}

// Called once per frame, after setting camera
//void setupLights() {
//    if (gGameState.currentScene == GameScene::LEVEL1) {
//        setupLevel1Lights();
//    }
//    else if (gGameState.currentScene == GameScene::LEVEL2) {
//        setupLevel2Lights();
//    }
//}

void updateLights(float dt) {
    // Animate sun (slight flicker / variation)
    gSunIntensity = 0.9f + 0.1f * sinf(glutGet(GLUT_ELAPSED_TIME) * 0.001f);

    // Animate neon lights (color cycling)
    gNeonPhase += dt * 2.0f;
    if (gNeonPhase > 2.0f * 3.14159f) gNeonPhase -= 2.0f * 3.14159f;

    // Animate moonlight
    gMoonPhase += dt * 0.5f;
    if (gMoonPhase > 2.0f * 3.14159f) gMoonPhase -= 2.0f * 3.14159f;
}

void setupLevel1Lights() {
    // LIGHT0 = Sun / daylight

    GLfloat sunPos[] = { 0.0f, 50.0f, 0.0f, 1.0f };
    GLfloat sunDiff[] = { gSunIntensity, gSunIntensity, gSunIntensity, 1.0f };
    GLfloat sunAmb[] = { 0.3f, 0.3f, 0.3f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, sunPos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, sunDiff);
    glLightfv(GL_LIGHT0, GL_AMBIENT, sunAmb);

    // You can disable others in Level 1 if you want:
    glDisable(GL_LIGHT1);
    glDisable(GL_LIGHT2);
    glDisable(GL_LIGHT3);
}

void setupLevel2Lights() {
    // LIGHT3 = Moonlight / ambient night
    float moonIntensity = 0.2f + 0.1f * sinf(gMoonPhase);
    GLfloat moonPos[] = { -10.0f, 40.0f, 10.0f, 1.0f };
    GLfloat moonDiff[] = { moonIntensity, moonIntensity, moonIntensity + 0.05f, 1.0f };
    GLfloat moonAmb[] = { 0.1f, 0.1f, 0.2f, 1.0f };

    glLightfv(GL_LIGHT3, GL_POSITION, moonPos);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, moonDiff);
    glLightfv(GL_LIGHT3, GL_AMBIENT, moonAmb);
    glEnable(GL_LIGHT3);

    // LIGHT1 = Neon lights (color animated)
    float r = 0.5f + 0.5f * sinf(gNeonPhase);
    float g = 0.5f + 0.5f * sinf(gNeonPhase + 2.0f);
    float b = 0.5f + 0.5f * sinf(gNeonPhase + 4.0f);
    GLfloat neonPos[] = { 0.0f, 10.0f, -10.0f, 1.0f };
    GLfloat neonDiff[] = { r, g, b, 1.0f };

    glLightfv(GL_LIGHT1, GL_POSITION, neonPos);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, neonDiff);
    glLightfv(GL_LIGHT1, GL_AMBIENT, neonDiff);
    glEnable(GL_LIGHT1);

    // LIGHT2 could be police checkpoint light (you can add later)
    // For now you can disable it:
    glDisable(GL_LIGHT2);

    // Disable LIGHT0 (sun) at night if you prefer:
    glDisable(GL_LIGHT0);
}
