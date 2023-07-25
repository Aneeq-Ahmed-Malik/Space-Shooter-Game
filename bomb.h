#include <SFML/Graphics.hpp>
#include <string.h>
#include <iostream>
#include <cmath>
using namespace sf;

#ifndef BOMB_H_
#define BOMB_H_

class Bomb
{
private:
    int x, y;
    double speed;

public:
    Texture tex;
    Sprite sprite;

    Bomb(std::string pngPath = "img/enemy_laser.png");
    Bomb(int x, int y, std::string pngPath = "img/enemy_laser.png");
    Bomb(const Bomb &copy);
    void operator=(const Bomb &copy);
    void move();
    bool checkBounds();
};

Bomb::Bomb(std::string pngPath)
{
    tex.loadFromFile(pngPath);
    sprite.setTexture(tex);
    this->speed = 2.8;
}

Bomb::Bomb(int x, int y, std::string pngPath)
{
    tex.loadFromFile(pngPath);
    sprite.setTexture(tex);
    this->x = x;
    this->y = y;
    sprite.setPosition(x, y);
    this->speed = 2.8;
}

Bomb::Bomb(const Bomb &copy)
{
    this->tex.update(copy.tex);
    this->sprite.setPosition(copy.sprite.getPosition());
    this->speed = copy.speed;
}

void Bomb::operator=(const Bomb &copy)
{
    this->tex.update(copy.tex);
    this->sprite.setPosition(copy.sprite.getPosition());
    this->speed = copy.speed;
}

void Bomb::move()
{

    this->sprite.move(0, this->speed);
}

bool Bomb::checkBounds()
{
    if (this->sprite.getPosition().y > 740)
        return true;
    return false;
}

#endif