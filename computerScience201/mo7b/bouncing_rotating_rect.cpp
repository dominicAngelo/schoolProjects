#include <SFML/Graphics.hpp>

int main() {
    const int PW = 800;
    const int PH = 800;
    sf::RenderWindow window(sf::VideoMode({PW, PH}), "Module 7b: Bouncing Rotating Rectangle");

    float RW = 100.0f;
    float RH = 60.0f;
    sf::RectangleShape rectangle({RW, RH});

    rectangle.setOrigin({RW / 2.0f, RH / 2.0f});

    rectangle.setPosition({0, 0});

    rectangle.setFillColor(sf::Color::Transparent);
    rectangle.setOutlineColor(sf::Color::White);
    rectangle.setOutlineThickness(2.0f);

    float moveSpeed = 0.01f; 
    float rotateSpeed = 0.1f;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        rectangle.move({moveSpeed, moveSpeed});
        rectangle.rotate(sf::degrees(rotateSpeed));

        if (rectangle.getPosition().x + RW >= PW || rectangle.getPosition().x <= 0) {
            moveSpeed = -moveSpeed;
        }

        window.clear(sf::Color::Black);
        window.draw(rectangle); 
        window.display();
    }

    return 0;
}