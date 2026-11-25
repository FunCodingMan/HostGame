#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class EntityHUD
{
public:
    EntityHUD();

    void Init(); 

    void SetPosition(sf::Vector2f entityPosition);

    void Update(float currentValue, float maxValue, sf::Color barColor);
    void UpdateAmmo(int current, int maxAmmo);
    void HideAmmo();
    void Draw(sf::RenderWindow& window);

private:
    void UpdateAmmoPositions();

    sf::RectangleShape m_bgBar; 
    sf::RectangleShape m_fgBar; 

    std::vector<sf::CircleShape> m_ammoDots;
    sf::Vector2f m_entityPosition{0.f, 0.f};
    bool m_showAmmo = false;

    float m_maxWidth;
};