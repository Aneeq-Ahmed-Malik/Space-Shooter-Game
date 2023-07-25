#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <iostream>
#include <math.h>

#include "Animation.h"
#include "player.h"
#include "bullet.h"
#include "Vector.h"
#include "enemy.h"
#include "bomb.h"
#include "laser.h"
#include "addOn.h"
#include "menu.h"

using namespace sf;
using namespace std;

const char title[] = "Space Shooter";

class Game
{
private:
    Sprite background, animation[2];
    Texture bg_texture, anime;
    RenderWindow *clone;
    Animation *explosion, *anime_explosion;
    Player *p;
    Vector<Bullet> b;
    Vector<Bomb> bomb;
    AddOn *add;
    Enemy **enemy, *en = new Alpha;
    int level;
    bool new_level = false;
    Menu *m;
    int alpha_count = 0, beta_count = 0, gamma_count = 0;
    Music music;
    SoundBuffer buffer[2];
    Sound sound[2];

public:
    Game();

    void spawnAddOn(bool &addon, float &addon_timer, float &addon_limit);
    void spawnMonster(bool &monster, float &monster_timer, float &monster_limit, bool &dodge_monster);
    void spawnDragon(bool &dragon, float &dragon_timer, float &dragon_limit, bool &dodge_dragon);

    bool checkFire(int point);
    double CalcArea(double x1, double y1, double x2, double y2, double x3, double y3);
    void checkLaser(bool monster, bool &dodge_monster);

    void levelUp();

    void remove(Enemy *&enemy, int &size, int index);
    void checkCollision(bool dragon, bool monster);
    void checkPlayerCollision(bool dragon, bool monster);

    void checkCooldown();
    void checkBomb();
    void fireBomb(float &bomb_timer, float &bomb_limit, bool monster, bool dragon);
    bool collidingAnimation();

    bool hasCollided(FloatRect rect1, FloatRect rect2);
    bool start_game();

    ~Game();
};

Game::Game()
{
    p = new Player("img/player_ship.png"); // making a new player

    bg_texture.loadFromFile("img/bg(6).jpg"); // loading the background texture
    bg_texture.setRepeated(true);

    anime.loadFromFile("img/ufo.png"); //  texture for animation

    //--------------------animation sprites------------------------//

    animation[0].setTexture(anime);
    animation[1].setTexture(anime);
    animation[0].setScale(0.6, 0.6);
    animation[1].setScale(0.6, 0.6);
    animation[0].setPosition(-300, 300);
    animation[1].setPosition(760, 300);

    //-------------------------------------------------------------//

    level = 0; // starting level

    m = new Menu; // making a new menu

    anime_explosion = new Animation("img/bomb.png", 6, 12, seconds(0.2), 1.2, 1.2); //  starting animation explosion

    explosion = new Animation("img/explosion.png", 4, 4, seconds(0.2), 0.4, 0.4); // enemy destroy animation

    background.setTexture(bg_texture); // background sprite
    background.setScale(1, 1.5);

    //-------------------- All sounds & music used throughout the game -------------------//
    music.openFromFile("Sound/game_sound.ogg");
    music.setLoop(true);
    music.setVolume(30);
    music.play();

    buffer[0].loadFromFile("Sound/bomb_sound.wav");
    sound[0].setBuffer(buffer[0]);

    buffer[1].loadFromFile("Sound/blast.wav");
    sound[1].setBuffer(buffer[1]);
}

void Game::spawnAddOn(bool &addon, float &addon_timer, float &addon_limit)
{
    if (!addon) // if there is to be no addon
        return;

    add->move(); // move the addon

    if (p->collectAddon(add)) // if player has collected the addon
    {
        addon_timer = 0;
        addon_limit += 2.0; // increasing time for next addon
        addon = false;

        int i;
        string str[] = {"FireBullet", "PowerUp", "Danger", "Lives"}; // all available addons
        for (i = 0; i < 4; i++)
            if (add->getType() == str[i])
                break;

        delete add; // delete the collected addon
        add = nullptr;

        void (Player::*func[4])() = {&Player::triggerFire, &Player::triggerPowerUp, &Player::triggerDanger, &Player::triggerLives};

        (p->*func[i])(); // run the specific function of player acc to the addon collected

        return;
    }

    if (add->checkBounds()) // if addon is not collected
    {
        if (add->getType() == "Danger")
            *p += add->getScore(); // if danger is dodged
        addon_timer = 0;
        addon_limit += 2.0; // increasing time for next addon
        addon = false;
        delete add; // deleting the current addon
        add = nullptr;
    }
}

bool Game::checkFire(int point)
{
    if (!enemy[4]->getFire().isOn())    // if fire is not on return
        return false;

    float px = p->sprite.getPosition().x;
    float py = p->sprite.getPosition().y;

    switch (point)      // check for top, right & left of space ship
    {
    case 1: // topmost of space ship
        switch (p->getRotation())       // check for rotated space ship
        {
        case 0:             // if space ship is not rotated
            px += 38; 
            break;
        case 1:             //  if space ship is right rotated
            px += 90;
            py += 30;
            break;
        case 2:             // if space ship is left rotated
            px += 30;
            py += 30;
            break;
        }
        break;
    case 2:             // check for left of the space ship
        switch (p->getRotation())           // check for rotated space ship
        {
        case 0:         // if space ship is not rotated
            py += 80; 
            break;
        case 1:         //  if space ship is right rotated
            py += 60;
            break;
        case 2:         // if space ship is left rotated
            px += 60;
            py += 105;
            break;
        }
        break;
    case 3:              // check for right of the space ship
        switch (p->getRotation())    // check for rotated space ship
        {
        case 0:             // if space ship is not rotated
            px += 80;
            py += 75;
            break;
        case 1:             //  if space ship is right rotated
            px += 60;
            py += 105;
            break;
        case 2:             // if space ship is left rotated
            px += 105;
            py += 60;
            break;
        }
        break;
    }

    float dx = 380, dy = 365;
    float d1x, d1y, d2x, d2y;

    switch (enemy[4]->getFire().rotated())      // check for rotated fire
    {
    case 0:         // if not rotated
        d1x = 430;
        d1y = 390;
        d2x = 355;
        d2y = 700;
        break;
    case 2:         // if left rotated
        d1x = 255;
        d1y = 650;
        d2x = 190;
        d2y = 560;
        break;
    case 1:         // if right rotated
        d1x = 505;
        d1y = 650;
        d2x = 580;
        d2y = 600;
        break;
    }

    double A = CalcArea(dx, dy, d1x, d1y, d2x, d2y);
    double A1 = CalcArea(dx, dy, d1x, d1y, px, py);
    double A2 = CalcArea(dx, dy, px, py, d2x, d2y);
    double A3 = CalcArea(px, py, d1x, d1y, d2x, d2y);

    if (abs((A1 + A2 + A3) - A) < 22)       // less than 22 for exceptions
        return true;

    return false;
}

double Game::CalcArea(double x1, double y1, double x2, double y2, double x3, double y3)
{
    //-----------------------Calculating Area using heros formula---------------------//

    double a = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
    double b = sqrt(pow(x3 - x2, 2) + pow(y3 - y2, 2));
    double c = sqrt(pow(x1 - x3, 2) + pow(y1 - y3, 2));
    double s = (a + b + c) / 2;
    double area = sqrt(s * (s - a) * (s - b) * (s - c));
    return area;
}

void Game::checkLaser(bool monster, bool &dodge_monster)
{
    if (monster && enemy[3]->getLaser().isOn()) // if the monster is on screen and laser is on
    {
        if (hasCollided(enemy[3]->sprite.getGlobalBounds(), p->sprite.getGlobalBounds()))
        {
            p->restart();          // if player is hit by the laser
            dodge_monster = false; // no score for dodging the monster
        }
    }
}

void Game::levelUp()
{
    if (alpha_count == 0 && beta_count == 0 && gamma_count == 0) // if all invaders are destroyed
    {
        // calling the specific formation for specific levels & waves //
        void (Enemy::*func[9])(int &, int &, int &, Enemy **&) = {&Enemy::setRectangle, &Enemy::setTriangle, &Enemy::setCross, &Enemy::setCircle, &Enemy::setDiamod, &Enemy::setHeart, &Enemy::setFilledDiamod, &Enemy::setFilledRectangle, &Enemy::setFilledTriangle};

        (en->*func[level - 1])(alpha_count, beta_count, gamma_count, enemy);
    }
}

void Game::remove(Enemy *&enemy, int &size, int index)
{
    Enemy *temp = enemy->getPtr(size - 1); //  make a temp array of corresponding passed invader of one less size
    int count = 0;

    for (int i = 0; i < size; i++)
    {
        if (i == index) // skip the invader that is to be removed
            continue;
        temp[count++] = enemy[i];
    }

    *p += enemy[index].getScore() * (level / 3 + 1); // increment the score of destroying the invader

    delete[] enemy; // delete previous array of specific invaders
    enemy = temp;   // point to new modified array of invaders
    size--;
}

void Game::checkCollision(bool dragon, bool monster)
{
    static bool doExplosion = false;
    static float x_cor = 20;
    static float y_cor = 20;

    if (doExplosion)
    {
        if (!explosion->isOver())
        {
            explosion->update(x_cor, y_cor);
            explosion->draw(*clone);
        }

        else
            doExplosion = false;
    }

    if (monster)
    {
        for (int j = 0; j < b.current; j++)
        {
            if (hasCollided(enemy[3]->sprite.getGlobalBounds(), b[j].sprite.getGlobalBounds()))
            {
                b.remove(j);
                sound[0].play();
                break;
            }
        }
    }

    if (dragon)
    {
        for (int j = 0; j < b.current; j++)
        {
            if (hasCollided(enemy[4]->sprite.getGlobalBounds(), b[j].sprite.getGlobalBounds()))
            {
                b.remove(j);
                sound[0].play();
                break;
            }
        }
    }

    bool flag = false;
    for (int i = 0; i < alpha_count; i++)
    {
        for (int j = 0; j < b.current; j++)
        {

            if (hasCollided(enemy[0][i].sprite.getGlobalBounds(), b[j].sprite.getGlobalBounds()))
            {
                doExplosion = true;
                x_cor = enemy[0][i].sprite.getPosition().x;
                y_cor = enemy[0][i].sprite.getPosition().y;
                explosion->isOver() = false;

                remove(enemy[0], alpha_count, i);

                if (!b[j].onFire())
                    b.remove(j);
                sound[0].play();
                break;
            }
        }
    }

    for (int i = 0; i < beta_count; i++)
    {
        for (int j = 0; j < b.current; j++)
        {
            int x = b[j].sprite.getPosition().x;
            int y = b[j].sprite.getPosition().y;

            if (hasCollided(enemy[1][i].sprite.getGlobalBounds(), b[j].sprite.getGlobalBounds()))
            {

                doExplosion = true;
                x_cor = enemy[1][i].sprite.getPosition().x;
                y_cor = enemy[1][i].sprite.getPosition().y;
                explosion->isOver() = false;

                remove(enemy[1], beta_count, i);

                if (!b[j].onFire())
                    b.remove(j);
                sound[0].play();
                break;
            }
        }
    }

    for (int i = 0; i < gamma_count; i++)
    {
        for (int j = 0; j < b.current; j++)
        {

            if (hasCollided(enemy[2][i].sprite.getGlobalBounds(), b[j].sprite.getGlobalBounds()))
            {

                doExplosion = true;
                x_cor = enemy[2][i].sprite.getPosition().x;
                y_cor = enemy[2][i].sprite.getPosition().y;
                explosion->isOver() = false;

                remove(enemy[2], gamma_count, i);

                if (!b[j].onFire())
                    b.remove(j);
                sound[0].play();
                break;
            }
        }
    }
}

void Game::checkPlayerCollision(bool dragon, bool monster)
{
    if (dragon)
        if (hasCollided(enemy[4]->sprite.getGlobalBounds(), p->sprite.getGlobalBounds()))
            p->restart();

    if (monster)
        if (hasCollided(enemy[3]->sprite.getGlobalBounds(), p->sprite.getGlobalBounds()))
            p->restart();

    for (int i = 0; i < alpha_count; i++)
        if (hasCollided(enemy[0][i].sprite.getGlobalBounds(), p->sprite.getGlobalBounds()))
        {
            remove(enemy[0], alpha_count, i);
            p->restart();
        }

    for (int i = 0; i < beta_count; i++)
        if (hasCollided(enemy[1][i].sprite.getGlobalBounds(), p->sprite.getGlobalBounds()))
        {
            remove(enemy[1], beta_count, i);
            p->restart();
        }
    for (int i = 0; i < gamma_count; i++)
        if (hasCollided(enemy[2][i].sprite.getGlobalBounds(), p->sprite.getGlobalBounds()))
        {
            remove(enemy[2], gamma_count, i);
            p->restart();
        }
}

void Game::checkCooldown()
{
    //------------------- used to check if a specific invader is cooled to throw another bomb-------------//

    for (int i = 0; i < alpha_count; i++)
        if (enemy[0][i].isCooled() == false && (int)enemy[0][i].timer.getElapsedTime().asSeconds() > enemy[0][i].getInterval())
            enemy[0][i].isCooled() = true;

    for (int i = 0; i < beta_count; i++)
        if (enemy[1][i].isCooled() == false && (int)enemy[1][i].timer.getElapsedTime().asSeconds() > enemy[1][i].getInterval())
            enemy[1][i].isCooled() = true;

    for (int i = 0; i < gamma_count; i++)
        if (enemy[2][i].isCooled() == false && (int)enemy[2][i].timer.getElapsedTime().asSeconds() > enemy[2][i].getInterval())
            enemy[2][i].isCooled() = true;
}

void Game::checkBomb()
{
    bool check;

    for (int i = 0; i < bomb.current; i++)
    {
        if (p->isShieldOn())
            check = hasCollided(bomb[i].sprite.getGlobalBounds(), p->shield_sprite.getGlobalBounds());
        else
            check = hasCollided(bomb[i].sprite.getGlobalBounds(), p->sprite.getGlobalBounds());

        if (check)
        {
            p->restart();
            bomb.remove(i);
        }
    }
}

void Game::spawnMonster(bool &monster, float &monster_timer, float &monster_limit, bool &dodge_monster)
{
    if (monster) // if the monster flag is true
    {
        if (monster_timer < 22) // monster on screen for 22 seconds
        {
            // after every 2 seconds if monster has descended trigger the laser

            if (enemy[3]->timer.getElapsedTime().asSeconds() > 2 && enemy[3]->has_descended())
            {
                enemy[3]->getLaser().isOn() = !enemy[3]->getLaser().isOn();
                enemy[3]->timer.restart(); // restart the timer
            }

            clone->draw(enemy[3]->sprite); // draw the monster

            // if mosnter has descended and its laser is on, draw the laser
            if (enemy[3]->has_descended() && enemy[3]->getLaser().isOn())
                clone->draw(enemy[3]->getLaser().sprite);

            enemy[3]->descend(); //  once descended the has_descended() return true

            en->raise_back(alpha_count, beta_count, gamma_count, enemy); // raise all invader above

            enemy[3]->move(); // move the monster
        }
        else // if 22 secs have passed
        {
            enemy[3]->getLaser().isOn() = false; // turn off the laser
            enemy[3]->raise_back();              // make the monster raise back

            en->descend(alpha_count, beta_count, gamma_count, enemy); // make the invaders come back

            clone->draw(enemy[3]->sprite); // draw the monster

            if (!enemy[3]->has_descended()) // if monster is completely raised back
            {
                monster = false;
                if (dodge_monster) // if player has dodged the monster
                    *p += enemy[3]->getScore();
                dodge_monster = true;
                monster_timer = 0;
                monster_limit += 3.0; // increasing time for next monster appearance
            }
        }
    }
}

void Game::spawnDragon(bool &dragon, float &dragon_timer, float &dragon_limit, bool &dodge_dragon)
{
    if (dragon)     // if dragon flag is true
    {
        if (dragon_timer < 30)      // dragon on screen for 30 seconds
        {

            // after every 1.5 seconds if dragon has descended trigger the laser

            if (enemy[4]->timer.getElapsedTime().asSeconds() > 1.5 && enemy[4]->has_descended())
            {
                enemy[4]->getFire().isOn() = !enemy[4]->getFire().isOn();
                enemy[4]->timer.restart();
                enemy[4]->getFire().rotate(p->getRegion());
            }
            clone->draw(enemy[4]->sprite);

            // if dragon has descended and its fire is on, draw the fire
            if (enemy[4]->has_descended() && enemy[4]->getFire().isOn())
                clone->draw(enemy[4]->getFire().sprite);

            enemy[4]->descend(); //  once descended the has_descended() return true

            en->raise_back(alpha_count, beta_count, gamma_count, enemy); // raise all invader above

            enemy[4]->fire(); // unleash the fire
        }
        else // if 30 secs have passed
        {
            enemy[4]->getFire().isOn() = false;     // turn off the fire

            enemy[4]->raise_back();             // raise the dragon back

            en->descend(alpha_count, beta_count, gamma_count, enemy);   // make the invader come back
            clone->draw(enemy[4]->sprite);

            if (!enemy[4]->has_descended())     // if dragon is gone
            {
                dragon = false;
                if (dodge_dragon)
                    *p += enemy[4]->getScore();     // increment score
                dodge_dragon = true;
                dragon_timer = 0;
                dragon_limit += 3.0; // increasing time for next dragon appearance
            }
        }
    }
}

void Game::fireBomb(float &bomb_timer, float &bomb_limit, bool monster, bool dragon)
{
    if (bomb_timer > bomb_limit && !monster && !dragon)
    {
        int invader = rand() % 3, ship = -1;        // random invader to throw the bomb
        int num = invader == 0 ? alpha_count : invader == 1 ? beta_count
                                                            : gamma_count;

        if (num)        
        {
            ship = rand() % num;
            if (enemy[invader][ship].isCooled())    // if that invader is not on cool down
            {
                enemy[invader][ship].fire(bomb);
                enemy[invader][ship].isCooled() = false;
                enemy[invader][ship].timer.restart();
            }
        }
        bomb_timer = 0;
        en->move(alpha_count, beta_count, gamma_count, enemy);  // move the invaders
    }
}

bool Game::start_game()
{
    srand(time(0));

    m->display_menu();          // displaying the main menu

    RenderWindow window(VideoMode(780, 780), title);
    clone = &window;                    // pointer to window, for accessing in different functions
    window.setVerticalSyncEnabled(true);

    Text text;
    Clock clock, clock1;
    bool refresh = true, monster = false, dragon = false, addon = false, reach = false, dodge_monster = 1, dodge_dragon = 1;
    bool doAnimation = false;

    float timer = 0, bulletTimer = 0, bomb_timer = 0, monster_timer = 0, monster_limit = 20, level_limit = 2, bomb_limit = 1.2;
    float dragon_timer = 0, dragon_limit = 35, addon_timer = 0, addon_limit = 7, bullet_limit = 1.2, level_timer = 0;

    sf::Shader parallaxShader;          // Shader for background movement
    parallaxShader.loadFromMemory(
        "uniform float offset;"

        "void main() {"
        "    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;"
        "    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;"
        "    gl_TexCoord[0].y = gl_TexCoord[0].y - offset;"
        "    gl_FrontColor = gl_Color;"
        "}",
        sf::Shader::Vertex);

    float offset = 0.0001;          // shader offset

    while (window.isOpen())
    {

        float time = clock.getElapsedTime().asSeconds();

        clock.restart();
        timer += time;
        bulletTimer += time;
        bomb_timer += time;
        monster_timer += time;
        dragon_timer += time;
        addon_timer += time;
        level_timer += time;

        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();
            if (Keyboard::isKeyPressed(Keyboard::P))
            {
                window.setActive(0);
                window.setVisible(0);
                m->writeScore(p->getScore());       // write score in file
                if (m->pauseScreen())
                    return 1;
                window.setActive(1);
                window.setVisible(1);
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Left))
            p->move("l");
        if (Keyboard::isKeyPressed(Keyboard::Right))
            p->move("r");
        if (Keyboard::isKeyPressed(Keyboard::Up))
            p->move("u");
        if (Keyboard::isKeyPressed(Keyboard::Down))
            p->move("d");

        if (p->getAddOn() != 2 && Keyboard::isKeyPressed(Keyboard::Right) && Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::Left) && Keyboard::isKeyPressed(Keyboard::Down) && p->getAddOn() != 2)
            p->rotate(1);
        else if (p->getAddOn() != 2 && Keyboard::isKeyPressed(Keyboard::Left) && Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::Down) && Keyboard::isKeyPressed(Keyboard::Right) && p->getAddOn() != 2)
            p->rotate(2);
        else
            p->reset();

        //-------------Bullet Control------------//
        if (bulletTimer > bullet_limit && reach && !doAnimation && enemy)
        {
            p->fire(b, bullet_limit);
            bulletTimer = 0;
        }

        //-----------Random Monster Control-----------//
        if (monster_timer > monster_limit && !monster && !dragon)
        {
            int num = rand() % 10000;

            if (num == 10)
            {
                monster_timer = 0;
                monster = true;
            }
        }

        //--------------Random Dragon Control----------------//
        if (dragon_timer > dragon_limit && !monster && !dragon)
        {
            int num = rand() % 10000;

            if (num == 10)
            {
                dragon_timer = 0;
                dragon = true;
                enemy[4]->getFire().isOn() = false;
            }
        }

        //---------------Random Addon Control------------------//
        if (addon_timer > addon_limit && !addon && reach && !doAnimation)
        {
            int num = rand() % 10;
            if (num == 5)
            {
                add->randomFall(add);
                addon_timer = 0;
                addon = true;
            }
        }

        //----------New Wave or Level Control-----------//
        if (alpha_count == 0 && beta_count == 0 && gamma_count == 0 && !new_level)
        {
            new_level = true;
            level_timer = 0;
            reach = false;
            level++;
        }

        //-----------Create Invader Pattern Acc to level----------//
        if (new_level && level_timer > level_limit)
        {
            levelUp();
            new_level = false;
            level_limit = 3;
        }

        checkCooldown();        // to check the cooldown of invaders

        checkBomb();                // check if bomb has hit the space ship

        checkLaser(monster, dodge_monster);     // check if laser has hit the space ship

        if (p->isShieldOn())        //      if shield is on then display the shiled
            p->enableShield();

        if (dragon)                 // if dragon is on 
            if (checkFire(1) || checkFire(2) || checkFire(3))       // check if fire has hit the palyer
            {
                p->restart();
                dodge_dragon = false;           //   No score for dodging the dragon
            }

        fireBomb(bomb_timer, bomb_limit, monster, dragon);      // bomb firing from invaders

        parallaxShader.setUniform("offset", offset += clock1.restart().asSeconds() / 100 + 0.0008);

        window.clear(Color::Black);
        window.draw(background, &parallaxShader);
        window.draw(p->sprite);
        window.draw(animation[0]);
        window.draw(animation[1]);

        checkCollision(dragon, monster);             // bullet collision with enemies
        checkPlayerCollision(dragon, monster);          // player space ship collision with enemies

        spawnMonster(monster, monster_timer, monster_limit, dodge_monster);   
        spawnDragon(dragon, dragon_timer, dragon_limit, dodge_dragon);
        spawnAddOn(addon, addon_timer, addon_limit);

        m->displayLives(p->getLives(), window);
        m->displayScore(p->getScore(), window);

        if (reach)         // if Level is displayed on screen then display wave number
            m->displayWave(this->level % 3, window);


        if (level % 3 == 1 && !reach)           // if it is a new level
        {
            if (m->newLevel(level / 3 + 1, window) && level_timer > 4)      // make new level animation
                if (m->goBack())           
                {
                    //-----------------Increasing speed---------------//

                    bullet_limit -= 0.2;
                    bomb_limit -= 0.15;
                    addon_limit -= 2;
                    p->changeSpeed() += 0.6;
                    reach = true;
                    doAnimation = true;
                }

            level_limit = 12;
        }
        else
            reach = true;

        //--------------Starting Animation---------------//
        if (doAnimation && level % 3 == 1)
            if (collidingAnimation())
                doAnimation = false;

        //-------------------Drawing everything on screen-------------//
        for (int i = 0; i < alpha_count; i++)
            window.draw(enemy[0][i].sprite);

        for (int i = 0; i < beta_count; i++)
            window.draw(enemy[1][i].sprite);

        for (int i = 0; i < gamma_count; i++)
            window.draw(enemy[2][i].sprite);

        for (int i = 0; i < b.current; i++)
        {
            window.draw(b[i].sprite);
            b[i].move();
            if (b[i].checkBounds())
                b.remove(i);
        }

        for (int i = 0; i < bomb.current; i++)
        {
            window.draw(bomb[i].sprite);
            bomb[i].move();
            if (bomb[i].checkBounds())
                bomb.remove(i);
        }

        if (addon)
            window.draw(add->sprite);

        if (p->isShieldOn())
            window.draw(p->shield_sprite);

        if (p->getLives() == 0)
        {
            window.setVisible(0);
            if (m->gameOver(p->getScore(), "Game Over"))
                return 1;
        }

        if (level == 10)
        {
            window.setVisible(0);
            if (m->gameOver(p->getScore(), "Game End!"))
                return 1;
        }

        window.display();
    }
    return 0;
}

bool Game::collidingAnimation()
{

    static bool expl = false;
    if (expl)                   // if both have collided
    {
        sound[1].play();
        anime_explosion->update(320, 350);
        anime_explosion->draw(*clone);

        if (anime_explosion->isOver())      // if explosion is over
        {
            anime_explosion->isOver() = false;
            expl = false;
            return 1;
        }
    }

    float x_offset = 3.5;

    if (animation[0].getPosition().x < 220 && !expl)        // if both have reached mid of screen
    {
        animation[0].move(x_offset, 0);
        animation[1].move(-x_offset, 0);
    }
    else
    {
        anime_explosion->isOver() = false;
        animation[0].setPosition(-300, 300);
        animation[1].setPosition(770, 300);
        expl = true;
    }

    return false;
}

Game::~Game()
{
    if (enemy)
    {
        for (int i = 0; i < 3; i++)
            delete[] enemy[i];
        delete[] enemy;
    }
    delete p;
    delete en;
    delete add;
    delete m;
}

bool Game::hasCollided(FloatRect rect1, FloatRect rect2)
{
    bool xOverlap = rect1.left < rect2.left + rect2.width && rect1.left + rect1.width > rect2.left;
    bool yOverlap = rect1.top < rect2.top + rect2.height && rect1.top + rect1.height > rect2.top;

    return xOverlap && yOverlap;
}
