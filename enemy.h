#include <SFML/Graphics.hpp>
#include <string.h>
#include <iostream>
#include <cstdlib>
#include <time.h>
#include "bomb.h"
#include "Vector.h"
#include "laser.h"
#include "fire.h"
using namespace sf;

#ifndef ENEMY_H_
#define ENEMY_H_

class Enemy
{
protected:
    int score, interval;
    int x, y;
    bool cooldown;
    bool hasDescended;

public:
    Texture *tex;
    Sprite sprite;
    Clock timer;

    Enemy();

    virtual void setRectangle(int &alpha, int &beta, int &gamma, Enemy **&en) {}
    virtual void setCircle(int &alpha, int &beta, int &gamma, Enemy **&en) {}
    virtual void setTriangle(int &alpha, int &beta, int &gamma, Enemy **&en) {}
    virtual void setCross(int &alpha, int &beta, int &gamma, Enemy **&en) {}
    virtual void setDiamod(int &alpha, int &beta, int &gamma, Enemy **&en) {}
    virtual void setHeart(int &alpha, int &beta, int &gamma, Enemy **&en) {}
    virtual void setFilledRectangle(int &alpha, int &beta, int &gamma, Enemy **&en) {}
    virtual void setFilledDiamod(int &alpha, int &beta, int &gamma, Enemy **&en) {}
    virtual void setFilledTriangle(int &alpha, int &beta, int &gamma, Enemy **&en) {}

    void operator=(const Enemy &copy);

    virtual Enemy *getPtr() = 0;
    virtual Enemy *getPtr(int size) = 0;

    bool has_descended();
    int getScore();
    int getInterval();
    bool &isCooled();

    virtual Laser &getLaser() {}
    virtual Fire &getFire() {}

    virtual void fire(Vector<Bomb> &bomb) {}
    virtual void fire() {}

    virtual void move(int alpha, int beta, int gamma, Enemy **&enemy) {}
    virtual void move() {}

    virtual void descend() {}
    virtual void descend(int alpha, int beta, int gamma, Enemy **&enemy) {}

    virtual void raise_back() {}
    virtual void raise_back(int alpha, int beta, int gamma, Enemy **&enemy) {}

    virtual ~Enemy();
};

Enemy::Enemy()
{
    tex = new Texture[5];
    tex[0].loadFromFile("img/enemy_1.png");
    tex[1].loadFromFile("img/enemy_2.png");
    tex[2].loadFromFile("img/enemy_3.png");
    tex[3].loadFromFile("img/monster1.png");
}

bool Enemy::has_descended()
{
    return this->hasDescended;
}

int Enemy::getScore()
{
    return this->score;
}

int Enemy::getInterval()
{
    return this->interval;
}

bool &Enemy::isCooled()
{
    return this->cooldown;
}

void Enemy::operator=(const Enemy &copy)
{

    this->sprite.setPosition(copy.sprite.getPosition());
}

Enemy::~Enemy()
{
    delete[] tex;
}


class Dragon : public Enemy
{
private:
    Fire *flame;

public:
    Dragon();

    virtual Dragon *getPtr();
    virtual Dragon *getPtr(int size);

    virtual Fire &getFire();
    virtual void descend();
    virtual void raise_back();
    virtual void fire();

    ~Dragon();
};

Dragon::Dragon()
{
    tex[4].loadFromFile("img/dragon.png");
    this->sprite.setTexture(tex[4]);
    this->sprite.setScale(1.2, 1.2);
    this->sprite.setPosition(-400, 90);
    this->interval = 15;
    this->flame = new Fire;
    this->flame->isOn() = false;
    this->hasDescended = false;
    this->score = 50;
}

Dragon *Dragon::getPtr()
{
    return new Dragon;
}

Dragon *Dragon::getPtr(int size)
{
    return new Dragon[size];
}

Fire &Dragon::getFire()
{
    return *this->flame;
}

void Dragon::descend()
{
    if (this->hasDescended)
        return;

    float x_offset = 2;
    if (this->sprite.getPosition().x <= 240)
        this->sprite.move(x_offset, 0);
    else
        this->hasDescended = true;
}

void Dragon::raise_back()
{
    if (!this->hasDescended)
        return;

    float x_offset = -2;
    if (this->sprite.getPosition().x > -400)
        this->sprite.move(x_offset, 0);
    else
        this->hasDescended = false;
}

void Dragon::fire()
{
    if (!this->hasDescended || !flame->isOn())
        return;

    int x_offset = -45, y_offset = 270;

    if (flame->rotated() == 1)      // offsets for perfecion of fire position
    {
        x_offset = 10;
        y_offset = -50;
    }
    if (flame->rotated() == 2)      // offsets for perfecion of fire position
    {
        x_offset = -185;
        y_offset = -70;
    }

    flame->sprite.setPosition(this->sprite.getPosition().x + x_offset, this->sprite.getPosition().y + y_offset);
}

Dragon::~Dragon()
{
    delete flame;
}


class Monster : public Enemy
{
private:
    Laser *laser;

public:
    Monster();

    virtual Monster *getPtr();
    virtual Monster *getPtr(int size);

    virtual Laser &getLaser();
    virtual void raise_back();
    virtual void move();
    virtual void fire();
    virtual void descend();

    ~Monster();
};

Monster::Monster()
{
    this->sprite.setTexture(tex[3]);
    this->sprite.setScale(0.5, 0.5);
    this->sprite.setPosition(220, -400);
    this->interval = 10;
    this->hasDescended = false;
    this->laser = new Laser;
    this->laser->isOn() = false;
    this->score = 40;
}

Monster *Monster::getPtr()
{
    return new Monster;
}

Monster *Monster::getPtr(int size)
{
    return new Monster[size];
}

Laser &Monster::getLaser()
{
    return *this->laser;
}

void Monster::raise_back()
{
    if (!this->hasDescended)
        return;

    float y_offset = -2;
    if (this->sprite.getPosition().y > -440)
        this->sprite.move(0, y_offset);
    else
        this->hasDescended = false;
}

void Monster::move()
{
    if (!this->hasDescended)
        return;
    static float x_offset = 2.4;

    if (this->sprite.getPosition().x + this->sprite.getTexture()->getSize().x * 0.5 > 780)      // right movement
        x_offset *= -1;

    if (this->sprite.getPosition().x < 0)       // left movement
        x_offset = 2.4;

    this->sprite.move(x_offset, 0);
    this->fire();
}

void Monster::fire()
{
    if (!this->hasDescended || !laser->isOn())
        return;
    laser->sprite.setPosition(this->sprite.getPosition().x + 155, this->sprite.getPosition().y + 255);
}

void Monster::descend()
{
    if (this->hasDescended)
        return;

    float y_offset = 2;
    if (this->sprite.getPosition().y < 70)
        this->sprite.move(0, y_offset);
    else
        this->hasDescended = true;
}

Monster::~Monster()
{
    delete laser;
}


class Invader : public Enemy
{

public:
    virtual void setRectangle(int &alpha, int &beta, int &gamma, Enemy **&en);
    virtual void setCircle(int &alpha, int &beta, int &gamma, Enemy **&en);
    virtual void setTriangle(int &alpha, int &beta, int &gamma, Enemy **&en);
    virtual void setCross(int &alpha, int &beta, int &gamma, Enemy **&en);
    virtual void setDiamod(int &alpha, int &beta, int &gamma, Enemy **&en);
    virtual void setHeart(int &alpha, int &beta, int &gamma, Enemy **&en);
    virtual void setFilledRectangle(int &alpha, int &beta, int &gamma, Enemy **&en);
    virtual void setFilledDiamod(int &alpha, int &beta, int &gamma, Enemy **&en);
    virtual void setFilledTriangle(int &alpha, int &beta, int &gamma, Enemy **&en);

    virtual Invader *getPtr() = 0;
    virtual Invader *getPtr(int size) = 0;

    virtual void makeInvaders(int alpha, int beta, int gamma, Enemy **&enemy);

    virtual void fire(Vector<Bomb> &bomb);

    virtual void move(int alpha, int beta, int gamma, Enemy **&enemy);

    virtual void descend(int alpha, int beta, int gamma, Enemy **&enemy);

    virtual void raise_back(int alpha, int beta, int gamma, Enemy **&enemy);
};


class Alpha : public Invader
{
public:
    Alpha();

    virtual Alpha *getPtr();

    virtual Alpha *getPtr(int size);
};

Alpha::Alpha()
{
    this->sprite.setTexture(tex[0]);
    this->sprite.setScale(0.4, 0.4);
    this->sprite.setPosition(340, 100);
    this->cooldown = true;
    this->interval = 5;
    this->score = 10;
}

Alpha *Alpha::getPtr()
{
    return new Alpha();
}

Alpha *Alpha::getPtr(int size)
{
    return new Alpha[size]{};
}


class Beta : public Invader
{
public:
    Beta();

    virtual Beta *getPtr();

    virtual Beta *getPtr(int size);
};

Beta::Beta()
{
    this->sprite.setTexture(tex[1]);
    this->sprite.setPosition(340, 100);
    this->sprite.setScale(0.4, 0.4);
    this->cooldown = true;
    this->interval = 3;
    this->score = 20;
}

Beta *Beta::getPtr()
{
    return new Beta(*this);
}

Beta *Beta::getPtr(int size)
{
    return new Beta[size]{};
}


class Gamma : public Invader
{
public:
    Gamma();

    virtual Gamma *getPtr();

    virtual Gamma *getPtr(int size);
};

Gamma::Gamma()
{
    this->sprite.setTexture(tex[2]);
    this->sprite.setPosition(340, 100);
    this->sprite.setScale(0.3, 0.3);
    this->cooldown = true;
    this->interval = 2;
    this->score = 10;
}

Gamma *Gamma::getPtr()
{
    return new Gamma(*this);
}

Gamma *Gamma::getPtr(int size)
{
    return new Gamma[size];
}


void Invader::makeInvaders(int alpha, int beta, int gamma, Enemy **&enemy)
{
    enemy = new Enemy *[5] {};
    enemy[0] = new Alpha[alpha]{};
    enemy[1] = new Beta[beta]{};
    enemy[2] = new Gamma[gamma]{};
    enemy[3] = new Monster;
    enemy[4] = new Dragon;
}

void Invader::setRectangle(int &alpha, int &beta, int &gamma, Enemy **&en)
{
    alpha = 7;
    beta = 7;
    gamma = 6;
    makeInvaders(alpha, beta, gamma, en);

    this->hasDescended = true;

    for (int i = 0; i < 7; i++)
        en[0][i].sprite.setPosition(i * 110 + 30, 40);

    for (int i = 0; i < 7; i++)
        en[1][i].sprite.setPosition(i * 110 + 30, 310);

    for (int i = 0; i < 3; i++)
        en[2][i].sprite.setPosition(30, i * 65 + 110);

    for (int i = 0, j = 3; j < 6; i++, j++)
        en[2][j].sprite.setPosition(690, i * 65 + 110);
}

void Invader::setCircle(int &alpha, int &beta, int &gamma, Enemy **&en)
{
    alpha = 7;
    beta = 7;
    gamma = 6;
    makeInvaders(alpha, beta, gamma, en);
    this->hasDescended = true;
    float angle = 0, x, y;

    for (int i = 0, j = 0, k = 0; i < 20; i++, k++)
    {
        angle = (2.0 * 3.14159 * i) / 20;

        x = cos(angle) * 260 + 380;
        y = sin(angle) * 200 + 230;

        en[j][k].sprite.setPosition(x, y);

        if (i == 6 || i == 13)
        {
            j++;
            k = -1;
        }
    }
}

void Invader::setTriangle(int &alpha, int &beta, int &gamma, Enemy **&en)
{
    alpha = 7;
    beta = 7;
    gamma = 6;
    makeInvaders(alpha, beta, gamma, en);
    this->hasDescended = true;

    for (int i = 0; i < 7; i++)
        en[0][i].sprite.setPosition(390 + i * 40, i * 50 + 35);
    for (int i = 0; i < 7; i++)
        en[1][i].sprite.setPosition(340 - i * 40, i * 50 + 30);
    for (int i = 0; i < 6; i++)
        en[2][i].sprite.setPosition(180 + 77 * i, 345);
}

void Invader::setCross(int &alpha, int &beta, int &gamma, Enemy **&en)
{
    alpha = 5;
    beta = 4;
    gamma = 4;
    makeInvaders(alpha, beta, gamma, en);
    this->hasDescended = true;

    for (int i = 0; i < 5; i++)
        en[0][i].sprite.setPosition(200 + i * 50, i * 50);

    en[2][0].sprite.setPosition(450, 250);
    en[2][1].sprite.setPosition(500, 300);

    for (int i = 0; i < 3; i++)
        en[1][i].sprite.setPosition(500 - i * 50, i * 50);

    en[1][3].sprite.setPosition(300, 190);
    en[2][2].sprite.setPosition(250, 250);
    en[2][3].sprite.setPosition(200, 300);
}

void Invader::setDiamod(int &alpha, int &beta, int &gamma, Enemy **&en)
{

    alpha = 5;
    beta = 5;
    gamma = 4;
    makeInvaders(alpha, beta, gamma, en);
    this->hasDescended = true;

    for (int i = 0; i < 4; i++)
        en[0][i].sprite.setPosition(390 + i * 50, i * 50 + 30);
    for (int i = 0; i < 4; i++)
        en[1][i].sprite.setPosition(340 - i * 50, i * 50 + 30);

    en[2][0].sprite.setPosition(240, 250);
    en[2][1].sprite.setPosition(290, 300);
    en[2][2].sprite.setPosition(490, 250);
    en[2][3].sprite.setPosition(440, 300);

    en[1][4].sprite.setPosition(330, 350);
    en[0][4].sprite.setPosition(390, 350);
}

void Invader::setFilledDiamod(int &alpha, int &beta, int &gamma, Enemy **&en)
{

    alpha = 10;
    beta = 8;
    gamma = 8;
    makeInvaders(alpha, beta, gamma, en);
    this->hasDescended = true;

    for (int i = 0; i < 4; i++)
        en[0][i].sprite.setPosition(390 + i * 50, i * 50 + 30);
    for (int i = 0; i < 4; i++)
        en[1][i].sprite.setPosition(340 - i * 50, i * 50 + 30);

    for (int i = 3, j = 1; i < 6; i++, j++)
        en[0][i].sprite.setPosition(230 + j * 70, 150);

    for (int i = 6, j = 1; i < 9; i++, j++)
        en[0][i].sprite.setPosition(230 + j * 70, 250);

    for (int i = 4, j = 1; i < 8; i++, j++)
        en[2][i].sprite.setPosition(200 + j * 70, 190);

    en[2][0].sprite.setPosition(240, 250);
    en[2][1].sprite.setPosition(290, 300);
    en[2][2].sprite.setPosition(490, 250);
    en[2][3].sprite.setPosition(440, 300);

    en[0][9].sprite.setPosition(540, 190);

    en[1][4].sprite.setPosition(340, 350);
    en[1][5].sprite.setPosition(370, 80);
    en[1][6].sprite.setPosition(370, 300);
    en[1][7].sprite.setPosition(390, 350);
}

void Invader::setHeart(int &alpha, int &beta, int &gamma, Enemy **&en)
{
    alpha = 6;
    beta = 7;
    gamma = 5;
    makeInvaders(alpha, beta, gamma, en);
    this->hasDescended = true;

    en[1][0].sprite.setPosition(210, 50);
    en[1][1].sprite.setPosition(340, 100);
    en[1][2].sprite.setPosition(470, 50);
    en[1][3].sprite.setPosition(160, 100);
    en[1][4].sprite.setPosition(520, 100);
    en[1][5].sprite.setPosition(530, 40);
    en[1][6].sprite.setPosition(160, 40);

    en[0][0].sprite.setPosition(270, 90);
    en[0][1].sprite.setPosition(400, 90);
    en[0][2].sprite.setPosition(190, 160);
    en[0][3].sprite.setPosition(220, 210);
    en[0][4].sprite.setPosition(260, 260);
    en[0][5].sprite.setPosition(295, 295);

    en[2][0].sprite.setPosition(340, 330);
    en[2][1].sprite.setPosition(395, 295);
    en[2][2].sprite.setPosition(440, 260);
    en[2][3].sprite.setPosition(490, 160);
    en[2][4].sprite.setPosition(470, 210);
}

void Invader::move(int alpha, int beta, int gamma, Enemy **&enemy)
{
    static bool outOfPlace = false;
    static int direction = 0;
    int x_offset = 15;

    if (!outOfPlace && !direction)      // starting point, right move
    {
        x_offset *= -1;
        outOfPlace = true;
        direction = 1;
    }
    else if (outOfPlace && direction)       // left move towards mean
    {
        x_offset *= 1;
        outOfPlace = false;
        direction = 1;
    }
    else if (!outOfPlace && direction)      // left move towards extreme
    {
        x_offset *= 1;
        outOfPlace = true;
        direction = 0;
    }
    else if (outOfPlace && !direction)      // right move towards mean
    {
        x_offset *= -1;
        outOfPlace = false;
        direction = 0;
    }

    for (int i = 0; i < alpha; i++)
        enemy[0][i].sprite.move(x_offset, 0);

    for (int i = 0; i < beta; i++)
        enemy[1][i].sprite.move(x_offset, 0);

    for (int i = 0; i < gamma; i++)
        enemy[2][i].sprite.move(x_offset, 0);
}

void Invader::raise_back(int alpha, int beta, int gamma, Enemy **&enemy)
{
    if (!this->hasDescended)
        return;

    static float moved = 0;
    float y_offset = -2.2;

    if (moved < 400)
    {
        for (int i = 0; i < alpha; i++)
            enemy[0][i].sprite.move(0, y_offset);

        for (int i = 0; i < beta; i++)
            enemy[1][i].sprite.move(0, y_offset);

        for (int i = 0; i < gamma; i++)
            enemy[2][i].sprite.move(0, y_offset);
        moved += -y_offset;
    }
    else
    {
        moved = 0;
        this->hasDescended = false;
    }
}

void Invader::descend(int alpha, int beta, int gamma, Enemy **&enemy)
{
    if (this->hasDescended)
        return;

    static float moved = 0;
    float y_offset = 2.2;

    if (moved < 400)
    {
        for (int i = 0; i < alpha; i++)
            enemy[0][i].sprite.move(0, y_offset);

        for (int i = 0; i < beta; i++)
            enemy[1][i].sprite.move(0, y_offset);

        for (int i = 0; i < gamma; i++)
            enemy[2][i].sprite.move(0, y_offset);
        moved += y_offset;
    }
    else
    {
        moved = 0;
        this->hasDescended = true;
    }
}

void Invader::setFilledRectangle(int &alpha, int &beta, int &gamma, Enemy **&en)
{
    alpha = 12;
    beta = 12;
    gamma = 11;
    makeInvaders(alpha, beta, gamma, en);

    this->hasDescended = true;

    for (int i = 0; i < 7; i++)
        en[0][i].sprite.setPosition(i * 110 + 30, 40);

    for (int i = 0; i < 7; i++)
        en[1][i].sprite.setPosition(i * 110 + 30, 310);

    for (int i = 0; i < 3; i++)
        en[2][i].sprite.setPosition(30, i * 65 + 115);

    for (int i = 0, j = 3; j < 6; i++, j++)
        en[2][j].sprite.setPosition(690, i * 65 + 115);

    for (int i = 7, j = 1; i < 12; i++, j++)
        en[0][i].sprite.setPosition(j * 110 + 30, 110);

    for (int i = 7, j = 1; i < 12; i++, j++)
        en[1][i].sprite.setPosition(j * 110 + 30, 175);

    for (int i = 6, j = 1; i < 11; i++, j++)
        en[2][i].sprite.setPosition(j * 110 + 30, 245);
}

void Invader::setFilledTriangle(int &alpha, int &beta, int &gamma, Enemy **&en)
{
    alpha = 17;
    beta = 10;
    gamma = 11;
    makeInvaders(alpha, beta, gamma, en);
    this->hasDescended = true;

    for (int i = 0; i < 7; i++)
        en[0][i].sprite.setPosition(390 + i * 40, i * 50 + 35);

    for (int i = 7, j = 0; i < 13; i++, j++)
        en[0][i].sprite.setPosition(215 + j * 60, 285);

    for (int i = 13, j = 0; i < 17; i++, j++)
        en[0][i].sprite.setPosition(275 + j * 60, 185);

    for (int i = 0; i < 7; i++)
        en[1][i].sprite.setPosition(340 - i * 40, i * 50 + 30);

    for (int i = 7, j = 0; i < 9; i++, j++)
        en[1][i].sprite.setPosition(330 + j * 80, 130);

    en[1][9].sprite.setPosition(370, 80);

    for (int i = 0; i < 6; i++)
        en[2][i].sprite.setPosition(180 + 77 * i, 345);

    for (int i = 6, j = 0; i < 11; i++, j++)
        en[2][i].sprite.setPosition(250 + j * 60, 230);
}

void Invader::fire(Vector<Bomb> &bomb)
{
    int x_offset = 15;
    int y_offset = 30;

    bomb.push(Bomb(this->sprite.getPosition().x + x_offset, this->sprite.getPosition().y + y_offset));
}

#endif