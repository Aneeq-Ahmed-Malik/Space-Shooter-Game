#include <SFML/Graphics.hpp>

#ifndef ADDON_H_
#define ADDON_H_

class AddOn
{
protected:
    int x, y;
    float speed;
    std::string type;

public:
    sf::Sprite sprite;
    sf::Texture tex;

    AddOn(std::string type);

    void randomFall(AddOn *&add);

    void move();

    bool checkBounds();

    std::string getType();

    virtual int getScore() {}
    virtual int getInterval() {}
};

AddOn::AddOn(std::string type)
{
    this->x = rand() % 750 + 10;
    this->y = 0;
    this->speed = 2;
    this->type = type;
}

std::string AddOn::getType()
{
    return this->type;
}

bool AddOn::checkBounds()
{
    if (this->sprite.getPosition().y > 740)
        return true;
    return false;
}

void AddOn::move()
{
    this->sprite.move(0, this->speed);
}


class PowerUp : public AddOn
{
    int interval;

public:
    PowerUp();

    virtual int getInterval();
};

PowerUp::PowerUp() : AddOn("PowerUp")
{
    this->tex.loadFromFile("img/energetic.png");
    this->sprite.setTexture(this->tex);
    this->interval = 5;
    this->sprite.setPosition(this->x, this->y);
}

int PowerUp::getInterval()
{
    return this->interval;
}


class Lives : public AddOn
{
public:
    Lives();
};

Lives::Lives() : AddOn("Lives")
{
    this->tex.loadFromFile("img/love.png");
    this->sprite.setTexture(this->tex);
    this->sprite.setPosition(this->x, this->y);
}


class FireBullet : public AddOn
{
private:
    int interval;

public:
    FireBullet();

    virtual int getInterval();
};

FireBullet::FireBullet() : AddOn("FireBullet")
{
    this->tex.loadFromFile("img/flames.png");
    this->sprite.setTexture(this->tex);
    this->interval = 5;
    this->sprite.setPosition(this->x, this->y);
}

int FireBullet::getInterval()
{
    return this->interval;
}


class Danger : public AddOn
{
    int score;

public:
    Danger();
    
    virtual int getScore();
};

Danger::Danger() : AddOn("Danger")
{
    this->tex.loadFromFile("img/sign.png");
    this->sprite.setTexture(this->tex);
    this->score = 5;
    this->sprite.setPosition(this->x, this->y);
}

int Danger::getScore()
{
    return this->score;
}

void AddOn::randomFall(AddOn *&add)
{
    AddOn *arr[] = {new PowerUp, new Lives, new Danger, new FireBullet};
    int num = rand() % 4;       // selecting a random power up

    add = arr[num];
}

#endif