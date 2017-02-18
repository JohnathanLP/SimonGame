#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <time.h>

#define WIDE 600
#define HIGH 600

#define CIRCLESIZE 125

int main()
{

  sf::ContextSettings settings;
  settings.antialiasingLevel = 8;

  sf::RenderWindow window(sf::VideoMode(WIDE, HIGH), "Simon", sf::Style::Default, settings);

  sf::CircleShape redCircle(CIRCLESIZE);
  redCircle.setPosition(25,25);
  redCircle.setFillColor(sf::Color(100,0,0));
  sf::CircleShape grnCircle(CIRCLESIZE);
  grnCircle.setPosition(325,25);
  grnCircle.setFillColor(sf::Color(0,100,0));
  sf::CircleShape ylwCircle(CIRCLESIZE);
  ylwCircle.setPosition(25,325);
  ylwCircle.setFillColor(sf::Color(100,100,0));
  sf::CircleShape bluCircle(CIRCLESIZE);
  bluCircle.setPosition(325,325);
  bluCircle.setFillColor(sf::Color(0,0,100));

  sf::Font font;
  if(!font.loadFromFile("orange juice 2.0.ttf"))
  {
    std::cout << "Error loading font!" << std::endl;
    return 0;
  }
  sf::Text text;
  text.setFont(font);
  text.setString("Game Over...");
  text.setColor(sf::Color::White);
  text.setCharacterSize(70);
  text.setPosition(125,250);

  sf::Text scoreText;
  scoreText.setFont(font);
  scoreText.setColor(sf::Color::White);
  scoreText.setCharacterSize(40);
  scoreText.setPosition(155,320);

  sf::Text highScoreText;
  highScoreText.setFont(font);
  highScoreText.setColor(sf::Color::White);
  highScoreText.setCharacterSize(40);
  highScoreText.setPosition(155,340);

  sf::RectangleShape rectangle(sf::Vector2f(390, 130));
  rectangle.setPosition(115,250);
  rectangle.setFillColor(sf::Color::Black);
  rectangle.setOutlineThickness(10);
  rectangle.setOutlineColor(sf::Color(50,50,50));

  sf::SoundBuffer buffer;
  if(!buffer.loadFromFile("Robot_blip.wav"))
  {
    std::cout << "Error loading sound" << std::endl;
    return 0;
  }

  sf::Sound sound;
  sound.setBuffer(buffer);

  int colorDelay = 0;
  bool inputPhase = false;
  bool seriesReady = true;
  int iterator = 0;
  int inputIterator = 0;
  std::vector<int> series;
  int inputNum = 0;
  bool gameOver = false;
  int score = 0;
  srand (time(NULL));
  int DELAY = 300;
  bool pause = false;

  while(window.isOpen())
  {
    sf::Event event;
    while(window.pollEvent(event))
    {
      if(event.type == sf::Event::Closed)
      {
        //Closes the window if a close event is received
        window.close();
      }
      if(event.type == sf::Event::LostFocus)
      {
        pause = true;
      }
      if(event.type == sf::Event::GainedFocus)
      {
        pause = false;
      }
    }

    if(!pause)
    {
      //starts a series
      if(!inputPhase && seriesReady && colorDelay<=0 && !gameOver)
      {
        //std::cout << "beginning a series..." << std::endl;
        series.push_back(rand()%4);
        iterator = series.size()-1;
        inputIterator = series.size()-1;
        seriesReady = false;
        colorDelay = DELAY;
        if(DELAY >=10)
          DELAY -= 10;
      }

      //plays a series
      if(colorDelay<=0 && iterator>=0 && !gameOver)
      {
        //std::cout << "playing a series..." << std::endl;
        if(series[iterator]==0)
        {
          colorDelay = DELAY;
          redCircle.setFillColor(sf::Color(255,0,0));
          sound.setPitch(0.8);
          sound.play();
        }
        else if(series[iterator]==1)
        {
          colorDelay = DELAY;
          ylwCircle.setFillColor(sf::Color(255,255,0));
          sound.setPitch(1);
          sound.play();
        }
        else if(series[iterator]==2)
        {
          colorDelay = DELAY;
          grnCircle.setFillColor(sf::Color(0,255,0));
          sound.setPitch(1.2);
          sound.play();
        }
        else if(series[iterator]==3)
        {
          colorDelay = DELAY;
          bluCircle.setFillColor(sf::Color(0,0,255));
          sound.setPitch(1.4);
          sound.play();
        }
        iterator--;
        if(iterator<0)
        {
          inputPhase = true;
        }
      }

      //handles player input
      if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && colorDelay <= 0 && inputPhase)
      {
        if(!gameOver)
        {
          int mouseX = sf::Mouse::getPosition(window).x;
          int mouseY = sf::Mouse::getPosition(window).y;

          if(25<=mouseX && mouseX<275)
          {
            if(25<=mouseY && mouseY<275)
            {
              inputNum = 0;
              colorDelay = DELAY;
              redCircle.setFillColor(sf::Color(255,0,0));
              sound.setPitch(0.8);
              sound.play();
            }
            else if(325<=mouseY && mouseY<575)
            {
              inputNum = 1;
              colorDelay = DELAY;
              ylwCircle.setFillColor(sf::Color(255,255,0));
              sound.setPitch(1.0);
              sound.play();
            }
          }
          else if(325<=mouseX && mouseX < 575)
          {
            if(25<=mouseY && mouseY<275)
            {
              inputNum = 2;
              colorDelay = DELAY;
              grnCircle.setFillColor(sf::Color(0,255,0));
              sound.setPitch(1.2);
              sound.play();
            }
            else if(325<=mouseY && mouseY<575)
            {
              inputNum = 3;
              colorDelay = DELAY;
              bluCircle.setFillColor(sf::Color(0,0,255));
              sound.setPitch(1.4);
              sound.play();
            }
          }

          if(inputNum == series[inputIterator])
          {
            //std::cout << "Correct!"<< std::endl;
            score++;
            inputIterator--;
          }
          else
          {
            //std::cout << "Wrong!" << std::endl;
            gameOver = true;
            sound.setPitch(0.2);
            sound.play();
          }

          if(inputIterator < 0)
          {
            seriesReady = true;
            inputPhase = false;
          }
        }
        else
        {
          inputPhase = false;
          seriesReady = true;
          series.clear();
          iterator = 0;
          inputIterator = 0;
          gameOver = false;
          score = 0;
          DELAY = 300;
        }
      }

      window.clear(sf::Color::White);

      window.draw(redCircle);
      window.draw(grnCircle);
      window.draw(ylwCircle);
      window.draw(bluCircle);

      //game over
      if(gameOver == true)
      {
        window.draw(rectangle);
        window.draw(text);
        scoreText.setString("Your Score: " + std::to_string(score));
        window.draw(scoreText);
      }

      window.display();

      colorDelay--;
      if(colorDelay <= DELAY/10)
      {
        redCircle.setFillColor(sf::Color(100,0,0));
        grnCircle.setFillColor(sf::Color(0,100,0));
        ylwCircle.setFillColor(sf::Color(100,100,0));
        bluCircle.setFillColor(sf::Color(0,0,100));
      }
    }
  }
}
