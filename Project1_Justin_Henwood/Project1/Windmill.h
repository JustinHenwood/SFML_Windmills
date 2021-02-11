

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Audio.hpp>

#define WIDTH 1024
#define HEIGHT 768


class Node
{
public:

    virtual ~Node()
    {
        // destroy its children
        for (std::size_t i = 0; i < m_children.size(); ++i)
        {
            delete m_children[i];
        }
    }

    void AddChildren(Node* pNode) { m_children.push_back(pNode); }
    void SetTransform(const sf::Transform& transform) { m_transform = transform; }
    Node* GetNode(int index) const { return m_children[index]; };
    std::size_t GetNodeCount() const { return m_children.size(); }



    virtual void Update(const sf::Transform& tTransform) = 0;

    void draw(sf::RenderWindow& target, const sf::Transform& parentTransform) const
    {
        // combine the parent transform with the node's one
        sf::Transform combinedTransform = parentTransform * m_transform;

        // let the node draw itself
        onDraw(target, combinedTransform);

        // draw its children
        for (std::size_t i = 0; i < m_children.size(); ++i)
        {
            m_children[i]->draw(target, combinedTransform);
        }

    }

private:
    virtual void onDraw(sf::RenderWindow& target, const sf::Transform& transform) const = 0;

    sf::Transform m_transform;
    std::vector<Node*> m_children;
};

// a simple derived class: a node that draws a sprite
class TowerNode : public Node
{
public:

    TowerNode();
    void Update(const sf::Transform& tTransform);

    void SetOrigin(int x, int y) { m_shape.setOrigin(x, y); }
    void SetDirection(int d) { m_direction = d; }
    void SetRotation(float r) { m_rotation = r; }
    void SetTexture(sf::Texture t) { m_texture = t; m_shape.setTexture(&m_texture); }
private:

    virtual void onDraw(sf::RenderWindow& target, const sf::Transform& transform) const
    {
        target.draw(m_shape, transform);
    }


protected:
    float m_rotation = 0;
    int m_direction = 0;
    sf::RectangleShape m_shape;
    sf::Texture m_texture;
};


class BladeNode : public Node
{
public:
    BladeNode();
    virtual void Update(const sf::Transform& tTransform);
    void SetTexture(sf::Texture t) { m_texture = t; m_shape.setTexture(&m_texture); }
private:

    virtual void onDraw(sf::RenderWindow& target, const sf::Transform& transform) const
    {
        target.draw(m_shape, transform);
    }

protected:
    float m_rotation = 0;
    int m_direction = 1;
    sf::RectangleShape m_shape;
    sf::Texture m_texture;
};


