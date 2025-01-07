#include <cstdlib>
#include "RBNode.h"
#include "IODialog.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

void startProcessing(RBTree*);
void perform(RBTree*, int);
void drawTree(sf::RenderWindow &window, RBNode* node, float x, float y, float offsetX, float offsetY, sf::Font& font, RBNode* highlightNode = nullptr) {
    if (node == RBNode::Nil) return;  // Base case: No node to draw

    // Draw the edges to the children first
    if (node->left != RBNode::Nil) {
        sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(x, y), sf::Color::Black),
                sf::Vertex(sf::Vector2f(x - offsetX, y + offsetY), sf::Color::Black)
        };
        window.draw(line, 2, sf::Lines);
        drawTree(window, node->left, x - offsetX, y + offsetY, offsetX / 2, offsetY, font, highlightNode);
    }

    if (node->right != RBNode::Nil) {
        sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(x, y), sf::Color::Black),
                sf::Vertex(sf::Vector2f(x + offsetX, y + offsetY), sf::Color::Black)
        };
        window.draw(line, 2, sf::Lines);
        drawTree(window, node->right, x + offsetX, y + offsetY, offsetX / 2, offsetY, font, highlightNode);
    }

    // Highlight the node if it matches the highlightNode
    if (node == highlightNode) {
        sf::CircleShape highlight(30);  // Slightly larger circle for highlight
        highlight.setFillColor(sf::Color::Transparent);
        highlight.setOutlineColor(sf::Color::Yellow);
        highlight.setOutlineThickness(5);
        highlight.setPosition(x - 30, y - 30);  // Adjust position for the larger circle
        window.draw(highlight);
    }

    // Draw the current node as a circle
    sf::CircleShape nodeShape(20);
    nodeShape.setFillColor(node->col == RBNode::RED ? sf::Color::Red : sf::Color::Black);
    nodeShape.setPosition(x - 20, y - 20);
    window.draw(nodeShape);

    // Draw the node's key as text
    sf::Text nodeText;
    nodeText.setFont(font);
    nodeText.setString(std::to_string(node->key));
    nodeText.setCharacterSize(15);
    nodeText.setFillColor(sf::Color::White);
    nodeText.setPosition(x - 10, y - 10);
    window.draw(nodeText);
}

int main() {
    // Create the Red-Black tree object
    RBTree* RB = new RBTree();

    startProcessing(RB);
    delete RB;
    return 0;
}

void startProcessing(RBTree* RB) {
    int o;
    while (true) {
        o = IODialog::getTreeOperation();
        perform(RB, o);
    }
}

void perform(RBTree* RB, int o) {
    list<int> nodeKeys;
    RBNode *rbn;
    int skey;

    switch (o) {
        case 1: // Add nodes
            IODialog::getNodeKeys(nodeKeys);
            for (list<int>::iterator it = nodeKeys.begin(); it != nodeKeys.end(); it++)
                RB->RBInsert(RB->createNode(*it));
            break;
        case 2: // Delete node
            skey = IODialog::getNodeKey();
            if (skey == -1) {
                // Invalid input; skip the operation and return to the menu
                break;
            }
            rbn = RB->search(RB->root, skey);
            if (!RB->isNil(rbn)) {
                RB->del(rbn);
                cout << "\033[1;32m✔️ Node with key " << skey << " deleted.\033[0m" << endl;
            } else {
                cout << "\033[1;31m❌ \033[33mRB: Node not found.\033[0m" << endl;
            }
            break;

        case 3: // Get minimum node
            rbn = RB->minimum(RB->root);
            if (RB->isNil(rbn)) {
                cout << "\033[1;31m❌ \033[33mRB: Minimum not found.\033[0m" << endl;
            } else {
                cout << "\033[1;32m✔️ RB: Minimum is: \033[1;34m" << rbn->toString() << "\033[0m" << endl;
                // Visualization
                sf::RenderWindow window(sf::VideoMode(800, 600), "Red-Black Tree Visualization");
                sf::Font font;
                if (!font.loadFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
                    cout << "\033[1;31m❌ Error loading font.\033[0m" << endl;
                    break;
                }
                while (window.isOpen()) {
                    sf::Event event;
                    while (window.pollEvent(event)) {
                        if (event.type == sf::Event::Closed)
                            window.close();
                    }
                    window.clear(sf::Color::White);
                    drawTree(window, RB->root, 400, 50, 100, 100, font, rbn);
                    window.display();
                }
            }
            break;
        case 4: // Get maximum node
            rbn = RB->maximum(RB->root);
            if (RB->isNil(rbn)) {
                cout << "\033[1;31m❌ \033[33mRB: Maximum not found.\033[0m" << endl;
            } else {
                cout << "\033[1;32m✔️ RB: Maximum is: \033[1;34m" << rbn->toString() << "\033[0m" << endl;
                // Visualization
                sf::RenderWindow window(sf::VideoMode(800, 600), "Red-Black Tree Visualization");
                sf::Font font;
                if (!font.loadFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
                    cout << "\033[1;31m❌ Error loading font.\033[0m" << endl;
                    break;
                }
                while (window.isOpen()) {
                    sf::Event event;
                    while (window.pollEvent(event)) {
                        if (event.type == sf::Event::Closed)
                            window.close();
                    }
                    window.clear(sf::Color::White);
                    drawTree(window, RB->root, 400, 50, 100, 100, font, rbn);
                    window.display();
                }
            }
            break;
        case 5: // Get successor of specified node
            skey = IODialog::getNodeKey();
            rbn = RB->search(RB->root, skey);
            if (!RB->isNil(rbn)) {
                rbn = RB->successor(rbn);
                if (RB->isNil(rbn)) {
                    cout << "\033[1;31m❌ \033[33mRB: Node has no successor.\033[0m" << endl;
                } else {
                    cout << "\033[1;32m✔️ RB: Successor is: \033[1;34m" << rbn->toString() << "\033[0m" << endl;
                    // Visualization
                    sf::RenderWindow window(sf::VideoMode(800, 600), "Red-Black Tree Visualization");
                    sf::Font font;
                    if (!font.loadFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
                        cout << "\033[1;31m❌ Error loading font.\033[0m" << endl;
                        break;
                    }
                    while (window.isOpen()) {
                        sf::Event event;
                        while (window.pollEvent(event)) {
                            if (event.type == sf::Event::Closed)
                                window.close();
                        }
                        window.clear(sf::Color::White);
                        drawTree(window, RB->root, 400, 50, 100, 100, font, rbn);
                        window.display();
                    }
                }
            }  else {
                cout << "\033[1;31m❌ \033[33mRB: Node not found.\033[0m" << endl;
            }
            break;
        case 6: // Get predecessor of specified node
            skey = IODialog::getNodeKey();
            rbn = RB->search(RB->root, skey);
            if (!RB->isNil(rbn)) {
                rbn = RB->predecessor(rbn);
                if (RB->isNil(rbn)) {
                    cout << "\033[1;31m❌ \033[33mRB: Node has no predecessor.\033[0m" << endl;
                } else {
                    cout << "\033[1;32m✔️ RB: Predecessor is: \033[1;34m" << rbn->toString() << "\033[0m" << endl;
                    // Visualization
                    sf::RenderWindow window(sf::VideoMode(800, 600), "Red-Black Tree Visualization");
                    sf::Font font;
                    if (!font.loadFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
                        cout << "Error loading font." << endl;
                        break;
                    }
                    while (window.isOpen()) {
                        sf::Event event;
                        while (window.pollEvent(event)) {
                            if (event.type == sf::Event::Closed)
                                window.close();
                        }
                        window.clear(sf::Color::White);
                        drawTree(window, RB->root, 400, 50, 100, 100, font, rbn);
                        window.display();
                    }
                }
            }  else {
                cout << "\033[1;31m❌ \033[33mRB: Node not found.\033[0m" << endl;
            }
            break;
        case 7: // Show tree RB (SFML Visualization)
        {
            sf::RenderWindow window(sf::VideoMode(800, 600), "Red-Black Tree Visualization");
            sf::Font font;
            if (!font.loadFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
                cout << "\033[1;31m❌ Error loading font.\033[0m" << endl;
                break;
            }
            while (window.isOpen()) {
                sf::Event event;
                while (window.pollEvent(event)) {
                    if (event.type == sf::Event::Closed)
                        window.close();
                }
                window.clear(sf::Color::White);
                drawTree(window, RB->root, 400, 50, 100, 100, font);
                window.display();
            }
        }
            break;
        case 8:
            cout << "\n\033[1;33mInorder traversal of RB tree yields:\033[0m" << ' ';
            RB->inorder();
            cout << "\033[0m" <<endl;
            break;
        case 9: // Show black-height of RB
            cout << "\033[1;33mThe black-height of the RB tree is: \033[1;34m" << RB->bh() << "\033[0m" << endl;
            break;
        case 10: { // Maximum key of BLACK nodes
            int maxKey = RB->maxBlackKey(RB->root);
            if (maxKey != -1000) {
                cout << "\033[1;33mThe max key of a black node in the RB tree is \033[1;34m" << maxKey << "\033[0m" << endl;

                sf::RenderWindow window(sf::VideoMode(800, 600), "Red-Black Tree Visualization");
                sf::Font font;
                if (!font.loadFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
                    cout << "\033[1;31m❌ Error loading font.\033[0m" << endl;
                    break;
                }

                RBNode* nodeToHighlight = RB->search(RB->root, maxKey); // Use search instead of findNodeByKey

                while (window.isOpen()) {
                    sf::Event event;
                    while (window.pollEvent(event)) {
                        if (event.type == sf::Event::Closed)
                            window.close();
                    }

                    window.clear(sf::Color::White);
                    drawTree(window, RB->root, 400, 50, 100, 100, font, nodeToHighlight); // Highlight the node
                    window.display();
                }
            } else {
                cout << "\033[1;31m❌ \033[33mNo black nodes in the RB tree.\033[0m" << endl;
            }
            break;
        }

        case 11: { // Maximum key of RED nodes
            int maxKey = RB->maxRedKey(RB->root);
            if (maxKey != -1000) {
                cout << "\033[1;33mThe max key of a red node in the RB tree is \033[1;34m" << maxKey << "\033[0m" << endl;

                sf::RenderWindow window(sf::VideoMode(800, 600), "Red-Black Tree Visualization");
                sf::Font font;
                if (!font.loadFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
                    cout << "\033[1;31m❌ \033[33mError loading font.\033[0m" << endl;
                    break;
                }

                RBNode* nodeToHighlight = RB->search(RB->root, maxKey); // Use search instead of findNodeByKey

                while (window.isOpen()) {
                    sf::Event event;
                    while (window.pollEvent(event)) {
                        if (event.type == sf::Event::Closed)
                            window.close();
                    }

                    window.clear(sf::Color::White);
                    drawTree(window, RB->root, 400, 50, 100, 100, font, nodeToHighlight); // Highlight the node
                    window.display();
                }
            } else {
                cout << "\033[1;31m❌ \033[33mNo red nodes in the RB tree.\033[0m" << endl;
            }
            break;
        }


        case 12: // Compute and show tree depth of RB
            cout << "\033[1;33m The tree depth of the RB tree is \033[1;34m" << RB->depth() << "\033[0m" << endl;
            break;

        case 13: // Exit
            delete RB;
            cout << "\033[1;31m👋 Bye! Exiting the program...\033[0m" << endl;
            exit(0);
    }
}