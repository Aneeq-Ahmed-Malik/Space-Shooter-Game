#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;
using namespace sf;

class Animation
{
    Sprite sprite;
    Texture tex;
    Clock animationClock;

    bool Over;
    int cols;
    int numFrames;
    int currentFrame;
    Time frameTime;             // time for animations to occur
    sf::IntRect textureRect;        

public:
    Animation(string path, int numColumns, int numFrames, Time frameTime, float x = 1, float y = 1);
    void update(float x, float y);
    void draw(RenderWindow &window);
    bool& isOver();
};

Animation::Animation(string path, int numColumns, int numFrames, Time frameTime, float x, float y)
{

    this->tex.loadFromFile(path);
    this->sprite.setTexture(tex);
    this->cols = numColumns;
    this->sprite.setScale(x, y);

    textureRect.height = (tex.getSize().y )  / (numFrames / cols);
    textureRect.width = (tex.getSize().x / cols);
    this->sprite.setTextureRect(textureRect);

    this->numFrames = numFrames;
    this->frameTime = frameTime;
    this->Over = false;
    this->currentFrame = 0;
}

void Animation::update(float x, float y)
{
    if (!Over)          // if the animation is not over
    {
        this->sprite.setPosition(x, y);
        if (animationClock.getElapsedTime() >= frameTime)       //      if time for a specific frame is reached
        {
            currentFrame++;                 
            if (currentFrame >= numFrames)      // if total frames reached
            {
                currentFrame = 0;
                Over = true;
            }
            animationClock.restart();       
            if(Over)
                return;
        }

        textureRect.left = (currentFrame % cols) * textureRect.width;
        textureRect.top = (currentFrame / cols) * textureRect.height;

        sprite.setTextureRect(textureRect);
    }

}

void Animation::draw(sf::RenderWindow &window)
{
    window.draw(this->sprite);
}

bool& Animation::isOver()
{
    return this->Over;
}