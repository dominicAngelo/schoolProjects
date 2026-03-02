#include <SFML/Graphics.hpp>

int main() {
    const int PW = 800;
    const int PH = 800;
    sf::RenderWindow window(sf::VideoMode({PW, PH}), "Module 7b: Bouncing Rotating Rectangle");

    float RW = 100.0f;
    float RH = 60.0f;
    sf::RectangleShape rect1({RW, RH});

    //rect1.setOrigin({RW / 2.0f, RH / 2.0f});
    rect1.setOrigin({0, 0});

    rect1.setPosition({0, 0});

    rect1.setFillColor(sf::Color::Transparent);
    rect1.setOutlineColor(sf::Color::White);
    rect1.setOutlineThickness(2.0f);

    float speed = 0.01f; 

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        rect1.move({speed, speed});

        if (rect1.getPosition().x + RW >= PW || rect1.getPosition().x <= 0) {
            speed = -speed;
        }

        window.clear(sf::Color::Black);
        window.draw(rect1); 
        window.display();
    }

    return 0;
}