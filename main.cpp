#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>
#include <cstdlib> // Needed for rand() to generate random enemy locations

// ============================================================================
// 1. ABSTRACT BASE CLASS
// ============================================================================
class Entity {
public:
    sf::Vector2f position;
    sf::Vector2f velocity;
    bool isDestroyed = false;

    Entity(float x, float y) : position(x, y), velocity(0.0f, 0.0f) {}
    virtual ~Entity() = default;

    virtual void update() = 0;
    virtual void draw(sf::RenderWindow& window) = 0; 

    // SFML 3 expects two Vector2f parameters (position, size) instead of 4 floats
    sf::FloatRect getBounds() const {
        return sf::FloatRect(position, sf::Vector2f(30.0f, 30.0f));
    }
};

// ============================================================================
// 2. DERIVED CLASSES
// ============================================================================
class Player : public Entity {
public:
    sf::RectangleShape shape;

    Player(float x, float y) : Entity(x, y) {
        shape.setSize(sf::Vector2f(30.0f, 30.0f));
        shape.setFillColor(sf::Color::Green);
    }

    void update() override {
        position += velocity;
        velocity = sf::Vector2f(0.0f, 0.0f);
        shape.setPosition(position);
    }

    void draw(sf::RenderWindow& window) override {
        window.draw(shape);
    }
};

class Bullet : public Entity {
public:
    sf::RectangleShape shape;

    Bullet(float x, float y) : Entity(x, y) {
        shape.setSize(sf::Vector2f(5.0f, 15.0f));
        shape.setFillColor(sf::Color::Yellow);
        velocity.y = -7.0f;
    }

    void update() override {
        position += velocity;
        shape.setPosition(position);
        if (position.y < 0) isDestroyed = true;
    }

    void draw(sf::RenderWindow& window) override {
        window.draw(shape);
    }
};

class Enemy : public Entity {
public:
    sf::CircleShape shape;

    Enemy(float x, float y) : Entity(x, y) {
        shape.setRadius(15.0f);
        shape.setFillColor(sf::Color::Red);
        velocity.y = 2.0f;
    }

    void update() override {
        position += velocity;
        shape.setPosition(position);
        // If an enemy slips past, we mark it destroyed so it gets cleaned up from memory
        if (position.y > 650) isDestroyed = true;
    }

    void draw(sf::RenderWindow& window) override {
        window.draw(shape);
    }
};

// ============================================================================
// 3. GAME STATE ENGINE
// ============================================================================
int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "C++ Arcade Architecture");
    window.setFramerateLimit(60);

    Player player(400.0f, 500.0f);
    
    std::vector<std::unique_ptr<Bullet>> bullets;
    std::vector<std::unique_ptr<Enemy>> enemies;

    // 1. ADDED SPAWN CLOCK TRACKER
    // Tracks frame counts right before the game loop kicks off
    int enemySpawnTimer = 0;

    while (window.isOpen()) {
        
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::Space) {
                    bullets.push_back(std::make_unique<Bullet>(player.position.x + 12.0f, player.position.y));
                }
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))  player.velocity.x = -5.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) player.velocity.x = 5.0f;

        player.update();

        // 2. ADDED AUTO-SPAWNER LOGIC
        // Increments every frame (60 frames = 1 second) and spawns a target dynamically
        enemySpawnTimer++;
        if (enemySpawnTimer >= 45) { // Spawns a brand new target roughly every 0.75 seconds
            float randomX = static_cast<float>(50 + (rand() % 700)); 
            enemies.push_back(std::make_unique<Enemy>(randomX, -30.0f));
            enemySpawnTimer = 0; // Reset the clock counter
        }

        for (auto& bullet : bullets) bullet->update();
        for (auto& enemy : enemies)   enemy->update();

        // Overlap Math Box Checks
        for (auto& bullet : bullets) {
            for (auto& enemy : enemies) {
                if (bullet->getBounds().findIntersection(enemy->getBounds())) {
                    bullet->isDestroyed = true;
                    enemy->isDestroyed = true;
                    std::cout << "Collision Registered!\n";
                }
            }
        }

        std::erase_if(bullets, [](const std::unique_ptr<Bullet>& b) { return b->isDestroyed; });
        std::erase_if(enemies, [](const std::unique_ptr<Enemy>& e) { return e->isDestroyed; });

        window.clear(sf::Color::Black);

        player.draw(window);
        for (auto& bullet : bullets) bullet->draw(window);
        for (auto& enemy : enemies)   enemy->draw(window);

        window.display();
    }

    return 0;
}
