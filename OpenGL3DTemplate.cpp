// =====================================================
//  OpenGL3DTemplate.cpp - All-in-One File
//  Member 1 (Core Engine & Level Management)
//  + Member 4 (Score, Collectibles, HUD, Lighting, Audio)
//  Single translation unit: no extra headers / .cpp files
// =====================================================

#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <vector>

#define GLUT_DISABLE_ATEXIT_HACK
#include <glut.h>

// =====================================================
//  AUDIO
// =====================================================

enum class SoundId {
    HONK,
    COLLISION,
    COIN,
    NITRO,
    CLOCK,
    SAYES1,
    SAYES2,
    POLICE_WHISTLE
};

void initAudio() {
    // Nothing required here for WAV + PlaySoundA usage
}

void playSound(SoundId id) {
    const char* file = nullptr;

    switch (id) {
    case SoundId::HONK:            file = "Sounds/HONK.wav"; break;
    case SoundId::COLLISION:       file = "Sounds/COLLISION.wav"; break;
    case SoundId::COIN:            file = "Sounds/COIN.wav"; break;
    case SoundId::NITRO:           file = "Sounds/NITRO.wav"; break;
    case SoundId::CLOCK:           file = "Sounds/CLOCK.wav"; break;
    case SoundId::SAYES1:          file = "Sounds/SAYES1.wav"; break;
    case SoundId::SAYES2:          file = "Sounds/SAYES2.wav"; break;
    case SoundId::POLICE_WHISTLE:  file = "Sounds/POLICE_WHISTLE.wav"; break;
    }

    if (file) {
        PlaySoundA(file, NULL, SND_FILENAME | SND_ASYNC);
    }
}

// =====================================================
//  SCORE SYSTEM
// =====================================================

struct ScoreState {
    int score;
    int collisions;

    int coinsCollected;
    int powerupsCollected;

    float timeRemaining;
    float parkingAccuracy;

    bool  shieldActive;
    float shieldTimer;
};

ScoreState gScore;

void initScore(float initialTimeSeconds) {
    gScore.score = 0;
    gScore.collisions = 0;
    gScore.coinsCollected = 0;
    gScore.powerupsCollected = 0;

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
    if (gScore.shieldActive) return; // shield ignores penalty

    gScore.score -= amount;
    if (gScore.score < 0) gScore.score = 0;

    gScore.collisions += 1;
}

void addTime(float seconds) {
    gScore.timeRemaining += seconds;
    if (gScore.timeRemaining < 0.0f)
        gScore.timeRemaining = 0.0f;
}

void updateTimer(float dt) {
    gScore.timeRemaining -= dt;
    if (gScore.timeRemaining < 0.0f)
        gScore.timeRemaining = 0.0f;
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

// =====================================================
//  COLLECTIBLES
// =====================================================

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
    float spinAngle;
};

std::vector<Collectible> gCollectiblesLevel1;
std::vector<Collectible> gCollectiblesLevel2;

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

    // placeholder model – replace with actual .obj later
    glutSolidTeapot(0.3f);
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

void applyCollectibleEffect(CollectibleType type) {
    switch (type) {
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
        // TODO: integrate with Player nitro system
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

// Stub – enable when Player position exists
void checkCollectiblePickup() {
    // When Player is implemented, check distance from player to collectibles
    // and mark as collected + applyCollectibleEffect.
}

// =====================================================
//  HUD
// =====================================================

static void drawBitmapText(const char* text, float x, float y) {
    glRasterPos2f(x, y);
    while (*text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text);
        ++text;
    }
}

void drawHUD() {
    // Save projection
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, 100.0, 0.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    char buffer[128];

    std::sprintf(buffer, "Score: %d", gScore.score);
    drawBitmapText(buffer, 2.0f, 96.0f);

    std::sprintf(buffer, "Time: %.1f", gScore.timeRemaining);
    drawBitmapText(buffer, 2.0f, 92.0f);

    std::sprintf(buffer, "Collisions: %d", gScore.collisions);
    drawBitmapText(buffer, 2.0f, 88.0f);

    std::sprintf(buffer, "Accuracy: %.1f%%", gScore.parkingAccuracy);
    drawBitmapText(buffer, 2.0f, 84.0f);

    if (gScore.shieldActive) {
        std::sprintf(buffer, "Shield ACTIVE");
        drawBitmapText(buffer, 70.0f, 96.0f);
    }

    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);

    glPopMatrix(); // modelview

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

// =====================================================
//  LIGHTING
// =====================================================

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
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
}

void updateLights(float dt) {
    gSunIntensity = 0.9f + 0.1f * std::sinf(glutGet(GLUT_ELAPSED_TIME) * 0.001f);

    gNeonPhase += dt * 2.0f;
    if (gNeonPhase > 2.0f * 3.14159f) gNeonPhase -= 2.0f * 3.14159f;

    gMoonPhase += dt * 0.5f;
    if (gMoonPhase > 2.0f * 3.14159f) gMoonPhase -= 2.0f * 3.14159f;
}

// Generic hook if you want to choose per-level outside
void setupLights() {
    // left empty on purpose
}

void setupLevel1Lights() {
    GLfloat sunPos[] = { 0.0f, 50.0f, 0.0f, 1.0f };
    GLfloat sunDiff[] = { gSunIntensity, gSunIntensity, gSunIntensity, 1.0f };
    GLfloat sunAmb[] = { 0.3f, 0.3f, 0.3f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, sunPos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, sunDiff);
    glLightfv(GL_LIGHT0, GL_AMBIENT, sunAmb);

    glDisable(GL_LIGHT1);
    glDisable(GL_LIGHT2);
    glDisable(GL_LIGHT3);
}

void setupLevel2Lights() {
    float moonIntensity = 0.2f + 0.1f * std::sinf(gMoonPhase);
    GLfloat moonPos[] = { -10.0f, 40.0f, 10.0f, 1.0f };
    GLfloat moonDiff[] = { moonIntensity, moonIntensity,
                           moonIntensity + 0.05f, 1.0f };
    GLfloat moonAmb[] = { 0.1f, 0.1f, 0.2f, 1.0f };

    glLightfv(GL_LIGHT3, GL_POSITION, moonPos);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, moonDiff);
    glLightfv(GL_LIGHT3, GL_AMBIENT, moonAmb);
    glEnable(GL_LIGHT3);

    float r = 0.5f + 0.5f * std::sinf(gNeonPhase);
    float g = 0.5f + 0.5f * std::sinf(gNeonPhase + 2.0f);
    float b = 0.5f + 0.5f * std::sinf(gNeonPhase + 4.0f);
    GLfloat neonPos[] = { 0.0f, 10.0f, -10.0f, 1.0f };
    GLfloat neonDiff[] = { r, g, b, 1.0f };

    glLightfv(GL_LIGHT1, GL_POSITION, neonPos);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, neonDiff);
    glLightfv(GL_LIGHT1, GL_AMBIENT, neonDiff);
    glEnable(GL_LIGHT1);

    glDisable(GL_LIGHT2);
    glDisable(GL_LIGHT0);
}

// =====================================================
//  GAME STATE (Member 1) + LEVEL MANAGER
// =====================================================

enum class GameMode {
    PLAYING,
    PAUSED,
    LEVEL_COMPLETE,
    GAME_OVER
};

struct GameState {
    GameMode mode = GameMode::PLAYING;
    int      currentLevel = 1;
    float    timeRemaining = 120.0f;
    bool     shouldQuit = false;

    void startGame() {
        mode = GameMode::PLAYING;
        currentLevel = 1;
        timeRemaining = 120.0f;
        shouldQuit = false;
    }

    void pause() {
        if (mode == GameMode::PLAYING)
            mode = GameMode::PAUSED;
    }

    void resume() {
        if (mode == GameMode::PAUSED)
            mode = GameMode::PLAYING;
    }

    void levelComplete() {
        mode = GameMode::LEVEL_COMPLETE;
    }

    void gameOver() {
        mode = GameMode::GAME_OVER;
    }
};

class LevelManager {
public:
    LevelManager();

    void loadLevel(GameState& state, int levelIndex);
    void update(GameState& state, float dt);
    void render(const GameState& state);

    void onParkingSuccess(GameState& state);
    void onTimeUp(GameState& state);

    int getCurrentLevel() const { return currentLevel; }

private:
    int   currentLevel;
    float demoRot;

    void setupLevel1();
    void setupLevel2();
};

LevelManager::LevelManager()
    : currentLevel(1),
    demoRot(0.0f)
{
}

void LevelManager::loadLevel(GameState& state, int levelIndex) {
    currentLevel = levelIndex;
    state.currentLevel = levelIndex;

    if (currentLevel == 1) {
        setupLevel1();
        initScore(120.0f);
        initCollectiblesLevel1();
    }
    else if (currentLevel == 2) {
        setupLevel2();
        initScore(90.0f);
        initCollectiblesLevel2();
    }

    state.timeRemaining = gScore.timeRemaining;
}

void LevelManager::setupLevel1() {
    // Cairo daytime setup can go here later
}

void LevelManager::setupLevel2() {
    // Alexandria night setup can go here later
}

void LevelManager::update(GameState& state, float dt) {
    if (state.mode != GameMode::PLAYING)
        return;

    updateTimer(dt);
    updateShield(dt);
    updateCollectibles(dt);
    updateLights(dt);

    state.timeRemaining = gScore.timeRemaining;

    if (isTimeOver()) {
        onTimeUp(state);
    }

    demoRot += 30.0f * dt;
    if (demoRot > 360.0f) demoRot -= 360.0f;

    checkCollectiblePickup(); // currently stub
}

void LevelManager::render(const GameState& state) {
    if (currentLevel == 1) {
        setupLevel1Lights();
    }
    else {
        setupLevel2Lights();
    }

    // Ground
    glPushMatrix();
    glTranslatef(0.0f, -0.55f, 0.0f);
    glScalef(20.0f, 0.1f, 20.0f);
    if (currentLevel == 1)
        glColor3f(0.25f, 0.25f, 0.25f);
    else
        glColor3f(0.1f, 0.2f, 0.35f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Placeholder car cube
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    glRotatef(demoRot, 0.0f, 1.0f, 0.0f);
    if (currentLevel == 1)
        glColor3f(0.8f, 0.1f, 0.1f);
    else
        glColor3f(0.1f, 0.6f, 0.9f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Collectibles
    if (currentLevel == 1)
        drawCollectiblesLevel1();
    else
        drawCollectiblesLevel2();

    // HUD overlay
    drawHUD();
}

void LevelManager::onParkingSuccess(GameState& state) {
    state.levelComplete();

    if (currentLevel == 1) {
        loadLevel(state, 2);
        state.mode = GameMode::PLAYING;
    }
    else if (currentLevel == 2) {
        state.gameOver();
    }
}

void LevelManager::onTimeUp(GameState& state) {
    state.gameOver();
}

// =====================================================
//  GLOBAL ENGINE STATE + GLUT CALLBACKS + main()
// =====================================================

GameState    g_state;
LevelManager g_levelManager;

int g_prevTimeMs = 0;
int g_winWidth = 800;
int g_winHeight = 600;

void initGL() {
    glClearColor(0.7f, 0.9f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    initLighting();
}

void Display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = (g_winHeight == 0) ? 1.0f : (float)g_winWidth / (float)g_winHeight;
    gluPerspective(60.0f, aspect, 0.1f, 200.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        0.0f, 3.0f, 8.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    );

    g_levelManager.render(g_state);

    glutSwapBuffers();
}

void Idle() {
    int nowMs = glutGet(GLUT_ELAPSED_TIME);
    float dt = (nowMs - g_prevTimeMs) / 1000.0f;
    g_prevTimeMs = nowMs;

    if (g_state.mode == GameMode::PLAYING) {
        g_levelManager.update(g_state, dt);
    }

    glutPostRedisplay();
}

void Reshape(int w, int h) {
    if (h == 0) h = 1;
    g_winWidth = w;
    g_winHeight = h;
    glViewport(0, 0, w, h);
}

void Keyboard(unsigned char key, int, int) {
    switch (key) {
    case 27: // ESC
        g_state.shouldQuit = true;
        std::exit(0);
        break;
    case 'r':
    case 'R':
        g_state.startGame();
        g_levelManager.loadLevel(g_state, 1);
        break;
    case 'p':
    case 'P':
        if (g_state.mode == GameMode::PLAYING)
            g_state.pause();
        else if (g_state.mode == GameMode::PAUSED)
            g_state.resume();
        break;
    default:
        break;
    }
}

void Mouse(int button, int state, int, int) {
    if (state != GLUT_DOWN) return;

    if (button == GLUT_LEFT_BUTTON) {
        // later: camera switch
    }
    else if (button == GLUT_RIGHT_BUTTON) {
        if (g_state.mode == GameMode::PLAYING)
            g_state.pause();
        else if (g_state.mode == GameMode::PAUSED)
            g_state.resume();
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(g_winWidth, g_winHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Khosh Hatgebak - All-in-One");

    initGL();
    initAudio();

    g_state.startGame();
    g_levelManager.loadLevel(g_state, 1);

    g_prevTimeMs = glutGet(GLUT_ELAPSED_TIME);

    glutDisplayFunc(Display);
    glutIdleFunc(Idle);
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(Keyboard);
    glutMouseFunc(Mouse);

    glutMainLoop();
    return 0;
}
