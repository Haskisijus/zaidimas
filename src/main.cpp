#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <unordered_map>
#include <set>
#include <memory>
#include <algorithm>
#include <iostream>
#include <string>
#include <functional>
#include <fstream>
#include <sstream>
#include <cmath>

#include "defines.h"
#include "objects.h"
#include "utils.h"
#include "button.h"
#include "asset_manager.h"
#include "graphics_utils.h"
#include "leaderboard_manager.h"

using namespace sf;
using namespace std;
using namespace graphics;

// Button klase importuota iš button.h

// ... existing code ...
// AssetManager klase importuota iš asset_manager.h

class CosmoFlyGame {
private:
    RenderWindow app;
    AssetManager assets;
    LeaderboardManager leaderboardMgr;

    vector<Plate> plates;
    set<Keyboard::Key> pressedKeys;

    Player player;

    Text textScore, textAtomicFuel, textStandardFuel, textLowFuel, textNoFuel, textGameOver, textRestart;
    Text textMenuTitle, textMenuStart, textMenuFullscreen, textMenuLeaderboard, textMenuQuit;
    Text textEnterName, textNameInput, textLeaderboardTitle, textLeaderboardEntries;
    Text textPauseTitle;

    // UI Komponenets
    RectangleShape restartBtn, fuelBar;
    RectangleShape btnStart, btnFullscreen, btnLeaderboard, btnQuit;
    RectangleShape btnBackFromLeaderboard;

    // Dinaminis skaliavinimas
    float scaleX = 1.0f;
    float scaleY = 1.0f;

    // Pause meniu buttons
    Button btnContinue, btnQuitToMenu;

    float dy = 0;
    float score = 0;
    const float maxFuel = MAX_FUEL;
    float currentFuel = maxFuel;
    bool isGameOver = false;
    bool isUnlimitedFuel = false;

    Clock blinkClock;
    bool blinkState = true;

    GameState gameState = MAIN_MENU;
    string playerName = "";
    bool isFullscreen = false;

    function<bool(const Player&, const Plate&)> checkCollision = [](const Player& p, const Plate& pl) {
        return (p.x + PLAYER_WIDTH > pl.x) && (p.x < pl.x + PLATES_WIDTH) &&
               (p.y + 70 > pl.y) && (p.y + 70 < pl.y + PLATES_HEIGHT);
    };

public:
    CosmoFlyGame() : app(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "CosmoFly") {
        app.setFramerateLimit(60);
        srand((unsigned)time(nullptr));

        initResources();
        initUI();
        initPauseMenu();
        leaderboardMgr.Load();
        updateScaling();
    }

    void updateScaling() {
        // Apskaičiuoti skalę pagal ekrano dydį
        scaleX = static_cast<float>(app.getSize().x) / WINDOW_WIDTH;
        scaleY = static_cast<float>(app.getSize().y) / WINDOW_HEIGHT;
    }

    void run() {
        while (app.isOpen()) {
            processEvents();
            update();
            render();
        }
    }

private:
    void initResources() {
        try {
            assets.loadFont("main", "resources/arialbd.ttf");
        } catch (...) {
            cerr << "Klaida: nepavyko įkelti šrifto!" << endl;
        }
    }

    // Piešimo funkcijos importuotos iš graphics_utils.h

    void initUI() {
        Font& font = assets.getFont("main");

        auto setupText = [&](Text& t, const string& str, int size, Color fill, Color outline = Color::Black, float outlineThick = 1.f) {
            t.setFont(font);
            t.setString(str);
            t.setCharacterSize(size);
            t.setFillColor(fill);
            t.setOutlineThickness(outlineThick);
            t.setOutlineColor(outline);
        };

        setupText(textScore, "0", 40, Color::Red);
        textScore.setPosition(WINDOW_WIDTH / 2.0f - 25.f, 10.f);

        setupText(textAtomicFuel, "ATOMIC FUEL", 20, Color(138, 43, 226));
        textAtomicFuel.setPosition(220.f, 50.f);

        setupText(textStandardFuel, "STANDARD FUEL", 20, Color::Green);
        textStandardFuel.setPosition(220.f, 50.f);

        setupText(textLowFuel, "LOW FUEL!", 35, Color::Red, Color::Yellow, 2.f);
        CenterOrigin(textLowFuel);
        textLowFuel.setPosition(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT - 30.f);

        setupText(textNoFuel, "NO FUEL!", 35, Color::Red, Color::Black, 2.f);
        CenterOrigin(textNoFuel);
        textNoFuel.setPosition(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT - 30.f);

        setupText(textGameOver, "GAME OVER", 60, Color::Red, Color::Black, 3.f);
        CenterOrigin(textGameOver);
        textGameOver.setPosition(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f - 50.0f);

        // Menu UI
        setupText(textMenuTitle, "CosmoFly", 60, Color::Yellow, Color::Black, 2.f);
        CenterOrigin(textMenuTitle);
        textMenuTitle.setPosition(WINDOW_WIDTH / 2.0f, 80.f);

        setupText(textMenuStart, "START GAME", 30, Color::White);
        CenterOrigin(textMenuStart);
        textMenuStart.setPosition(WINDOW_WIDTH / 2.0f, 180.f);

        setupText(textMenuLeaderboard, "LEADERBOARD", 30, Color::White);
        CenterOrigin(textMenuLeaderboard);
        textMenuLeaderboard.setPosition(WINDOW_WIDTH / 2.0f, 250.f);

        setupText(textMenuFullscreen, "FULLSCREEN: OFF", 20, Color::Cyan);
        CenterOrigin(textMenuFullscreen);
        textMenuFullscreen.setPosition(WINDOW_WIDTH / 2.0f, 320.f);

        setupText(textMenuQuit, "QUIT", 30, Color::White);
        CenterOrigin(textMenuQuit);
        textMenuQuit.setPosition(WINDOW_WIDTH / 2.0f, 390.f);

        // Name input UI
        setupText(textEnterName, "ENTER YOUR NAME:", 25, Color::White);
        textEnterName.setPosition(30.f, 150.f);

        setupText(textNameInput, "", 30, Color::Yellow);
        textNameInput.setPosition(30.f, 220.f);

        // Leaderboard UI
        setupText(textLeaderboardTitle, "LEADERBOARD", 50, Color::Yellow, Color::Black, 2.f);
        CenterOrigin(textLeaderboardTitle);
        textLeaderboardTitle.setPosition(WINDOW_WIDTH / 2.0f, 30.f);

        setupText(textLeaderboardEntries, "", 18, Color::White);
        textLeaderboardEntries.setPosition(30.f, 100.f);

        // Buttons
        btnStart.setSize(Vector2f(200, 50));
        btnStart.setFillColor(Color::Blue);
        btnStart.setOutlineColor(Color::White);
        btnStart.setOutlineThickness(2);
        btnStart.setOrigin(100, 25);
        btnStart.setPosition(WINDOW_WIDTH / 2.0f, 180.f);

        btnLeaderboard.setSize(Vector2f(200, 50));
        btnLeaderboard.setFillColor(Color::Blue);
        btnLeaderboard.setOutlineColor(Color::White);
        btnLeaderboard.setOutlineThickness(2);
        btnLeaderboard.setOrigin(100, 25);
        btnLeaderboard.setPosition(WINDOW_WIDTH / 2.0f, 250.f);

        btnFullscreen.setSize(Vector2f(200, 50));
        btnFullscreen.setFillColor(Color::Blue);
        btnFullscreen.setOutlineColor(Color::White);
        btnFullscreen.setOutlineThickness(2);
        btnFullscreen.setOrigin(100, 25);
        btnFullscreen.setPosition(WINDOW_WIDTH / 2.0f, 320.f);

        btnQuit.setSize(Vector2f(200, 50));
        btnQuit.setFillColor(Color::Blue);
        btnQuit.setOutlineColor(Color::White);
        btnQuit.setOutlineThickness(2);
        btnQuit.setOrigin(100, 25);
        btnQuit.setPosition(WINDOW_WIDTH / 2.0f, 390.f);

        btnBackFromLeaderboard.setSize(Vector2f(100, 40));
        btnBackFromLeaderboard.setFillColor(Color::Blue);
        btnBackFromLeaderboard.setOutlineColor(Color::White);
        btnBackFromLeaderboard.setOutlineThickness(2);
        btnBackFromLeaderboard.setOrigin(50, 20);
        btnBackFromLeaderboard.setPosition(WINDOW_WIDTH / 2.0f, 500.f);

        restartBtn.setSize(Vector2f(200, 50));
        restartBtn.setFillColor(Color::Blue);
        restartBtn.setOutlineColor(Color::White);
        restartBtn.setOutlineThickness(2);
        restartBtn.setOrigin(100, 25);
        restartBtn.setPosition(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f + 50.0f);

        setupText(textRestart, "RESTART", 30, Color::White, Color::Transparent, 0);
        CenterOrigin(textRestart);
        textRestart.setPosition(restartBtn.getPosition());

        fuelBar.setPosition(10, 50);
    }


    void resetGame() {
        isGameOver = false;
        score = 0;
        dy = 0;
        currentFuel = maxFuel;
        isUnlimitedFuel = false;
        pressedKeys.clear();
        playerName = "";

        plates.clear();
        for (int i = 0; i < PLATES_AMOUNT; ++i) {
            Plate p;
            p.x = static_cast<float>(rand() % (WINDOW_WIDTH - static_cast<int>(PLATES_WIDTH)));
            p.y = static_cast<float>(WINDOW_HEIGHT) / PLATES_AMOUNT * i;
            plates.push_back(p);
        }

        int startPlatIdx = PLATES_AMOUNT - 3;
        player.x = plates[startPlatIdx].x + PLATES_WIDTH / 2.0f - 44.0f;
        player.y = plates[startPlatIdx].y - 73.0f;
    }

    void toggleFullscreen() {
        isFullscreen = !isFullscreen;

        if (isFullscreen) {
            VideoMode desktopMode = VideoMode::getDesktopMode();
            app.create(desktopMode, "CosmoFly", Style::Fullscreen);
            textMenuFullscreen.setString("FULLSCREEN: ON");
        } else {
            app.create(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "CosmoFly", Style::Default);
            textMenuFullscreen.setString("FULLSCREEN: OFF");
        }

        updateScaling();
        rescaleUI();
        app.setFramerateLimit(60);
    }

    // Pergrupuoti UI elementus pagal ekrano dydį
    void rescaleUI() {
        float windowW = app.getSize().x;
        float windowH = app.getSize().y;

        // Meniu titulas
        textMenuTitle.setCharacterSize(static_cast<int>(60 * scaleX));
        CenterOrigin(textMenuTitle);
        textMenuTitle.setPosition(windowW / 2.0f, windowH * 0.15f);

        // START GAME mygtukas ir tekstas
        btnStart.setPosition(windowW / 2.0f, windowH * 0.35f);
        btnStart.setSize(Vector2f(200 * scaleX, 50 * scaleY));
        btnStart.setOrigin(100 * scaleX, 25 * scaleY);
        textMenuStart.setCharacterSize(static_cast<int>(30 * scaleX));
        CenterOrigin(textMenuStart);
        textMenuStart.setPosition(windowW / 2.0f, windowH * 0.35f);

        // LEADERBOARD mygtukas
        btnLeaderboard.setPosition(windowW / 2.0f, windowH * 0.47f);
        btnLeaderboard.setSize(Vector2f(200 * scaleX, 50 * scaleY));
        btnLeaderboard.setOrigin(100 * scaleX, 25 * scaleY);
        textMenuLeaderboard.setCharacterSize(static_cast<int>(30 * scaleX));
        CenterOrigin(textMenuLeaderboard);
        textMenuLeaderboard.setPosition(windowW / 2.0f, windowH * 0.47f);

        // FULLSCREEN mygtukas
        btnFullscreen.setPosition(windowW / 2.0f, windowH * 0.60f);
        btnFullscreen.setSize(Vector2f(200 * scaleX, 50 * scaleY));
        btnFullscreen.setOrigin(100 * scaleX, 25 * scaleY);
        textMenuFullscreen.setCharacterSize(static_cast<int>(20 * scaleX));
        CenterOrigin(textMenuFullscreen);
        textMenuFullscreen.setPosition(windowW / 2.0f, windowH * 0.60f);

        // QUIT mygtukas
        btnQuit.setPosition(windowW / 2.0f, windowH * 0.73f);
        btnQuit.setSize(Vector2f(200 * scaleX, 50 * scaleY));
        btnQuit.setOrigin(100 * scaleX, 25 * scaleY);
        textMenuQuit.setCharacterSize(static_cast<int>(30 * scaleX));
        CenterOrigin(textMenuQuit);
        textMenuQuit.setPosition(windowW / 2.0f, windowH * 0.73f);
    }

    void initPauseMenu() {
        Font& font = assets.getFont("main");

        // Pause title
        textPauseTitle.setFont(font);
        textPauseTitle.setString("PAUSED");
        textPauseTitle.setCharacterSize(60);
        textPauseTitle.setFillColor(Color::Red);
        textPauseTitle.setOutlineThickness(2);
        textPauseTitle.setOutlineColor(Color::Black);
        FloatRect titleBounds = textPauseTitle.getLocalBounds();
        textPauseTitle.setOrigin(titleBounds.width / 2.f, titleBounds.height / 2.f);
        textPauseTitle.setPosition(WINDOW_WIDTH / 2.f, 150.f);

        // Continue button
        btnContinue.setPosition(WINDOW_WIDTH / 2.f, 280.f);
        btnContinue.setText("CONTINUE", font, 30);

        // Quit to menu button
        btnQuitToMenu.setPosition(WINDOW_WIDTH / 2.f, 370.f);
        btnQuitToMenu.setText("QUIT TO MENU", font, 25);
    }

    void updateLeaderboardText() {
        textLeaderboardEntries.setString(leaderboardMgr.GetFormattedLeaderboard());
    }

    // Transformuoti myšės koordinates iš window i game world koordinates
    Vector2f getGameCoords(Vector2i pixelPos) const {
        // Tiesiogiai konvertuoti pixel poziciją į game koordinates
        return Vector2f(
            static_cast<float>(pixelPos.x) / scaleX,
            static_cast<float>(pixelPos.y) / scaleY
        );
    }

    void processEvents() {
        Event e;
        while (app.pollEvent(e)) {
            if (e.type == Event::Closed)
                app.close();

            if (e.type == Event::KeyPressed) pressedKeys.insert(e.key.code);
            if (e.type == Event::KeyReleased) pressedKeys.erase(e.key.code);

            if (e.type == Event::KeyPressed && e.key.code == Keyboard::N) {
                if (gameState == PLAYING) {
                    isUnlimitedFuel = !isUnlimitedFuel;
                }
            }

            // ESC meniu - tik PLAYING metu
            if (e.type == Event::KeyPressed && e.key.code == Keyboard::Escape) {
                if (gameState == PLAYING) {
                    gameState = PAUSE_MENU;
                } else if (gameState == PAUSE_MENU) {
                    gameState = PLAYING;
                }
            }

            // Menu Navigation
            if (gameState == MAIN_MENU) {
                if (e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left) {
                    Vector2i pixelPos = Mouse::getPosition(app);

                    if (btnStart.getGlobalBounds().contains(Vector2f(pixelPos))) {
                        resetGame();
                        gameState = PLAYING;
                    }
                    else if (btnLeaderboard.getGlobalBounds().contains(Vector2f(pixelPos))) {
                        updateLeaderboardText();
                        gameState = LEADERBOARD;
                    }
                    else if (btnFullscreen.getGlobalBounds().contains(Vector2f(pixelPos))) {
                        toggleFullscreen();
                    }
                    else if (btnQuit.getGlobalBounds().contains(Vector2f(pixelPos))) {
                        app.close();
                    }
                }
            }
            else if (gameState == LEADERBOARD) {
                if (e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left) {
                    Vector2i pixelPos = Mouse::getPosition(app);

                    if (btnBackFromLeaderboard.getGlobalBounds().contains(Vector2f(pixelPos))) {
                        gameState = MAIN_MENU;
                    }
                }
                if ((e.type == Event::KeyPressed && e.key.code == Keyboard::Escape) ||
                    (e.type == Event::KeyPressed && e.key.code == Keyboard::BackSpace)) {
                    gameState = MAIN_MENU;
                }
            }
            else if (gameState == ENTERING_NAME) {
                if (e.type == Event::TextEntered) {
                    if (e.text.unicode < 128) {
                        if (e.text.unicode == 8 && !playerName.empty()) {
                            playerName.pop_back();
                        } else if (e.text.unicode >= 32 && e.text.unicode < 127 && playerName.length() < 20) {
                            playerName += static_cast<char>(e.text.unicode);
                        }
                    }
                }
                if (e.type == Event::KeyPressed && e.key.code == Keyboard::Return) {
                    if (!playerName.empty()) {
                        leaderboardMgr.AddScore(playerName, static_cast<int>(score));
                        gameState = MAIN_MENU;
                    }
                }
            }
            else if (gameState == GAME_OVER) {
                if ((e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left) ||
                    (e.type == Event::KeyPressed && e.key.code == Keyboard::Enter)) {

                    Vector2i pixelPos = Mouse::getPosition(app);
                    if (restartBtn.getGlobalBounds().contains(Vector2f(pixelPos)) || e.type == Event::KeyPressed) {
                        gameState = ENTERING_NAME;
                        playerName = "";
                        textNameInput.setString("");
                    }
                }
            }
            else if (gameState == PAUSE_MENU) {
                if (e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left) {
                    Vector2i pixelPos = Mouse::getPosition(app);
                    Vector2f pixelPosF = Vector2f(pixelPos);

                    if (btnContinue.shape.getGlobalBounds().contains(pixelPosF)) {
                        gameState = PLAYING;
                    }
                    else if (btnQuitToMenu.shape.getGlobalBounds().contains(pixelPosF)) {
                        gameState = MAIN_MENU;
                    }
                }
            }
        }
    }

    void update() {
        if (blinkClock.getElapsedTime().asSeconds() > 0.5f) {
            blinkState = !blinkState;
            blinkClock.restart();
        }

        if (gameState == ENTERING_NAME) {
            textNameInput.setString(playerName + (blinkState ? "_" : ""));
            return;
        }

        if (gameState != PLAYING) return;

        const float dx = 3.5f;
        if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A)) player.x -= dx;
        if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D)) player.x += dx;

        if (player.x + PLAYER_WIDTH / 2 > WINDOW_WIDTH) player.x = -PLAYER_WIDTH / 2;
        if (player.x + PLAYER_WIDTH / 2 < 0) player.x = WINDOW_WIDTH - PLAYER_WIDTH / 2;

        if (Keyboard::isKeyPressed(Keyboard::K) || Keyboard::isKeyPressed(Keyboard::Space)) {
            if (isUnlimitedFuel || currentFuel > 0) {
                dy -= 0.5f;
                if (!isUnlimitedFuel) currentFuel -= 1.5f;
            }
        }

        dy += 0.2f;
        player.y += dy;

        if (player.y > WINDOW_HEIGHT) {
            isGameOver = true;
            gameState = GAME_OVER;
        }

        if (player.y < MAX_PLAYER_Y) {
            player.y = MAX_PLAYER_Y;
            score -= 0.05f * dy;

            for_each(plates.begin(), plates.end(), [&](Plate& p) {
                p.y -= dy;
                if (p.y > WINDOW_HEIGHT) {
                    p.y = static_cast<float>(rand() % 10);
                    p.x = static_cast<float>(rand() % (WINDOW_WIDTH - static_cast<int>(PLATES_WIDTH)));
                }
            });
        }

        for (const auto& plate : plates) {
            if (checkCollision(player, plate) && dy > 0) {
                currentFuel = maxFuel;
                player.y -= dy;
                dy = 0;
                break;
            }
        }

        if (currentFuel < 0) currentFuel = 0;
    }

    void render() {
        app.clear(Color::Black);

        // Nustatome default view - be jokių transformacijų
        app.setView(app.getDefaultView());

        if (gameState == MAIN_MENU) {
            renderMainMenu();
        }
        else if (gameState == PLAYING) {
            renderGame();
        }
        else if (gameState == GAME_OVER) {
            renderGameOver();
        }
        else if (gameState == ENTERING_NAME) {
            renderNameEntry();
        }
        else if (gameState == LEADERBOARD) {
            renderLeaderboard();
        }
        else if (gameState == PAUSE_MENU) {
            renderPauseMenu();
        }

        app.display();
    }

    void drawRocketScaled(RenderWindow& window, float x, float y, bool hasThrust) {
        DrawRocketScaled(window, x, y, hasThrust, scaleX, scaleY);
    }

    void drawPlatformScaled(RenderWindow& window, float x, float y) {
        DrawPlatformScaled(window, x, y, scaleX, scaleY);
    }

    void renderMainMenu() {
        DrawBackground(app, scaleX, scaleY);

        // Fono efektai - viešos šešėliai
        RectangleShape titleBackground(Vector2f(300 * scaleX, 150 * scaleY));
        titleBackground.setFillColor(Color(0, 0, 0, 100));
        titleBackground.setPosition(50 * scaleX, 20 * scaleY);
        app.draw(titleBackground);

        app.draw(textMenuTitle);

        // Mygtukų konteineris su efektu
        RectangleShape buttonContainer(Vector2f(240 * scaleX, 280 * scaleY));
        buttonContainer.setFillColor(Color(20, 30, 60, 80));
        buttonContainer.setOutlineColor(Color(100, 150, 255, 100));
        buttonContainer.setOutlineThickness(2.f);
        buttonContainer.setPosition(app.getSize().x / 2.0f - 120 * scaleX, app.getSize().y * 0.3f);
        app.draw(buttonContainer);

        app.draw(btnStart);
        app.draw(textMenuStart);

        app.draw(btnLeaderboard);
        app.draw(textMenuLeaderboard);

        app.draw(btnFullscreen);
        app.draw(textMenuFullscreen);

        app.draw(btnQuit);
        app.draw(textMenuQuit);
    }

    void renderGame() {
        DrawBackground(app, scaleX, scaleY);

        for (const auto& plate : plates) {
            drawPlatformScaled(app, plate.x, plate.y);
        }

        bool hasThrust = (currentFuel > 0) && (Keyboard::isKeyPressed(Keyboard::K) || Keyboard::isKeyPressed(Keyboard::Space));
        drawRocketScaled(app, player.x, player.y, hasThrust);

        textScore.setString(to_string(static_cast<int>(score)));
        textScore.setPosition(WINDOW_WIDTH / 2.0f * scaleX - 25.f * scaleX, 10.f * scaleY);
        app.draw(textScore);

        if (isUnlimitedFuel) {
            fuelBar.setFillColor(Color(138, 43, 226));
            fuelBar.setSize(Vector2f(200.0f * scaleX, 20.0f * scaleY));
            textAtomicFuel.setPosition(220.f * scaleX, 50.f * scaleY);
            app.draw(textAtomicFuel);
        } else {
            fuelBar.setSize(Vector2f(200.0f * scaleX * (currentFuel / maxFuel), 20.0f * scaleY));
            fuelBar.setFillColor((currentFuel < 20) ? Color::Red : Color::Green);
            textStandardFuel.setPosition(220.f * scaleX, 50.f * scaleY);
            app.draw(textStandardFuel);
        }
        fuelBar.setPosition(10.f * scaleX, 50.f * scaleY);
        app.draw(fuelBar);

        if (!isUnlimitedFuel) {
            if (currentFuel <= 0.0f) {
                textNoFuel.setPosition(WINDOW_WIDTH / 2.0f * scaleX, (WINDOW_HEIGHT - 30.f) * scaleY);
                app.draw(textNoFuel);
            }
            else if (currentFuel < maxFuel * 0.40f && blinkState) {
                textLowFuel.setPosition(WINDOW_WIDTH / 2.0f * scaleX, (WINDOW_HEIGHT - 30.f) * scaleY);
                app.draw(textLowFuel);
            }
        }
    }

    void renderGameOver() {
        DrawBackground(app, scaleX, scaleY);

        textGameOver.setPosition(WINDOW_WIDTH / 2.0f * scaleX, (WINDOW_HEIGHT / 2.0f - 50.0f) * scaleY);
        app.draw(textGameOver);

        restartBtn.setPosition(WINDOW_WIDTH / 2.0f * scaleX, (WINDOW_HEIGHT / 2.0f + 50.0f) * scaleY);
        restartBtn.setSize(Vector2f(200 * scaleX, 50 * scaleY));
        restartBtn.setOrigin(100 * scaleX, 25 * scaleY);
        app.draw(restartBtn);

        textRestart.setPosition(WINDOW_WIDTH / 2.0f * scaleX, (WINDOW_HEIGHT / 2.0f + 50.0f) * scaleY);
        app.draw(textRestart);
    }

    void renderNameEntry() {
        DrawBackground(app, scaleX, scaleY);

        textEnterName.setPosition(30.f * scaleX, 150.f * scaleY);
        textEnterName.setCharacterSize(static_cast<int>(25 * scaleX));
        app.draw(textEnterName);

        textNameInput.setPosition(30.f * scaleX, 220.f * scaleY);
        textNameInput.setCharacterSize(static_cast<int>(30 * scaleX));
        app.draw(textNameInput);
    }

    void renderLeaderboard() {
        DrawBackground(app, scaleX, scaleY);

        textLeaderboardTitle.setPosition(WINDOW_WIDTH / 2.0f * scaleX, 30.f * scaleY);
        textLeaderboardTitle.setCharacterSize(static_cast<int>(50 * scaleX));
        app.draw(textLeaderboardTitle);

        textLeaderboardEntries.setPosition(30.f * scaleX, 100.f * scaleY);
        textLeaderboardEntries.setCharacterSize(static_cast<int>(18 * scaleX));
        app.draw(textLeaderboardEntries);

        btnBackFromLeaderboard.setPosition(WINDOW_WIDTH / 2.0f * scaleX, 500.f * scaleY);
        btnBackFromLeaderboard.setSize(Vector2f(100 * scaleX, 40 * scaleY));
        btnBackFromLeaderboard.setOrigin(50 * scaleX, 20 * scaleY);
        app.draw(btnBackFromLeaderboard);

        Text textBack("BACK", assets.getFont("main"), static_cast<int>(20 * scaleX));
        textBack.setFillColor(Color::White);
        FloatRect backBounds = textBack.getLocalBounds();
        textBack.setOrigin(backBounds.width / 2, backBounds.height / 2);
        textBack.setPosition(WINDOW_WIDTH / 2.0f * scaleX, 500.f * scaleY);
        app.draw(textBack);
    }

    void renderPauseMenu() {
        renderGame();

        // Tamsus overlay
        RectangleShape overlay(Vector2f(WINDOW_WIDTH * scaleX, WINDOW_HEIGHT * scaleY));
        overlay.setFillColor(Color(0, 0, 0, 150));
        app.draw(overlay);

        // Pause meniu - sudalytas skalias tekstas
        textPauseTitle.setCharacterSize(static_cast<int>(60 * scaleX));
        FloatRect titleBounds = textPauseTitle.getLocalBounds();
        textPauseTitle.setOrigin(titleBounds.width / 2.f, titleBounds.height / 2.f);
        textPauseTitle.setPosition(WINDOW_WIDTH / 2.0f * scaleX, 150.f * scaleY);
        app.draw(textPauseTitle);

        btnContinue.setPosition(WINDOW_WIDTH / 2.0f * scaleX, 280.f * scaleY);
        btnContinue.draw(app);

        btnQuitToMenu.setPosition(WINDOW_WIDTH / 2.0f * scaleX, 370.f * scaleY);
        btnQuitToMenu.draw(app);
    }
};

int main() {
    CosmoFlyGame game;
    game.run();
    return 0;
}