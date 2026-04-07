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

using namespace sf;
using namespace std;

const int WINDOW_WIDTH = 400;
const int WINDOW_HEIGHT = 533;
const float PLAYER_WIDTH = 50.f;
const float PLATES_WIDTH = 68.f;
const float MAX_PLAYER_Y = 200.f;
const int PLATES_AMOUNT = 10;

struct Plate {
    float x, y;
};

struct Player {
    float x, y;
};

class AssetManager {
private:
    unordered_map<string, unique_ptr<Texture>> textures;
    unordered_map<string, unique_ptr<Font>> fonts;

public:
    void loadTexture(const string& name, const string& path) {
        auto texture = make_unique<Texture>();
        if (texture->loadFromFile(path)) {
            textures[name] = move(texture);
        }
    }

    Texture& getTexture(const string& name) {
        return *textures.at(name);
    }

    void loadFont(const string& name, const string& path) {
        auto font = make_unique<Font>();
        if (font->loadFromFile(path)) {
            fonts[name] = move(font);
        }
    }

    Font& getFont(const string& name) {
        return *fonts.at(name);
    }
};

class CosmoFlyGame {
private:
    RenderWindow app;
    AssetManager assets;

    vector<Plate> plates;
    set<Keyboard::Key> pressedKeys;

    Player player;
    Sprite sprBackground, sprPlayer, sprPlatform;

    Text textScore, textAtomicFuel, textStandardFuel, textLowFuel, textNoFuel, textGameOver, textRestart;
    RectangleShape restartBtn, fuelBar;

    float dy = 0;
    float score = 0;
    const float maxFuel = 100.0f;
    float currentFuel = maxFuel;
    bool isGameOver = false;
    bool isUnlimitedFuel = false;

    Clock blinkClock;
    bool blinkState = true;

    function<bool(const Player&, const Plate&)> checkCollision = [](const Player& p, const Plate& pl) {
        return (p.x + PLAYER_WIDTH > pl.x) && (p.x < pl.x + PLATES_WIDTH) &&
               (p.y + 70 > pl.y) && (p.y + 70 < pl.y + 14);
    };

public:
    CosmoFlyGame() : app(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "CosmoFly") {
        app.setFramerateLimit(60);
        srand((unsigned)time(nullptr));
        initResources();
        initUI();
        resetGame();
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
        assets.loadTexture("bg", "resources/background.png");
        assets.loadTexture("player1", "resources/raketa1.png");
        assets.loadTexture("player2", "resources/raketa2.png");
        assets.loadTexture("platform", "resources/platform.png");
        assets.loadFont("main", "resources/arialbd.ttf");

        sprBackground.setTexture(assets.getTexture("bg"));
        sprPlayer.setTexture(assets.getTexture("player1"));
        sprPlatform.setTexture(assets.getTexture("platform"));
    }

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
        centerOrigin(textLowFuel);
        textLowFuel.setPosition(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT - 30.f);

        setupText(textNoFuel, "NO FUEL!", 35, Color::Red, Color::Black, 2.f);
        centerOrigin(textNoFuel);
        textNoFuel.setPosition(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT - 30.f);

        setupText(textGameOver, "GAME OVER", 60, Color::Red, Color::Black, 3.f);
        centerOrigin(textGameOver);
        textGameOver.setPosition(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f - 50.0f);

        restartBtn.setSize(Vector2f(200, 50));
        restartBtn.setFillColor(Color::Blue);
        restartBtn.setOutlineColor(Color::White);
        restartBtn.setOutlineThickness(2);
        restartBtn.setOrigin(100, 25);
        restartBtn.setPosition(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f + 50.0f);

        setupText(textRestart, "RESTART", 30, Color::White, Color::Transparent, 0);
        centerOrigin(textRestart);
        textRestart.setPosition(restartBtn.getPosition());

        fuelBar.setPosition(10, 50);
    }

    void centerOrigin(Text& t) {
        FloatRect rect = t.getLocalBounds();
        t.setOrigin(rect.left + rect.width / 2.0f, rect.top + rect.height / 2.0f);
    }

    void resetGame() {
        isGameOver = false;
        score = 0;
        dy = 0;
        currentFuel = maxFuel;
        isUnlimitedFuel = false;
        pressedKeys.clear();

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

    void processEvents() {
        Event e;
        while (app.pollEvent(e)) {
            if (e.type == Event::Closed)
                app.close();

            if (e.type == Event::KeyPressed) pressedKeys.insert(e.key.code);
            if (e.type == Event::KeyReleased) pressedKeys.erase(e.key.code);

            if (e.type == Event::KeyPressed && e.key.code == Keyboard::N) {
                isUnlimitedFuel = !isUnlimitedFuel;
            }

            if (isGameOver) {
                if ((e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left) ||
                    (e.type == Event::KeyPressed && e.key.code == Keyboard::Enter)) {

                    Vector2f mousePos = app.mapPixelToCoords(Mouse::getPosition(app));
                    if (restartBtn.getGlobalBounds().contains(mousePos) || e.type == Event::KeyPressed) {
                        resetGame();
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

        if (isGameOver) return;

        const float dx = 3.5f;
        if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A)) player.x -= dx;
        if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D)) player.x += dx;

        if (player.x + PLAYER_WIDTH / 2 > WINDOW_WIDTH) player.x = -PLAYER_WIDTH / 2;
        if (player.x + PLAYER_WIDTH / 2 < 0) player.x = WINDOW_WIDTH - PLAYER_WIDTH / 2;

        if (Keyboard::isKeyPressed(Keyboard::K) || Keyboard::isKeyPressed(Keyboard::Space)) {
            if (isUnlimitedFuel || currentFuel > 0) {
                sprPlayer.setTexture(assets.getTexture("player2"));
                dy -= 0.5f;
                if (!isUnlimitedFuel) currentFuel -= 1.5f;
            } else {
                sprPlayer.setTexture(assets.getTexture("player1"));
            }
        } else {
            sprPlayer.setTexture(assets.getTexture("player1"));
        }

        dy += 0.2f;
        player.y += dy;

        if (player.y > WINDOW_HEIGHT) isGameOver = true;

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
        app.clear();

        app.draw(sprBackground);

        for (const auto& plate : plates) {
            sprPlatform.setPosition(plate.x, plate.y);
            app.draw(sprPlatform);
        }

        sprPlayer.setPosition(player.x, player.y);
        app.draw(sprPlayer);

        textScore.setString(to_string(static_cast<int>(score)));
        app.draw(textScore);

        if (isUnlimitedFuel) {
            fuelBar.setFillColor(Color(138, 43, 226));
            fuelBar.setSize(Vector2f(200.0f, 20.0f));
            app.draw(textAtomicFuel);
        } else {
            fuelBar.setSize(Vector2f(200.0f * (currentFuel / maxFuel), 20.0f));
            fuelBar.setFillColor((currentFuel < 20) ? Color::Red : Color::Green);
            app.draw(textStandardFuel);
        }
        app.draw(fuelBar);

        if (!isGameOver && !isUnlimitedFuel) {
            if (currentFuel <= 0.0f) app.draw(textNoFuel);
            else if (currentFuel < maxFuel * 0.40f && blinkState) app.draw(textLowFuel);
        }

        if (isGameOver) {
            app.draw(textGameOver);
            app.draw(restartBtn);
            app.draw(textRestart);
        }

        app.display();
    }
};

int main() {
    CosmoFlyGame game;
    game.run();
    return 0;
}