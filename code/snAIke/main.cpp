#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(400, 400), "snAIke");

    sf::CircleShape circle(200);
    circle.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            window.clear();

            window.draw(circle);

            window.display();
        }
    }
}