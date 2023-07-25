#include <SFML/Graphics.hpp>
#include <string.h>
#include <iostream>
#include <cmath>
using namespace sf;

#ifndef FIRE_H_
#define FIRE_H_

class Fire
{
private:
    int x, y, rotation;
    bool On;
    string path[3] = {"img/fire.png", "img/fire_55.png", "img/fire_-55.png"};

public:
    Texture tex;
    Sprite sprite;

    Fire(std::string pngPath = "img/fire.png");
    Fire(int x, int y, std::string pngPath = "img/fire.png");
    Fire(const Fire &copy);

    void operator=(const Fire &copy);

    void rotate(int direction);
    bool &isOn();
    int &rotated();
};

Fire::Fire(std::string pngPath)
{
    tex.loadFromFile(pngPath);
    sprite.setTexture(tex);
    sprite.setScale(1, 1.8);
    this->rotation = 0;
}

Fire::Fire(int x, int y, std::string pngPath)
{
    tex.loadFromFile(pngPath);
    sprite.setTexture(tex);
    this->x = x;
    this->y = y;
    sprite.setScale(1, 1.8);
    sprite.setPosition(x, y);
}

void Fire::rotate(int direction)
{
    this->rotation = direction;

    Sprite temp;        // temp sprite to store rotated fire        
    tex.loadFromFile(path[direction]);      // pick rotated fire acc to direction
    temp.setTexture(tex);
    if (rotation != 0)
        temp.setScale(1.2, 2.3);
    else
        temp.setScale(1, 1.8);

    temp.setPosition(sprite.getPosition().x, sprite.getPosition().y);

    sprite = temp;      // changing the actual sprite
}

bool &Fire::isOn()
{
    return this->On;
}

int &Fire::rotated()
{
    return this->rotation;
}

Fire::Fire(const Fire &copy)
{
    this->tex.update(copy.tex);
    this->sprite.setPosition(copy.sprite.getPosition());
}

void Fire::operator=(const Fire &copy)
{
    this->tex.update(copy.tex);
    this->sprite.setPosition(copy.sprite.getPosition());
}

#endif