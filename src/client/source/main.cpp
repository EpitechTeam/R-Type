/*
** EPITECH PROJECT, 2018
** R-Type
** File description:
**        main.cpp
*/

#include <SFML/Graphics.hpp>
#include <iostream>


class IObject
{
public:
    virtual std::string getObjectName() const = 0;
    virtual bool isActive() const = 0;
    virtual void setActive(bool _active) = 0;
};


class AObject : public IObject
{
protected:
    bool _active;
    std::string _name;

public:
    explicit AObject(bool _active = false, const std::string &_name = "");
    virtual ~AObject() = default;

    std::string getObjectName() const override;
    bool isActive() const override;
    void setActive(bool _active) override;
};

/*
 *
class Menu {

public:
    sf::Sprite background;

    Menu() {
        sf::Texture texture;
        if (!texture.loadFromFile("../myasset/stars.png"))
        {
            std::cout << "ERROR" << std::endl;
        }
        this->background(texture);
    }

    ~Menu() {

    }

    void draw(sf::RenderWindow window){
        this->DrawBackGround(window);
    }

    void DrawBackGround(sf::RenderWindow window) {
        window.draw(this->background);
    }
};
*/

//TODO SET SIZE
//void setSize(sf::)

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "R * TYPE by [ EZTeam feat BABOU'GAMES ] ®");


    sf::Texture texture;
    if (!texture.loadFromFile("./myasset/background.jpg")) {
        std::cout << "ERROR TEXTURE" << std::endl;
    }
    sf::Sprite background(texture);
    background.setScale(1.9f, 1.4f);




    sf::Texture texture2;
    if (!texture2.loadFromFile("./myasset/r-type.png")) {
        std::cout << "ERROR TEXTURE" << std::endl;
    }
    sf::Sprite logo(texture2);
    logo.setPosition(60, 70);


    sf::Texture texture3;
    if (!texture3.loadFromFile("./myasset/starship.png")) {
        std::cout << "ERROR TEXTURE" << std::endl;
    }
    sf::Sprite starship(texture3);
    float positionx = 30;
    float positiony = 442;
    starship.setPosition(positionx, positiony);
    starship.setScale(0.3f, 0.3f);

    sf::Font font;
    if (!font.loadFromFile("./myasset/space font.ttf")) {
        std::cout << "ERROR FONT" << std::endl;
    }
    sf::Text text;

    text.setFont(font); // font est un sf::Font

    //text.setString("single player");
    text.setString("multyplayer");
    //text.setString("exit");

// choix de la taille des caractères
    text.setCharacterSize(120); // exprimée en pixels, pas en points !
    text.setPosition(250, 180 + 200);
// choix de la couleur du texte
 //   text.setColor(sf::Color::Red);


    sf::Text text2;

    text2.setFont(font); // font est un sf::Font

    //text2.setString("single player");
    text2.setString("Exit");
    //text2.setString("exit");

// choix de la taille des caractères
    text2.setCharacterSize(120); // exprimée en pixels, pas en points !
    text2.setPosition(360, 350 + 200);
// choix de la couleur du text2e
   // text2.setColor(sf::Color::Red);

float x  = 0;
    //Menu *menu = new Menu();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event)) {
            //std::cout << starship.getPosition().y << std::endl;
            if (event.type == sf::Event::Closed)
                window.close();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                starship.move(-3.f, 0.f);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                starship.move(3.f, 0.f);
            }

            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                starship.move(0.f, 3.f);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                starship.move(0.f, -3.f);
            }

            if(starship.getPosition().y > 550)
            {
                text2.setColor(sf::Color::Red);
                text2.setCharacterSize(128);
                text.setColor(sf::Color::White);
                text.setCharacterSize(120);

            }
            else{

                text.setColor(sf::Color::Red);
                text.setCharacterSize(128);
                text2.setColor(sf::Color::White);
                text2.setCharacterSize(120);

            }

        }
        window.clear();
        x -= 0.015;
        background.setPosition(x,0 );
        window.draw(background);
        window.draw(text);
        window.draw(text2);
        window.draw(logo);
        window.draw(starship);
        window.display();
    }
    return 0;
}