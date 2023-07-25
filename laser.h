#include <SFML/Graphics.hpp>
#include <string.h>
#include <iostream>
#include <cmath>
using namespace sf;

#ifndef LASER_H_
#define LASER_H_

class Laser
{
private:
    int x, y;
    bool On;

public:
    Texture tex;
    Sprite sprite;

    Laser(std::string pngPath = "img/laser.png");
    Laser(int x, int y, std::string pngPath = "img/laser(1).png");
    Laser(const Laser &copy);

    void operator=(const Laser &copy);

    bool &isOn();
};

Laser::Laser(std::string pngPath)
{
    tex.loadFromFile(pngPath);
    sprite.setTexture(tex);
    sprite.setScale(0.35, 0.82);
}

Laser::Laser(int x, int y, std::string pngPath)
{
    tex.loadFromFile(pngPath);
    sprite.setTexture(tex);
    this->x = x;
    this->y = y;
    sprite.setPosition(x, y);
}

Laser::Laser(const Laser &copy)
{
    this->tex.update(copy.tex);
    this->sprite.setPosition(copy.sprite.getPosition());
}

void Laser::operator=(const Laser &copy)
{
    this->tex.update(copy.tex);
    this->sprite.setPosition(copy.sprite.getPosition());
}

bool &Laser::isOn()
{
    return this->On;
}

#endif
