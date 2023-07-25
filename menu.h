#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "highscore.h"
using namespace sf;

class Menu
{
private:
    Font font, menu, font1;
    Text text, options[5], back;
    Texture background, badge[3];
    Sprite bg, medals[3];
    HighScore file;
    Clock clock;
    std::string name;
    RenderWindow *clone;

public:
    Menu();

    void display_menu();
    void startGame();
    void exit();
    void instructions();
    void leaderboard();

    bool mouseClicked(Text &text, float x, float y);
    void checkMouse(Text &text, float x, float y);

    void displayLives(int lives, RenderWindow &window);
    void displayScore(int newScore, RenderWindow &window);
    void displayWave(int wave, RenderWindow &window);

    bool newLevel(int level, RenderWindow &window);
    bool descend();
    bool goBack();

    bool TakeName();
    bool pauseScreen();
    bool gameOver(int score, string disp);
    void writeScore(int score);

};

Menu::Menu()
{
    font.loadFromFile("font/MachineGunk-nyqg.ttf");
    menu.loadFromFile("font/valorax.otf");
    font1.loadFromFile("font/Roboto-Bold.ttf");
    text.setFont(font);
    text.setCharacterSize(50);
    text.setString("Level - 0");
    text.setFillColor(Color::Yellow);
    text.setPosition(-150, 300);

    badge[0].loadFromFile("img/first.png");
    badge[1].loadFromFile("img/second.png");
    badge[2].loadFromFile("img/third.png");

    for (int i = 0; i < 3; i++)
        medals[i].setTexture(badge[i]);

    background.loadFromFile("img/space_background.jpg");
    bg.setTexture(background);
    bg.setColor(Color(45, 45, 60, 200));
    this->name = "";

    back.setString("Back");
    back.setFont(menu);
    back.setCharacterSize(30);
}

void Menu::display_menu()
{
    RenderWindow window(VideoMode(800, 600), "Space Invaders");
    clone = &window;
    for (int i = 0; i < 5; i++)
        options[i].setFont(menu);

    options[0].setString("Space Invaders");
    options[0].setCharacterSize(60);
    options[0].setPosition(100, 100);
    options[0].setFillColor(Color(105, 99, 255));

    options[1].setString("Start Game");
    options[1].setCharacterSize(30);
    options[1].setPosition(300, 200);
    options[1].setFillColor(Color(255, 255, 255));

    options[2].setString("Leaderboard");
    options[2].setCharacterSize(30);
    options[2].setPosition(285, 270);
    options[2].setFillColor(Color(100, 100, 100));

    options[3].setString("Instructions");
    options[3].setCharacterSize(30);
    options[3].setPosition(295, 340);
    options[3].setFillColor(Color(100, 100, 100));

    options[4].setString("Exit");
    options[4].setCharacterSize(30);
    options[4].setPosition(390, 410);
    options[4].setFillColor(Color(100, 100, 100));

    void (Menu::*func[4])() = {&Menu::startGame, &Menu::leaderboard, &Menu::instructions, &Menu::exit};
    int i = 5;      // possible functions that can be accessed through main menu

    while (window.isOpen())
    {
        i = 5;
        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                std::exit(1);

            if (e.type == Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left)
            {
                for (i = 1; i < 5; i++)
                    if (mouseClicked(options[i], Mouse::getPosition(window).x, Mouse::getPosition(window).y))
                        break;
            }
        }

        if (i < 5)
            (this->*func[i - 1])();     // call corresponding functions

        window.draw(bg);
        window.draw(options[0]);
        for (int i = 1; i < 5; i++)
        {
            // change color upon hover
            checkMouse(options[i], Mouse::getPosition(window).x, Mouse::getPosition(window).y);
            window.draw(options[i]);
        }

        window.display();
    }
}

void Menu::startGame()
{
    if (TakeName())
        clone->close();
}

void Menu::exit()
{
    std::exit(1);
}

void Menu::instructions()
{
    RenderWindow window(VideoMode(800, 600), "Space Invaders");
    int x = sf::VideoMode::getDesktopMode().width / 2 - 560, y = sf::VideoMode::getDesktopMode().height / 2 - 300;
    window.setPosition({x, y});

    Text text("Instructions ", menu, 55), inst("", font1, 25);
    inst.setString("1. Shoot the invaders to get Score.\n2. Dodge the Dragon to get Score.\n3. Dodge the Monster to get Score.\n4. Collect Addons to get differnet Powers\n5. Dodge the Danger Sign to get Bonus.\n6. Total of 3 levels with 3 waves.\n7. Score as much as you can to get Medals!");
    inst.setLineSpacing(1.5);
    text.setFillColor(sf::Color::Yellow);
    text.setPosition(160, 50);
    inst.setFillColor(Color(216, 191, 216));
    inst.setPosition(180, 150);
    back.setPosition(330, 500);

    while (window.isOpen())
    {
        Event e;

        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
                std::exit(1);

            if (e.type == Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left)
            {
                if (mouseClicked(back, Mouse::getPosition(window).x, Mouse::getPosition(window).y))
                    return;
            }
        }

        window.draw(bg);
        window.draw(text);
        window.draw(back);
        window.draw(inst);
        checkMouse(back, Mouse::getPosition(window).x, Mouse::getPosition(window).y);

        window.display();
    }
}

void Menu::leaderboard()
{
    RenderWindow window(VideoMode(800, 600), "Space Invaders");
    int x = sf::VideoMode::getDesktopMode().width / 2 - 560, y = sf::VideoMode::getDesktopMode().height / 2 - 300;
    window.setPosition({x, y});

    Text text("Leaderboard ", menu, 55), name[3], score[3];
    text.setFillColor(sf::Color::Yellow);
    text.setPosition(140, 100);
    back.setPosition(330, 500);

    string *temp1 = this->file.getName(), *temp2 = this->file.getScore();

    for (int i = 0; i < 3; i++)
    {
        name[i].setFont(font1);
        score[i].setFont(font1);
        name[i].setString(temp1[i]);
        score[i].setString(temp2[i]);
        name[i].setPosition(320, i * 70 + 200);
        score[i].setPosition(470, i * 70 + 200);
        medals[i].setPosition(250, i * 70 + 190);
    }

    while (window.isOpen())
    {
        Event e;

        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
                std::exit(1);

            if (e.type == Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left)
            {
                if (mouseClicked(back, Mouse::getPosition(window).x, Mouse::getPosition(window).y))
                {
                    window.close();
                    return;
                }
            }
        }

        window.draw(bg);
        window.draw(text);
        window.draw(back);
        checkMouse(back, Mouse::getPosition(window).x, Mouse::getPosition(window).y);

        for (int i = 0; i < 3; i++)
        {
            window.draw(name[i]);
            window.draw(score[i]);
            window.draw(medals[i]);
        }

        window.display();
    }
}

bool Menu::mouseClicked(Text &text, float x, float y)
{
    float left_x = text.getPosition().x, right_x = text.getGlobalBounds().width + left_x;
    float top_y = text.getPosition().y, bottom_y = text.getGlobalBounds().height + top_y;

    if (x >= left_x && x <= right_x && y >= top_y && y <= bottom_y)
        return 1;
    else
        return 0;
}

void Menu::checkMouse(Text &text, float x, float y)
{
    float left_x = text.getPosition().x, right_x = text.getGlobalBounds().width + left_x;
    float top_y = text.getPosition().y, bottom_y = text.getGlobalBounds().height + top_y;

    if (x >= left_x && x <= right_x && y >= top_y && y <= bottom_y)
        text.setFillColor(Color(255, 255, 255));
    else
        text.setFillColor(Color(100, 100, 100));
}

void Menu::displayLives(int lives, RenderWindow &window)
{
    static Sprite sprite[3];
    static Texture tex[2];
    tex[0].loadFromFile("img/life.png");
    tex[1].loadFromFile("img/lose_life.png");

    int i;
    for (i = 0; i < lives; i++)
        sprite[i].setTexture(tex[0]);

    for (i; i < 3; i++)
        sprite[i].setTexture(tex[1]);

    for (int i = 0; i < 3; i++)
    {
        sprite[i].setPosition(650 + (i * 40), 15);
        window.draw(sprite[i]);
    }
}

void Menu::displayScore(int newScore, RenderWindow &window)
{
    Text score("Score : 0", font, 20);
    score.setString("Score : " + std::to_string(newScore));
    score.setFillColor(Color::Yellow);
    score.setPosition(50, 10);
    window.draw(score);
}

void Menu::displayWave(int wave, RenderWindow &window)
{
    Text text("Wave - 0", font, 30);
    wave == 0 ? wave = 3 : wave = wave;
    text.setString("Wave - " + std::to_string(wave));
    text.setFillColor(Color::Magenta);
    text.setPosition(350, 10);
    window.draw(text);
}

bool Menu::newLevel(int level, RenderWindow &window)
{
    text.setString("Level - " + std::to_string(level));
    bool check = descend();
    window.draw(text);
    return check;
}

bool Menu::descend()
{
    if (text.getPosition().x > 300)
        return true;

    static float moved = 0;
    float x_offset = 3.5;

    if (moved < 400)
    {
        text.move(x_offset, 0);
        moved += x_offset;
    }
    else
        moved = 0;
    return false;
}

bool Menu::goBack()
{
    static float moved = 0;
    float x_offset = 3.5;
    if (moved < 500)
    {
        text.move(x_offset, 0);
        moved += x_offset;
    }
    else
    {
        moved = 0;
        text.setPosition(-150, 300);
        return true;
    }
    return false;
}

bool Menu::TakeName()
{
    RenderWindow window(VideoMode(800, 600), "Space Invaders");
    int x = sf::VideoMode::getDesktopMode().width / 2 - 560, y = sf::VideoMode::getDesktopMode().height / 2 - 300;

    window.setPosition({x, y});
    Text text("Enter Name ", font, 55), name("", font1, 40);
    text.setFillColor(sf::Color::Yellow);
    text.setPosition(260, 150);
    name.setPosition(240, 280);
    name.setFillColor(sf::Color::Magenta);
    text.setLetterSpacing(2);
    back.setPosition(350, 500);

    bool typing = true;

    sf::Clock cursorClock;
    bool cursorVisible = true;

    sf::RectangleShape cursor({4, text.getLocalBounds().height + 5}), box({330, text.getLocalBounds().height + 14}); // create a cursor shape
    cursor.setFillColor(sf::Color::Black);
    box.setFillColor(sf::Color::White);
    box.setOutlineColor(sf::Color::Magenta);
    box.setOutlineThickness(2);
    box.setPosition(230, 280);

    while (window.isOpen())
    {
        typing = true;
        Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                std::exit(1);
            else if (event.type == sf::Event::TextEntered && typing)
            {
                if (event.text.unicode == 13)       // enter pressed
                    typing = false;
                else if (event.text.unicode == 8 && !(name.getString().isEmpty()))      // backspace pressed
                    name.setString(name.getString().substring(0, name.getString().getSize() - 1));

                else if (event.text.unicode < 128)      // append string
                    name.setString(name.getString() + char(event.text.unicode));
            }
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                if (mouseClicked(back, Mouse::getPosition(window).x, Mouse::getPosition(window).y))
                    return 0;
            }
        }

        if (typing)
        {
            sf::FloatRect textBounds = name.getGlobalBounds();
            cursor.setPosition(textBounds.left + textBounds.width + 3.5, textBounds.top - 5);

            if (cursorClock.getElapsedTime().asMilliseconds() > 500)
            {
                cursorVisible = !cursorVisible;
                cursorClock.restart();
            }
        }

        checkMouse(back, Mouse::getPosition(window).x, Mouse::getPosition(window).y);

        window.draw(bg);
        window.draw(box);
        window.draw(text);
        window.draw(name);
        window.draw(back);
        if (typing && cursorVisible)
            window.draw(cursor);
        if (!typing)
        {
            this->name += name.getString();
            if (this->name.empty())
                this->name = "Guest";
            return 1;
        }

        window.display();
    }
    return 0;
}

bool Menu::pauseScreen()
{
    RenderWindow window(VideoMode(800, 600), "Space Invaders");
    int x = sf::VideoMode::getDesktopMode().width / 2 - 560, y = sf::VideoMode::getDesktopMode().height / 2 - 300;
    window.setPosition({x, y});

    Text text("Paused", menu, 75);
    text.setFillColor(sf::Color::Yellow);
    text.setPosition(230, 50);

    for (int i = 0; i < 5; i++)
        options[i].setFont(menu);

    options[0].setString("Resume");
    options[0].setCharacterSize(40);
    options[0].setPosition(300, 160);
    options[0].setFillColor(Color(105, 99, 255));

    options[1].setString("Restart");
    options[1].setCharacterSize(40);
    options[1].setPosition(300, 230);
    options[1].setFillColor(Color(105, 99, 255));

    options[2].setString("Leaderboard");
    options[2].setCharacterSize(40);
    options[2].setPosition(240, 300);
    options[2].setFillColor(Color(100, 100, 100));

    options[3].setString("Instructions");
    options[3].setCharacterSize(40);
    options[3].setPosition(247, 370);
    options[3].setFillColor(Color(100, 100, 100));

    options[4].setString("Exit");
    options[4].setCharacterSize(40);
    options[4].setPosition(360, 430);
    options[4].setFillColor(Color(255, 255, 255));

    void (Menu::*func[3])() = {&Menu::leaderboard, &Menu::instructions, &Menu::exit};
    int i;

    while (window.isOpen())
    {
        i = 5;
        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                std::exit(1);

            if (e.type == Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left)
            {
                for (i = 0; i < 5; i++)
                    if (mouseClicked(options[i], Mouse::getPosition(window).x, Mouse::getPosition(window).y))
                        break;
            }
        }
        if (i == 0)
            return 0;
        if (i == 1)
            return 1;

        if (i < 5)
            (this->*func[i - 2])();

        window.draw(bg);
        window.draw(text);
        for (int i = 0; i < 5; i++)
        {
            checkMouse(options[i], Mouse::getPosition(window).x, Mouse::getPosition(window).y);
            window.draw(options[i]);
        }

        window.display();
    }
    return 0;
}

bool Menu::gameOver(int score, string disp)
{
    RenderWindow window(VideoMode(800, 600), "Space Invaders");
    int x = sf::VideoMode::getDesktopMode().width / 2 - 560, y = sf::VideoMode::getDesktopMode().height / 2 - 300;
    window.setPosition({x, y});

    this->writeScore(score);

    Text text(disp, menu, 75), myScore("Your Score: " + to_string(score), font, 50);
    text.setFillColor(sf::Color::Red);
    myScore.setFillColor(sf::Color::Yellow);
    text.setPosition(130, 100);
    myScore.setPosition(250, 200);

    for (int i = 0; i < 3; i++)
        options[i].setFont(menu);

    options[0].setString("Restart");
    options[0].setCharacterSize(40);
    options[0].setPosition(310, 370);
    options[0].setFillColor(Color(105, 99, 255));

    options[1].setString("Leaderboard");
    options[1].setCharacterSize(40);
    options[1].setPosition(240, 300);
    options[1].setFillColor(Color(100, 100, 100));

    options[2].setString("Exit");
    options[2].setCharacterSize(40);
    options[2].setPosition(370, 440);
    options[2].setFillColor(Color(255, 255, 255));

    void (Menu::*func[2])() = {&Menu::leaderboard, &Menu::exit};
    int i;

    while (window.isOpen())
    {
        i = 4;
        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                std::exit(1);

            if (e.type == Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left)
            {
                for (i = 0; i < 3; i++)
                    if (mouseClicked(options[i], Mouse::getPosition(window).x, Mouse::getPosition(window).y))
                        break;
            }
        }
        if (i == 0)
            return 1;

        if (i < 3)
            (this->*func[i - 1])();

        window.draw(bg);
        window.draw(text);
        window.draw(myScore);
        for (int i = 0; i < 3; i++)
        {
            checkMouse(options[i], Mouse::getPosition(window).x, Mouse::getPosition(window).y);
            window.draw(options[i]);
        }

        window.display();
    }
    return 0;
}

void Menu::writeScore(int score)
{
    string s = to_string(score);
    file.record_Input(this->name, s);
    cout << file.get_data();
}
