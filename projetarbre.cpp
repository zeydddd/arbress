#include <SFML/Graphics.hpp>
#include <memory>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int NODE_RADIUS = 20;

struct Node {
    int value;
    std::shared_ptr<Node> child;
    std::shared_ptr<Node> sibling;
};

std::shared_ptr<Node> createNode(int value) {
    std::shared_ptr<Node> newNode = std::make_shared<Node>();
    newNode->value = value;
    newNode->child = nullptr;
    newNode->sibling = nullptr;
    return newNode;
}

std::shared_ptr<Node> insertNode(std::shared_ptr<Node> root, int value) {
    if (root == nullptr) {
        return createNode(value);
    }
    std::shared_ptr<Node> newNode = createNode(value);
    if (root->child == nullptr) {
        root->child = newNode;
    } else {
        std::shared_ptr<Node> current = root->child;
        while (current->sibling != nullptr) {
            current = current->sibling;
        }
        current->sibling = newNode;
    }
    return newNode;
}

void drawNode(sf::RenderWindow& window, std::shared_ptr<Node> node, float x, float y, float level) {
    if (node == nullptr) return;
    sf::CircleShape circle(NODE_RADIUS);
    circle.setFillColor(sf::Color::White);
    circle.setOutlineColor(sf::Color::Black);
    circle.setOutlineThickness(2);
    circle.setPosition(x - NODE_RADIUS, y - NODE_RADIUS);
    window.draw(circle);
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        // Handle font loading failure if needed
        return;
    }

    sf::Text text(std::to_string(node->value), font, 24);
    text.setFillColor(sf::Color::Black);
    text.setPosition(x - NODE_RADIUS / 2, y - NODE_RADIUS / 2);
    window.draw(text);
    if (node->child != nullptr) {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(x, y + NODE_RADIUS), sf::Color::Black),
            sf::Vertex(sf::Vector2f(x - 50 + level * 25, y + NODE_RADIUS + 50), sf::Color::Black)
        };
        window.draw(line, 2, sf::Lines);
        drawNode(window, node->child, x - 50 + level * 25, y + NODE_RADIUS + 50, level + 1);
    }
    if (node->sibling != nullptr) {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(x, y + NODE_RADIUS), sf::Color::Black),
            sf::Vertex(sf::Vector2f(x + 50 - level * 25, y + NODE_RADIUS + 50), sf::Color::Black)
        };
        window.draw(line, 2, sf::Lines);
        drawNode(window, node->sibling, x + 50 - level * 25, y + NODE_RADIUS + 50, level + 1);
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "SFML Tree");
    std::shared_ptr<Node> root = createNode(1);
    insertNode(root, 2);
    insertNode(root, 3);
    insertNode(root->child, 4);
    insertNode(root->child, 5);
    insertNode(root->child->sibling, 6);
    insertNode(root->child->sibling, 7);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear(sf::Color::White);
        drawNode(window, root, SCREEN_WIDTH / 2, 50, 0);
        window.display();
    }

    return 0;
}

