#include <cstdlib>          
#define GLUT_DISABLE_ATEXIT_HACK
#include <glut.h>

// -----------------------------
//  Game state (global status)
// -----------------------------
enum class GameMode {
    PLAYING,
    PAUSED,
    LEVEL_COMPLETE,
    GAME_OVER
};

struct GameState {
    GameMode mode = GameMode::PLAYING;
    int      currentLevel = 1;      // 1 = Level 1 (Cairo), 2 = Level 2 (Alex)
    float    timeRemaining = 120.0f; // seconds
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

// Forward declarations for future modules (Members 2/3/4)
// (only pointers are used, so definitions can come later in other files)
struct Player;
struct NPCManager;
struct CollectibleManager;
struct ScoreSystem;
struct HUD;
struct LightingSystem;

// -----------------------------
//  LevelManager class
// -----------------------------
class LevelManager {
public:
    LevelManager();

    // In future you can pass real pointers here; for now they default to nullptr
    void init(
        Player* player = nullptr,
        NPCManager* npcs = nullptr,
        CollectibleManager* collectibles = nullptr,
        ScoreSystem* score = nullptr,
        HUD* hud = nullptr,
        LightingSystem* lighting = nullptr
    );

    void loadLevel(GameState& state, int levelIndex);
    void update(GameState& state, float dt);
    void render(const GameState& state);

    void onParkingSuccess(GameState& state);
    void onTimeUp(GameState& state);

    int getCurrentLevel() const { return currentLevel; }

private:
    int   currentLevel;
    float demoRot;   // simple rotation just so we see animation

    // Pointers for future integration with other members
    Player* player;
    NPCManager* npcs;
    CollectibleManager* collectibles;
    ScoreSystem* score;
    HUD* hud;
    LightingSystem* lighting;

    void setupLevel1();
    void setupLevel2();
};

// -----------------------------
//  Global engine variables
// -----------------------------
GameState    g_state;
LevelManager g_levelManager;

int g_prevTimeMs = 0;
int g_winWidth = 800;
int g_winHeight = 600;

// -----------------------------
//  LevelManager implementation
// -----------------------------
LevelManager::LevelManager()
    : currentLevel(1),
    demoRot(0.0f),
    player(nullptr),
    npcs(nullptr),
    collectibles(nullptr),
    score(nullptr),
    hud(nullptr),
    lighting(nullptr)
{
}

void LevelManager::init(
    Player* player_,
    NPCManager* npcs_,
    CollectibleManager* collectibles_,
    ScoreSystem* score_,
    HUD* hud_,
    LightingSystem* lighting_
) {
    player = player_;
    npcs = npcs_;
    collectibles = collectibles_;
    score = score_;
    hud = hud_;
    lighting = lighting_;
}

void LevelManager::loadLevel(GameState& state, int levelIndex) {
    currentLevel = levelIndex;
    state.currentLevel = levelIndex;

    if (currentLevel == 1) {
        setupLevel1();
        state.timeRemaining = 120.0f; // example: 2 minutes
    }
    else if (currentLevel == 2) {
        setupLevel2();
        state.timeRemaining = 90.0f;  // example: 1.5 minutes
    }
}

void LevelManager::setupLevel1() {
    // Later:
    // - Cairo daytime sky / ambient light
    // - Initial player spawn
    // - Paths for tuk-tuks, pedestrians, etc.
}

void LevelManager::setupLevel2() {
    // Later:
    // - Alexandria Corniche night lighting
    // - Initial player spawn
    // - Paths for microbus, hantour, etc.
}

void LevelManager::update(GameState& state, float dt) {
    if (state.mode != GameMode::PLAYING)
        return;

    // ---- timer ----
    state.timeRemaining -= dt;
    if (state.timeRemaining <= 0.0f) {
        state.timeRemaining = 0.0f;
        onTimeUp(state);
    }

    // Simple demo rotation so we SEE something moving
    demoRot += 30.0f * dt;  // 30 degrees per second
    if (demoRot >= 360.0f) demoRot -= 360.0f;

    // ---- calls to other modules will go here later ----
    // if (player)       player->update(dt);
    // if (npcs)         npcs->update(dt, player);
    // if (collectibles) collectibles->update(dt, player);
    // if (score)        score->update(dt);
    // if (lighting)     lighting->update(dt, currentLevel);
}

void LevelManager::render(const GameState& state) {
    // Lighting per level could be customized here with real LightingSystem

    // ----- Draw a simple "ground" -----
    glPushMatrix();
    glTranslatef(0.0f, -0.55f, 0.0f);
    glScalef(20.0f, 0.1f, 20.0f);

    if (currentLevel == 1) {
        // Level 1: Cairo street – warm asphalt
        glColor3f(0.25f, 0.25f, 0.25f);
    }
    else {
        // Level 2: Alexandria Corniche – bluish tone
        glColor3f(0.1f, 0.2f, 0.35f);
    }
    glutSolidCube(1.0f);
    glPopMatrix();

    // ----- Rotating cube as placeholder "player car" -----
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    glRotatef(demoRot, 0.0f, 1.0f, 0.0f);

    if (currentLevel == 1) {
        glColor3f(0.8f, 0.1f, 0.1f);  // red-ish for Level 1
    }
    else {
        glColor3f(0.1f, 0.6f, 0.9f);  // blue-ish for Level 2
    }
    glutSolidCube(1.0f);
    glPopMatrix();

    // ----- Little sphere as "collectible" placeholder -----
    glPushMatrix();
    glTranslatef(2.0f, 0.5f, 0.0f);
    glColor3f(0.9f, 0.8f, 0.0f);
    glutSolidSphere(0.4, 20, 20);
    glPopMatrix();

    // Later:
    // - player->render();
    // - npcs->render();
    // - collectibles->render();
    // - hud->render(state);
}

void LevelManager::onParkingSuccess(GameState& state) {
    state.levelComplete();

    if (currentLevel == 1) {
        // Finished Level 1 → go to Level 2
        loadLevel(state, 2);
        state.mode = GameMode::PLAYING;
    }
    else if (currentLevel == 2) {
        // Finished Level 2 → whole game finished
        state.gameOver();
    }
}

void LevelManager::onTimeUp(GameState& state) {
    state.gameOver();
}

// -----------------------------
//  OpenGL / GLUT setup
// -----------------------------
void initGL() {
    glClearColor(0.7f, 0.9f, 1.0f, 1.0f);  // light sky
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    GLfloat lightPos[] = { 5.0f, 10.0f, 5.0f, 1.0f };
    GLfloat lightAmb[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    GLfloat lightDiff[] = { 0.9f, 0.9f, 0.9f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiff);
}

// -----------------------------
//  GLUT callbacks
// -----------------------------
void Display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = (g_winHeight == 0) ? 1.0f : (float)g_winWidth / (float)g_winHeight;
    gluPerspective(60.0f, aspect, 0.1f, 200.0f);

    // View (camera) – later Member 2 will control this
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        0.0f, 3.0f, 8.0f,   // eye
        0.0f, 0.0f, 0.0f,   // look at origin
        0.0f, 1.0f, 0.0f    // up
    );

    // Ask LevelManager to draw the current level
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

void Keyboard(unsigned char key, int x, int y) {
    switch (key) {

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
        // Later: forward to Member 2 input
        break;
    }
}

void Mouse(int button, int state, int x, int y) {
    if (state != GLUT_DOWN) return;

    if (button == GLUT_LEFT_BUTTON) {
        // Later: camera switch (first person / third person)
    }
    else if (button == GLUT_RIGHT_BUTTON) {
        // Pause / resume with right click (as in your proposal)
        if (g_state.mode == GameMode::PLAYING)
            g_state.pause();
        else if (g_state.mode == GameMode::PAUSED)
            g_state.resume();
    }
}

// -----------------------------
//  main
// -----------------------------
int main(int argc, char** argv) {
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(g_winWidth, g_winHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Khosh Hatgebak - Core Engine");

    initGL();

    // Initialize overall game state
    g_state.startGame();

    // For now we don't have real Player/NPC/HUD objects,
    // so we just call init() with default nullptrs.
    g_levelManager.init();
    g_levelManager.loadLevel(g_state, 1);

    g_prevTimeMs = glutGet(GLUT_ELAPSED_TIME);

    // Register GLUT callbacks
    glutDisplayFunc(Display);
    glutIdleFunc(Idle);
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(Keyboard);
    glutMouseFunc(Mouse);

    glutMainLoop();
    return 0;
}
