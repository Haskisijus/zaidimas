#include "graphics_utils.h"
#include "defines.h"

namespace graphics {

    void DrawRocket(RenderWindow& window, float x, float y, bool hasThrust) {
        // Raketės korpusas
        RectangleShape rocketBody(Vector2f(30.f, 60.f));
        rocketBody.setPosition(x, y);
        rocketBody.setFillColor(hasThrust ? Color::Yellow : Color::White);
        window.draw(rocketBody);

        // Raketės tipas
        CircleShape tip(15.f);
        tip.setPosition(x + 7.5f, y - 15.f);
        tip.setFillColor(Color::Red);
        window.draw(tip);

        // Degimo efektas
        if (hasThrust) {
            RectangleShape flame(Vector2f(20.f, 20.f));
            flame.setPosition(x + 5.f, y + 60.f);
            flame.setFillColor(Color(255, 165, 0));
            window.draw(flame);
        }
    }

    void DrawRocketScaled(RenderWindow& window, float x, float y, bool hasThrust, float scaleX, float scaleY) {
        x *= scaleX;
        y *= scaleY;

        // Raketės šešėlis
        CircleShape shadow(12.f * scaleX);
        shadow.setFillColor(Color(0, 0, 0, 50));
        shadow.setPosition(x + 3.f * scaleX, y + 62.f * scaleY);
        window.draw(shadow);

        // Raketės šonai - delta wings
        RectangleShape leftWing(Vector2f(12.f * scaleX, 25.f * scaleY));
        leftWing.setPosition(x - 12.f * scaleX, y + 20.f * scaleY);
        leftWing.setFillColor(Color(100, 110, 130));
        leftWing.setRotation(-25.f);
        window.draw(leftWing);

        RectangleShape rightWing(Vector2f(12.f * scaleX, 25.f * scaleY));
        rightWing.setPosition(x + 30.f * scaleX, y + 20.f * scaleY);
        rightWing.setFillColor(Color(100, 110, 130));
        rightWing.setRotation(25.f);
        window.draw(rightWing);

        // Raketės korpusas
        RectangleShape rocketBody(Vector2f(30.f * scaleX, 60.f * scaleY));
        rocketBody.setPosition(x, y);
        rocketBody.setFillColor(hasThrust ? Color(255, 200, 0) : Color(180, 190, 210));
        window.draw(rocketBody);

        // Raketės blizgesys
        RectangleShape bodyHighlight(Vector2f(6.f * scaleX, 55.f * scaleY));
        bodyHighlight.setPosition(x + 1.5f * scaleX, y + 2.f * scaleY);
        bodyHighlight.setFillColor(Color(255, 255, 255, 120));
        window.draw(bodyHighlight);

        // Raketės šoninis šešėlis
        RectangleShape bodyShadow(Vector2f(4.f * scaleX, 60.f * scaleY));
        bodyShadow.setPosition(x + 26.f * scaleX, y);
        bodyShadow.setFillColor(Color(80, 90, 110));
        window.draw(bodyShadow);

        // Raketės nosis
        CircleShape noseTip(10.f * scaleX);
        noseTip.setFillColor(Color(255, 40, 40));
        noseTip.setPosition(x + 5.f * scaleX, y - 10.f * scaleY);
        window.draw(noseTip);

        CircleShape noseBase(8.f * scaleX);
        noseBase.setFillColor(Color(200, 30, 30));
        noseBase.setPosition(x + 7.f * scaleX, y - 6.f * scaleY);
        window.draw(noseBase);

        CircleShape noseShine(4.f * scaleX);
        noseShine.setFillColor(Color(255, 180, 180, 180));
        noseShine.setPosition(x + 10.f * scaleX, y - 8.f * scaleY);
        window.draw(noseShine);

        // Langai - kabina
        CircleShape window1(2.5f * scaleX);
        window1.setFillColor(Color(80, 180, 255));
        window1.setPosition(x + 13.f * scaleX, y + 5.f * scaleY);
        window.draw(window1);

        CircleShape window2(2.5f * scaleX);
        window2.setFillColor(Color(80, 180, 255));
        window2.setPosition(x + 13.f * scaleX, y + 18.f * scaleY);
        window.draw(window2);

        CircleShape window3(2.5f * scaleX);
        window3.setFillColor(Color(80, 180, 255));
        window3.setPosition(x + 13.f * scaleX, y + 31.f * scaleY);
        window.draw(window3);

        // Žiedai
        CircleShape ring1(3.f * scaleX);
        ring1.setFillColor(Color::Transparent);
        ring1.setOutlineThickness(1.f);
        ring1.setOutlineColor(Color(150, 160, 180));
        ring1.setPosition(x + 12.f * scaleX, y + 42.f * scaleY);
        window.draw(ring1);

        // Degimo efektas
        if (hasThrust) {
            RectangleShape flameOuter(Vector2f(26.f * scaleX, 30.f * scaleY));
            flameOuter.setPosition(x + 2.f * scaleX, y + 60.f * scaleY);
            flameOuter.setFillColor(Color(255, 130, 0, 200));
            window.draw(flameOuter);

            RectangleShape flameMiddle(Vector2f(18.f * scaleX, 22.f * scaleY));
            flameMiddle.setPosition(x + 6.f * scaleX, y + 65.f * scaleY);
            flameMiddle.setFillColor(Color(255, 200, 0, 220));
            window.draw(flameMiddle);

            RectangleShape flameInner(Vector2f(10.f * scaleX, 15.f * scaleY));
            flameInner.setPosition(x + 10.f * scaleX, y + 70.f * scaleY);
            flameInner.setFillColor(Color(255, 255, 150, 240));
            window.draw(flameInner);

            CircleShape flameDrop1(3.f * scaleX);
            flameDrop1.setFillColor(Color(255, 150, 0, 150));
            flameDrop1.setPosition(x + 2.f * scaleX, y + 85.f * scaleY);
            window.draw(flameDrop1);

            CircleShape flameDrop2(3.f * scaleX);
            flameDrop2.setFillColor(Color(255, 150, 0, 150));
            flameDrop2.setPosition(x + 24.f * scaleX, y + 87.f * scaleY);
            window.draw(flameDrop2);
        }
    }

    void DrawPlatform(RenderWindow& window, float x, float y) {
        RectangleShape platform(Vector2f(PLATES_WIDTH, 14.f));
        platform.setPosition(x, y);
        platform.setFillColor(Color(100, 200, 100));
        platform.setOutlineColor(Color::Green);
        platform.setOutlineThickness(2.f);
        window.draw(platform);
    }

    void DrawPlatformScaled(RenderWindow& window, float x, float y, float scaleX, float scaleY) {
        x *= scaleX;
        y *= scaleY;

        // Platformos šešėlis
        RectangleShape platformShadow(Vector2f(PLATES_WIDTH * scaleX + 2.f, 3.f * scaleY));
        platformShadow.setFillColor(Color(0, 0, 0, 60));
        platformShadow.setPosition(x - 1.f, y + 14.f * scaleY);
        window.draw(platformShadow);

        // Pagrindinė platforma
        RectangleShape platform(Vector2f(PLATES_WIDTH * scaleX, 14.f * scaleY));
        platform.setPosition(x, y);
        platform.setFillColor(Color(80, 200, 80));
        window.draw(platform);

        // Platformos blizgesys
        RectangleShape platformTop(Vector2f(PLATES_WIDTH * scaleX - 2.f, 4.f * scaleY));
        platformTop.setPosition(x + 1.f, y + 1.f * scaleY);
        platformTop.setFillColor(Color(150, 255, 150, 180));
        window.draw(platformTop);

        // Platformos apačia
        RectangleShape platformBottom(Vector2f(PLATES_WIDTH * scaleX, 3.f * scaleY));
        platformBottom.setPosition(x, y + 11.f * scaleY);
        platformBottom.setFillColor(Color(40, 120, 40, 150));
        window.draw(platformBottom);

        // Platformos šonai
        RectangleShape platformLeft(Vector2f(2.f * scaleX, 14.f * scaleY));
        platformLeft.setPosition(x, y);
        platformLeft.setFillColor(Color(50, 150, 50));
        window.draw(platformLeft);

        RectangleShape platformRight(Vector2f(2.f * scaleX, 14.f * scaleY));
        platformRight.setPosition(x + PLATES_WIDTH * scaleX - 2.f, y);
        platformRight.setFillColor(Color(50, 150, 50));
        window.draw(platformRight);
    }

    void DrawBackground(RenderWindow& window, float scaleX, float scaleY) {
        // Fonas
        RectangleShape backgroundTop(Vector2f(WINDOW_WIDTH * scaleX, WINDOW_HEIGHT * scaleY * 0.7f));
        backgroundTop.setFillColor(Color(15, 15, 35));
        backgroundTop.setPosition(0, 0);
        window.draw(backgroundTop);

        RectangleShape backgroundBottom(Vector2f(WINDOW_WIDTH * scaleX, WINDOW_HEIGHT * scaleY * 0.3f));
        backgroundBottom.setFillColor(Color(25, 20, 45));
        backgroundBottom.setPosition(0, WINDOW_HEIGHT * scaleY * 0.7f);
        window.draw(backgroundBottom);

        // Nebula efektas
        CircleShape nebula1(60.f * scaleX);
        nebula1.setFillColor(Color(100, 50, 150, 40));
        nebula1.setPosition(-20.f * scaleX, 80.f * scaleY);
        window.draw(nebula1);

        CircleShape nebula2(80.f * scaleX);
        nebula2.setFillColor(Color(50, 100, 150, 30));
        nebula2.setPosition(WINDOW_WIDTH * scaleX - 60.f * scaleX, WINDOW_HEIGHT * scaleY - 100.f * scaleY);
        window.draw(nebula2);

        // Žvaigždės
        for (int i = 0; i < 50; i++) {
            float size = (i % 3 == 0) ? 2.5f : ((i % 3 == 1) ? 1.5f : 0.8f);
            CircleShape star(size * scaleX);

            float starX = ((i * 67 + i * 13) % static_cast<int>(WINDOW_WIDTH * scaleX));
            float starY = ((i * 97 + i * 31) % static_cast<int>(WINDOW_HEIGHT * scaleY));

            star.setPosition(starX, starY);

            if (i % 5 == 0) {
                star.setFillColor(Color(255, 255, 200, 200));
            } else if (i % 5 == 1) {
                star.setFillColor(Color(200, 220, 255, 220));
            } else if (i % 5 == 2) {
                star.setFillColor(Color(255, 200, 200, 180));
            } else if (i % 5 == 3) {
                star.setFillColor(Color(200, 255, 200, 190));
            } else {
                star.setFillColor(Color(255, 240, 200, 210));
            }

            window.draw(star);
        }
    }

    void CenterOrigin(Text& text) {
        FloatRect rect = text.getLocalBounds();
        text.setOrigin(rect.left + rect.width / 2.0f, rect.top + rect.height / 2.0f);
    }

} // namespace graphics

