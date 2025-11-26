#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Objects.h"
#include "Utils.h"

using namespace sf;
using namespace std;

void UpdatePositions(Player &player, Plate plates[], float &dy, float &score, float &currentFuel, float maxFuel, bool &isGameOver)
{
    const float dx = 3.5f;

    if (!isGameOver)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
           player.x -= dx;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
           player.x += dx;
    }

    if (player.x + PLAYER_WIDTH / 2 > WINDOW_WIDTH)
       player.x = -PLAYER_WIDTH / 2;

    if (player.x + PLAYER_WIDTH / 2 < 0)
       player.x = WINDOW_WIDTH - PLAYER_WIDTH / 2;

    dy += 0.2f;
    player.y += dy;

    if (player.y > WINDOW_HEIGHT)
    {
       isGameOver = true;
    }

    if (player.y < MAX_PLAYER_Y)
    {
       player.y = MAX_PLAYER_Y;
       score -= 0.05f * dy;

       for (int i = 0; i < PLATES_AMOUNT; ++i)
       {
          plates[i].y -= dy;

          if (plates[i].y > WINDOW_HEIGHT)
          {
             plates[i].y = float(rand() % 10);
             plates[i].x = float(rand() % (WINDOW_WIDTH - PLATES_WIDTH));
          }
       }
    }

    for (int i = 0; i < PLATES_AMOUNT; ++i)
    {
       if (utils::InOnPlate(player, plates[i]) && dy > 0)
       {
          dy = PLAYER_JUMP_V;
          currentFuel = maxFuel;
       }
    }
}

int main()
{
    srand((unsigned)time(nullptr));
    RenderWindow app(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "CosmoFly");
    app.setFramerateLimit(60);

    Texture tBackground, tPlayer1, tPlatform, tPlayer2;
    tBackground.loadFromFile("resources/background.png");
    tPlayer1.loadFromFile("resources/doodle1.png");
    tPlayer2.loadFromFile("resources/doodle2.png");
    tPlatform.loadFromFile("resources/platform.png");

    sf::Font font;
    font.loadFromFile("resources/arialbd.ttf");

    sf::Text text;
    text.setFont(font);
    text.setString("0");
    text.setCharacterSize(40);
    text.setFillColor(Color::Red);
    text.setOutlineThickness(1);
    text.setOutlineColor(Color::Black);
    text.setPosition(WINDOW_WIDTH / 2.0f - 25.f, 10.f);

    sf::Text textGameOver;
    textGameOver.setFont(font);
    textGameOver.setString("GAME OVER");
    textGameOver.setCharacterSize(60);
    textGameOver.setFillColor(Color::Red);
    textGameOver.setOutlineThickness(3);
    textGameOver.setOutlineColor(Color::Black);

    FloatRect textRect = textGameOver.getLocalBounds();
    textGameOver.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
    textGameOver.setPosition(WINDOW_WIDTH/2.0f, WINDOW_HEIGHT/2.0f);

    Sprite sprBackground(tBackground);
    Sprite sprPlayer(tPlayer1);
    Sprite sprPlatform(tPlatform);

#ifdef SOUND_ON
    sf::SoundBuffer buffer;
    buffer.loadFromFile("resources/music.wav");
    sf::Sound sound;
    sound.setBuffer(buffer);
    sound.play();
    sound.setLoop(true);
#endif

    Player player;
    player.x = WINDOW_WIDTH / 2;
    player.y = MAX_PLAYER_Y;

    Plate plates[PLATES_AMOUNT];

    for (int i = 0; i < PLATES_AMOUNT; ++i)
    {
       plates[i].x = float(rand() % (WINDOW_WIDTH - PLATES_WIDTH));
       plates[i].y = (float)WINDOW_HEIGHT / PLATES_AMOUNT * i;
    }

    float dy = 0;
    float score = 0;
    float maxFuel = 100.0f;
    float currentFuel = maxFuel;
    bool isGameOver = false;

    RectangleShape fuelBar;
    fuelBar.setPosition(10, 50);

    while (app.isOpen())
    {
       Event e;
       while (app.pollEvent(e))
       {
          if (e.type == Event::Closed)
             app.close();
       }

       if (!isGameOver)
       {
           if (sf::Keyboard::isKeyPressed(sf::Keyboard::K) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
           {
              if (currentFuel > 0)
              {
                 sprPlayer.setTexture(tPlayer2);
                 dy -= 0.5f;
                 currentFuel -= 1.5f;
              }
              else
              {
                 sprPlayer.setTexture(tPlayer1);
              }
           }
           else
           {
              sprPlayer.setTexture(tPlayer1);
           }

           UpdatePositions(player, plates, dy, score, currentFuel, maxFuel, isGameOver);
       }

       app.clear();

       app.draw(sprBackground);

       for (int i = 0; i < PLATES_AMOUNT; ++i)
       {
          sprPlatform.setPosition(plates[i].x, plates[i].y);
          app.draw(sprPlatform);
       }

       sprPlayer.setPosition(player.x, player.y);
       app.draw(sprPlayer);

       text.setString(to_string((int)score));
       app.draw(text);

       if (currentFuel < 0) currentFuel = 0;
       fuelBar.setSize(Vector2f(200.0f * (currentFuel / maxFuel), 20.0f));

       if(currentFuel < 20)
           fuelBar.setFillColor(Color::Red);
       else
           fuelBar.setFillColor(Color::Green);

       app.draw(fuelBar);

       if (isGameOver)
       {
           app.draw(textGameOver);
       }

       app.display();
    }

    return 0;
}