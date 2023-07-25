#include <SFML/Graphics.hpp>
#include <string.h>
#include <iostream>
#include <cmath>
using namespace sf;

#ifndef BULLET_H_
#define BULLET_H_

class Bullet
{
private:
    int x, y, rotation;
    int degree;
    double speed;
    bool fire;

public:
    Texture tex;
    Sprite sprite;

    Bullet(std::string pngPath = "img/bullet1.png");
    Bullet(int x, int y, int r, int deg, std::string pngPath = "img/bullet1.png", bool fire = false);
    Bullet(const Bullet &copy);
    void operator=(const Bullet &copy);

    void move();
    bool checkBounds();
    bool onFire();

};

Bullet::Bullet(std::string pngPath)
{
    tex.loadFromFile(pngPath);
    sprite.setTexture(tex);
    this->fire = false;
    this->degree = 0;
    this->speed = 5;
}

Bullet::Bullet(int x, int y, int r, int deg, std::string pngPath, bool fire)
{
    this->rotation = r;
    this->degree = deg;
    tex.loadFromFile(pngPath);
    sprite.setTexture(tex);
    this->x = x;
    this->y = y;
    sprite.setPosition(x, y);
    this->fire = fire;
    this->speed = 5;
}

Bullet::Bullet(const Bullet &copy)
{
    this->tex.update(copy.tex);
    this->rotation = copy.rotation;
    this->fire = copy.fire;
    this->degree = copy.degree;
    if (rotation)
        this->sprite.setScale(0.8, 0.8);
    if (this->fire)
        this->sprite.setScale(0.8, 0.8);
    if (this->fire && this->rotation)
        this->sprite.setScale(0.6, 0.6);
    this->sprite.setPosition(copy.sprite.getPosition());
    this->speed = copy.speed;
}

void Bullet::operator=(const Bullet &copy)
{
    this->tex.update(copy.tex);
    this->rotation = copy.rotation;
    this->fire = copy.fire;
    this->degree = copy.degree;
    this->speed = copy.speed;
    if (rotation)
        this->sprite.setScale(0.8, 0.8);
    if (this->fire)
        this->sprite.setScale(0.8, 0.8);
    if (this->fire && this->rotation)
        this->sprite.setScale(0.6, 0.6);

    this->sprite.setPosition(copy.sprite.getPosition());
}

void Bullet::move()
{
    float radian = this->degree * 3.14159 / 180;

    this->sprite.move(this->speed * cos(radian), -this->speed * sin(radian));
}

bool Bullet::checkBounds()
{
    if (this->sprite.getPosition().y < 0 || this->sprite.getPosition().x < 0 || this->sprite.getPosition().x > 740)
        return true;
    return false;
}

bool Bullet::onFire()
{
    return this->fire;
}

#endif