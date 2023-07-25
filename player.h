#include <SFML/Graphics.hpp>
#include <string.h>
#include <iostream>
#include "Vector.h"
#include "bullet.h"
#include "addOn.h"

using namespace sf;
class Player
{
private:

	float speed = 2.6;
	int x, y, score;
	int rotation, addon;
	int lives, addon_interval, shield_inteval;
	bool shield;

public:
	Texture tex, shield_tex;
	Sprite sprite, shield_sprite;

	Clock timer, shield_timer;

	Player(std::string png_path);

	void fire(Vector<Bullet> &b, float &bullet_limit);
	void bulletOnFire(Vector<Bullet> &b, float &bullet_limit);
	void sevenBullet(Vector<Bullet> &b, float &bullet_limit);

	void rotate(int direction);
	void move(std::string s);
	void wrap_around();

	void restart();
	void reset();
	void enableShield();
	bool isShieldOn();

	int getRegion();
	int getLives();
	int getRotation();
	int getScore();
	int getAddOn();
	float& changeSpeed();
	void operator+=(int score);

	bool collectAddon(AddOn *&add);
	void triggerFire();
	void triggerPowerUp();
	void triggerLives();
	void triggerDanger();
};

Player::Player(std::string png_path)
{
	tex.loadFromFile(png_path);
	sprite.setTexture(tex);
	this->shield_tex.loadFromFile("img/circle.png");
	x = 340;
	y = 600;
	sprite.setPosition(x, y);
	sprite.setScale(0.75, 0.75);
	this->rotation = 0;
	this->addon = 0;
	this->lives = 3;
	this->shield_inteval = 3;
	this->score = 0;
}

void Player::enableShield()
{
	if (shield_timer.getElapsedTime().asSeconds() > this->shield_inteval)
		this->shield = false;
	this->shield_sprite.setTexture(this->shield_tex);
	this->shield_sprite.setPosition(this->sprite.getPosition().x - 25, this->sprite.getPosition().y - 20);
}

void Player::fire(Vector<Bullet> &b, float &bullet_limit)
{
	if (addon)		// if addon has been collected
	{
		void (Player::*func[2])(Vector<Bullet> &, float &) = {&Player::bulletOnFire, &Player::sevenBullet};
		(this->*func[addon - 1])(b, bullet_limit);		// call corresponding fucntion
		return;
	}

	int x_offset = 30;
	int y_offset = 30;
	int deg;
	string img[] = {"img/bullet_45.png", "img/bullet_-45.png", "img/bullet1.png"}, path;
	string deg_str[] = {"45", "135", "90"};		// degrees acc to images

	if (!rotation)
	{
		path = img[2];
		deg = stoi(deg_str[2]);
		x_offset -= 20;		// offset for position perfection
	}
	else
	{
		path = img[this->rotation - 1];
		deg = stoi(deg_str[this->rotation - 1]);
		if (this->rotation == 1)
			x_offset += 20;			// offset for position perfection
	}

	b.push(Bullet(this->sprite.getPosition().x + x_offset * 0.707, this->sprite.getPosition().y + y_offset * 0.707, rotation, deg, path));
}

void Player::rotate(int direction)
{
	this->rotation = direction;
	direction--;
	Sprite temp;
	string img[] = {"img/rotation_45.png", "img/rotation_-45.png"};
	tex.loadFromFile(img[direction]);
	temp.setTexture(tex);
	temp.setScale(0.75, 0.75);
	temp.setPosition(sprite.getPosition().x, sprite.getPosition().y);
	sprite = temp;
}

void Player::reset()
{
	this->rotation = 0;
	Sprite temp;
	tex.loadFromFile("img/player_ship.png");
	temp.setTexture(tex);
	temp.setScale(0.75, 0.75);
	temp.setPosition(sprite.getPosition().x, sprite.getPosition().y);
	sprite = temp;
}

void Player::move(std::string s)
{
	float delta_x = 0, delta_y = 0;

	if (s == "u")
		delta_y = -1;
	else if (s == "d")
		delta_y = 1;
	else if (s == "l")
		delta_x = -1;
	else if (s == "r")
		delta_x = 1;

	delta_x *= speed;
	delta_y *= speed;

	sprite.move(delta_x, delta_y);
	this->wrap_around();
}

void Player::wrap_around()
{
	int x = this->sprite.getPosition().x, y = this->sprite.getPosition().y;
	if (y > 650)
		this->sprite.setPosition(x, 5);
	if (y < 0)
		this->sprite.setPosition(x, 650);

	if (x < 0)
		this->sprite.setPosition(740, y);
	if (x > 740)
		this->sprite.setPosition(5, y);
}

void Player::restart()
{
	if (this->shield)		// if sheild is on dont restart
		return;

	x = 340;
	y = 600;
	sprite.setPosition(x, y);
	this->rotation = 0;
	lives--;
	this->shield = true;
	this->shield_timer.restart();
}

int Player::getRegion()
{
	if (this->sprite.getPosition().x < 260)
		return 2;
	if (this->sprite.getPosition().x > 520)
		return 1;
	return 0;
}

bool Player::collectAddon(AddOn *&add)
{
	if (add->sprite.getGlobalBounds().intersects(this->sprite.getGlobalBounds()))
	{
		this->addon_interval = add->getInterval();
		return 1;
	}

	return 0;
}

void Player::triggerFire()
{
	this->addon = 1;
	timer.restart();
}

void Player::triggerPowerUp()
{
	this->addon = 2;
	this->shield = true;
	this->shield_timer.restart();
	this->shield_inteval = this->addon_interval;
	timer.restart();
}

void Player::triggerLives()
{
	if (this->lives < 3)
		this->lives++;
}

void Player::triggerDanger()
{
	this->restart();
}

void Player::bulletOnFire(Vector<Bullet> &b, float &bullet_limit)
{
	if (timer.getElapsedTime().asSeconds() > this->addon_interval)
	{
		this->addon = 0;
		bullet_limit = 1;
		this->addon_interval = 0;
		return;
	}
	int deg;
	int x_offset = 30;
	int y_offset = 30;
	bullet_limit = 0.6;
	string img[] = {"img/fireBullet_45.png", "img/fireBullet_-45.png", "img/fireBullet.png"}, path;
	string deg_str[] = {"45", "135", "90"};

	if (!rotation)
	{
		path = img[2];
		deg = stoi(deg_str[2]);
		x_offset -= 20;
	}
	else
	{
		path = img[this->rotation - 1];
		deg = stoi(deg_str[this->rotation - 1]);
		if (this->rotation == 1)
			x_offset += 20;
	}

	b.push(Bullet(this->sprite.getPosition().x + x_offset * 0.707, this->sprite.getPosition().y + y_offset * 0.707, rotation, deg, path, true));
}

void Player::sevenBullet(Vector<Bullet> &b, float &bullet_limit)
{
	if (timer.getElapsedTime().asSeconds() > this->addon_interval)
	{
		this->addon = 0;
		bullet_limit = 1;
		this->addon_interval = 0;
		this->shield = false;
		this->shield_inteval = 3;
		return;
	}
	bullet_limit = 0.7;
	string img[] = {"img/7bullet0.png", "img/7bullet15.png", "img/7bullet30.png", "img/7bullet45.png", "img/7bullet-15.png", "img/7bullet-30.png", "img/7bullet-45.png"};
	string deg_str[] = {"90", "75", "60", "45", "105", "120", "135"};

	for (int i = 0; i < 7; i++)
		b.push(Bullet(this->sprite.getPosition().x + 20 * 0.707, this->sprite.getPosition().y + 30 * 0.707, this->rotation, stoi(deg_str[i]), img[i]));
}

void Player::operator+=(int score)
{
	this->score += score;
}

int Player::getLives()
{
	return this->lives;
}

bool Player::isShieldOn()
{
	return this->shield;
}

int Player::getRotation()
{
	return this->rotation;
}

int Player::getScore()
{
	return this->score;
}

int Player::getAddOn()
{
	return this->addon;
}

float& Player::changeSpeed()
{
	return this->speed;
}
