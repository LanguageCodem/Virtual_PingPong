/*s --------------------------------------------------------
 *    File: pong_defs.h
 *  Author: Markendy Enock Pierre
 *   Class: COP 2006 - CRN 13969 - Programming 1
 * Purpose: definitions for pong game
 * -------------------------------------------------------- */
#include <SFML/Graphics.hpp>

#ifndef PONGFINAL_PONG_DEFS_H
#define PONGFINAL_PONG_DEFS_H

// Global Constants
// --------------------------------------------------------
// window properties
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const sf::Color WINDOW_COLOR = sf::Color::White;
// drawing properties
const float FRAME_RATE = (1.0/30.0) * 1000.0;       // FPS in ms
const sf::Color BALL_COLOR = sf::Color::Black;
// These are just for fun
// speed that the can accelerate at to span window in
// n-seconds (in ms) broken up for each frame
const float BALL_RADIUS = 10;
const float BALL_SPEED_X = BALL_RADIUS *20.0/1000.0;    // speed horizontally
const float BALL_SPEED_Y = BALL_RADIUS *15.5/1000.0;   // span  vertically
// Type definitions
// --------------------------------------------------------
// drawing paddles properties
const float PADDLE_HEIGHT = 80.0;
const float PADDLE_THICKNESS = 10.0;
const sf::Color PADDLE_COLOR = sf::Color::Magenta;
const float PADDLE_SPEED = PADDLE_HEIGHT/5.0/1000.0;
// --------------------------------------------------------
// wall properties
const float WALL_THICKNESS = 10.0;
const sf::Color WALL_COLOR = sf::Color::Blue;
// --------------------------------------------------------
// Block and Border structures
struct Block{
    float left, top, width, height;
    sf::Color color;
    sf::RectangleShape rectangle;
}; // end of Block struct

struct Moving_Block{
    Block block;
    float velocityX, velocityY;
}; // end of Moving_Block struct

struct Borders{
    Block leftWall;
    Block topWall;
    Block rightWall;
    Block bottomWall;
}; // end of Borders struct
//
// --------------------------------------------------------
// ball properties
struct Ball {
    float radius = BALL_RADIUS;
    float coordinateX, coordinateY;
    float velocityX, velocityY;
    sf::Color color;
}; // end of Ball struct
// --------------------------------------------------------
// direction enum here
enum Direction
{
    Exit = -1,
    None = 0,
    Left = 1,
    Up = 2,
    Right = 3,
    Down = 4,
    Start
};// end of Direction enum
//

#endif //PONGPADDLE_PONG_DEFS_H
