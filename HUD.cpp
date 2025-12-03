#include "HUD.h"
#include "ScoreSystem.h"
#include <glut.h>
#include <cstdio>

// Simple helper to draw bitmap text in 2D
static void drawBitmapText(const char* text, float x, float y) {
    glRasterPos2f(x, y);
    while (*text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text);
        ++text;
    }
}

void drawHUD() {
    // Save previous matrices
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    // Simple 0..100 orthographic space
    gluOrtho2D(0.0, 100.0, 0.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Disable depth & lighting for 2D text overlay
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    char buffer[128];

    sprintf(buffer, "Score: %d", gScore.score);
    drawBitmapText(buffer, 2.0f, 96.0f);

    sprintf(buffer, "Time: %.1f", gScore.timeRemaining);
    drawBitmapText(buffer, 2.0f, 92.0f);

    sprintf(buffer, "Collisions: %d", gScore.collisions);
    drawBitmapText(buffer, 2.0f, 88.0f);

    sprintf(buffer, "Accuracy: %.1f%%", gScore.parkingAccuracy);
    drawBitmapText(buffer, 2.0f, 84.0f);

    if (gScore.shieldActive) {
        sprintf(buffer, "Shield ACTIVE");
        drawBitmapText(buffer, 70.0f, 96.0f);
    }

    // Restore state
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);

    glPopMatrix(); // modelview

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}
