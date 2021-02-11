
#include "Windmill.h"


TowerNode::TowerNode()
{
    m_shape.setSize(sf::Vector2f(100, 200));
    m_shape.setOrigin(50, 50);
}

void TowerNode::Update(const sf::Transform& tTransform) {
    m_rotation += 1.5 * (float)m_direction;
    sf::Transform t;
    t.rotate(m_rotation);
    SetTransform(t);

    // draw its children
    for (std::size_t i = 0; i < GetNodeCount(); ++i)
    {
        GetNode(i)->Update(m_shape.getTransform());
    }
}

BladeNode::BladeNode()
{
    m_shape.setSize(sf::Vector2f(100, 100));
    m_shape.setOrigin(50, 50);
}

void BladeNode::Update(const sf::Transform& parentTransform) {
    sf::Transform t1 = parentTransform;
    t1.translate(50, 50);
    m_rotation += 2 * m_direction;
    t1.rotate(m_rotation);
    SetTransform(t1);

    // draw its children
    for (std::size_t i = 0; i < GetNodeCount(); ++i)
    {
        GetNode(i)->Update(m_shape.getTransform());
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SFML works!");

    //Music Settings
    sf::Music music;
    music.openFromFile("AmbientForest.wav");
    music.setLoop(true);
    music.play();

    //Background Variables
    sf::RectangleShape background(sf::Vector2f(WIDTH * 2, HEIGHT));
    sf::Texture backgroundTexture;
    float backgroundSpeed = 1.0f;

    //Background config
    backgroundTexture.loadFromFile("Bliss.png");
    background.setTexture(&backgroundTexture);
    background.setPosition(-(WIDTH / 2), 0);

    //Tower Texture
    sf::Texture towerTexture;
    towerTexture.loadFromFile("Tower.png");

    //Blade Texture
    sf::Texture bladeTexture;
    bladeTexture.loadFromFile("Blades.png");


    bool orbit = false;
    bool tPress = false;

    bool num1Pressed = false;
    bool num2Pressed = false;
    bool num3Pressed = false;
    bool num4Pressed = false;

    int direction = 1;

    sf::Vector2i lastMousePos = sf::Mouse::getPosition(window);

    TowerNode windmills[3];

    BladeNode* blades[3];

    for (int i = 0; i < 3; i++) {
        blades[i] = new BladeNode();
        blades[i]->SetTexture(bladeTexture);
        windmills[i].AddChildren(blades[i]);
    }

    for (int i = 0; i < 3; i++) {
        windmills[i].SetTexture(towerTexture);
    }


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:

                if (event.key.code == sf::Keyboard::Num1) {
                    windmills[0].SetDirection(direction);
                    num1Pressed = true;
                }
                if (event.key.code == sf::Keyboard::Num2) {
                    windmills[1].SetDirection(direction);
                    num2Pressed = true;
                }
                if (event.key.code == sf::Keyboard::Num3) {
                    windmills[2].SetDirection(direction);
                    num3Pressed = true;
                }

                if (event.key.code == sf::Keyboard::Num4 || event.key.code == sf::Keyboard::A) {
                    orbit = !orbit;


                    if (orbit) {
                        num4Pressed = true;

                        for (int i = 0; i < 3; i++) {
                            windmills[i].SetOrigin(300, 50);
                            windmills[i].SetDirection(direction);
                        }

                        windmills[0].SetRotation(0.0f);
                        windmills[1].SetRotation(120.0f);
                        windmills[2].SetRotation(240.0f);

                    }
                    else {
                        num4Pressed = false;

                        for (int i = 0; i < 3; i++) {
                            windmills[i].SetOrigin(50, 50);
                            windmills[i].SetDirection(0);
                        }

                        windmills[0].SetRotation(0.0f);
                        windmills[1].SetRotation(0.0f);
                        windmills[2].SetRotation(0.0f);
                    }
                }
                if (event.key.code == sf::Keyboard::T) {
                    tPress = !tPress;
                }
                break;
            case sf::Event::KeyReleased:
                if (event.key.code == sf::Keyboard::Num1) {
                    windmills[0].SetDirection(0);
                    num1Pressed = false;
                }
                if (event.key.code == sf::Keyboard::Num2) {
                    windmills[1].SetDirection(0);
                    num2Pressed = false;
                }
                if (event.key.code == sf::Keyboard::Num3) {
                    windmills[2].SetDirection(0);
                    num3Pressed = false;
                }
                break;
            case sf::Event::MouseMoved:
                sf::Vector2i currentMousePos = sf::Mouse::getPosition(window);
                if (abs(currentMousePos.x - lastMousePos.x) > abs(currentMousePos.y - lastMousePos.y)) {
                    direction = -1;
                }
                else {
                    direction = 1;
                }

                if (num1Pressed) {
                    windmills[0].SetDirection(direction);
                }
                if (num2Pressed) {
                    windmills[1].SetDirection(direction);
                }
                if (num3Pressed) {
                    windmills[2].SetDirection(direction);
                }
                if (num4Pressed) {
                    for (int i = 0; i < 3; i++) {
                        windmills[i].SetDirection(direction);
                    }
                }

                lastMousePos = currentMousePos;
                break;
            }
        }


        //create initial transform for the parent
        sf::Transform t;
        t.translate(WIDTH / 2, HEIGHT / 2);


        sf::Transform wT[3];

        if (tPress) {
            wT[0].translate(200, 200);
            wT[1].translate(400, 400);
            wT[2].translate(600, 600);
        }
        else if (orbit) {
            wT[0].translate(WIDTH / 2, HEIGHT / 2);
            wT[1].translate(WIDTH / 2, HEIGHT / 2);
            wT[2].translate(WIDTH / 2, HEIGHT / 2);
        }
        else {
            wT[0].translate(256, HEIGHT / 2);
            wT[1].translate(512, HEIGHT / 2);
            wT[2].translate(768, HEIGHT / 2);
        }



        // Draw
        window.clear();
        window.setFramerateLimit(30);


        if (background.getPosition().x >= 0 || background.getPosition().x <= -WIDTH)
            backgroundSpeed *= -1;

        background.move(backgroundSpeed, 0.0f);
        window.draw(background);


        for (int i = 0; i < 3; i++) {
            windmills[i].Update(wT[i]);
            windmills[i].draw(window, wT[i]);
        }


        window.display();
    }

    return 0;
}
