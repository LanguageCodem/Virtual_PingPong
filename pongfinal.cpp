/* --------------------------------------------------------
 *    File: pongpaddle.cpp
 *  Author: Markendy Enock Pierre
 *   Class: COP 2006 - CRN 13969 - Programming 1
 * Purpose: adding the player paddle to this program with some basic game-ability
 * -------------------------------------------------------- */
#include "pong_defs.h"

// Function declarations (prototypes)
// --------------------------------------------------------
void setup(Ball &ball, Borders &walls, Moving_Block &paddle, Moving_Block &paddle_Ai);
Direction processInput();
bool update(Direction &input, Ball &ball, Borders walls, Moving_Block &paddle, Moving_Block &paddle_Ai, float delta, bool &started);
void render(sf::RenderWindow &window, Ball ball, float delta, Borders wallBorders, Moving_Block playerPaddle, Moving_Block player_AI);
bool checkCollision(Ball ball, Block block);
bool checkBlockCollision(Block moving, Block stationary);
bool doCollisionCheck(Ball &ball, Borders walls, Moving_Block &paddle, Moving_Block &paddle_Ai);
/**
 * The main application
 * @return OS status message (0=Success)
 */

int main() {

    // create a 2d graphics window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "HelloSFML");
    window.clear(WINDOW_COLOR);

    // declaring the Ball and Borders struct in main
    Ball theBall;
    Borders wallBorders;
    // declaring the player paddle struct in main
    Moving_Block playerPaddle;
    Moving_Block paddle_AI;

    setup(theBall, wallBorders, playerPaddle, paddle_AI);

    // timing variables for the main game loop
    sf::Clock clock;
    sf::Time startTime = clock.getElapsedTime();
    sf::Time stopTime = startTime;
    float delta = 0.0;
    bool started = false;
    bool gameOver = false;
    while (!gameOver)
    {
        // calculate frame time
        stopTime = clock.getElapsedTime();
        delta += (stopTime.asMilliseconds() - startTime.asMilliseconds());
        startTime = stopTime;
        // process events
        sf::Event event;
        while (!gameOver && window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                gameOver = true;
        }
        // Process user input
        // ------------------------------------------------
        Direction userInput = processInput();
        if (userInput == Exit)
            gameOver = Exit;
        // Process Updates
        // ------------------------------------------------
        if (delta >= FRAME_RATE) {    // if we have made it at least a full frame time

            gameOver = update(userInput, theBall,wallBorders, playerPaddle, paddle_AI, delta, started);
            // subtract the frame-rate from the current frame-time
            // for each full frame covered by this update
            while (delta >= FRAME_RATE)
                delta -= FRAME_RATE;
        }
        // Render the window
        // ------------------------------------------------
        render(window, theBall, delta, wallBorders, playerPaddle, paddle_AI);

    } // end main game loop
    // game ending, close the graphics window
    window.close();
    return 0;   // return success to the OS
} // end main
/**
 * Onetime initialization of game objects
 * @param ball defining the ball properties
 * @param walls defining the wall properties
 * @param paddle defining the paddle properties
 */

void setup(Ball &ball, Borders &walls, Moving_Block &paddle, Moving_Block &paddle_Ai){

    // Left wall properties
    walls.leftWall.left = 0.0;
    walls.leftWall.top = 0.0;
    walls.leftWall.width = WALL_THICKNESS;
    walls.leftWall.height = WINDOW_HEIGHT;
    walls.leftWall.rectangle.setSize(sf::Vector2(walls.leftWall.width, walls.leftWall.height));
    walls.leftWall.rectangle.setPosition(sf::Vector2(walls.leftWall.left, walls.leftWall.top));
    walls.leftWall.rectangle.setFillColor(WALL_COLOR);

    // Top wall properties
    walls.topWall.left = 0.0;
    walls.topWall.top = 0.0;
    walls.topWall.width = WINDOW_WIDTH;
    walls.topWall.height = WALL_THICKNESS;
    walls.topWall.rectangle.setSize(sf::Vector2(walls.topWall.width, walls.topWall.height));
    walls.topWall.rectangle.setPosition(walls.topWall.left, walls.topWall.top);
    walls.topWall.rectangle.setFillColor(WALL_COLOR);

    // Right wall properties
    walls.rightWall.left = WINDOW_WIDTH - WALL_THICKNESS;
    walls.rightWall.top = 0.0;
    walls.rightWall.width = WALL_THICKNESS;
    walls.rightWall.height = WINDOW_HEIGHT;
    walls.rightWall.rectangle.setSize(sf::Vector2(walls.rightWall.width, walls.rightWall.height));
    walls.rightWall.rectangle.setPosition(walls.rightWall.left, walls.rightWall.top);
    walls.rightWall.rectangle.setFillColor(WALL_COLOR);

    // Bottom wall properties
    walls.bottomWall.left = 0.0;
    walls.bottomWall.top = WINDOW_HEIGHT - WALL_THICKNESS;
    walls.bottomWall.width = WINDOW_WIDTH;
    walls.bottomWall.height = WALL_THICKNESS;
    walls.bottomWall.rectangle.setSize(sf::Vector2(walls.bottomWall.width, walls.bottomWall.height));
    walls.bottomWall.rectangle.setPosition(walls.bottomWall.left, walls.bottomWall.top);
    walls.bottomWall.rectangle.setFillColor(WALL_COLOR);

    // Paddle properties
    paddle.block.left = PADDLE_THICKNESS;
    paddle.block.top = (WINDOW_HEIGHT - PADDLE_HEIGHT)/2.0;
    paddle.block.width = PADDLE_THICKNESS;
    paddle.block.height = PADDLE_HEIGHT;
    paddle.block.rectangle.setFillColor(PADDLE_COLOR);
    paddle.block.rectangle.setSize(sf::Vector2f(paddle.block.width, paddle.block.height));
    paddle.velocityX = 0.0;
    paddle.velocityY = 0.0;

    // Paddle_AI properties
    paddle_Ai.block.left = PADDLE_THICKNESS;
    paddle_Ai.block.top = (WINDOW_HEIGHT - PADDLE_HEIGHT)/2.0;
    paddle_Ai.block.width = PADDLE_THICKNESS;
    paddle_Ai.block.height = PADDLE_HEIGHT;
    paddle_Ai.block.rectangle.setFillColor(PADDLE_COLOR);
    paddle_Ai.block.rectangle.setSize(sf::Vector2f(paddle_Ai.block.width, paddle_Ai.block.height));
    paddle_Ai.velocityX = 0.0;
    paddle_Ai.velocityY = 0.0;

    // defining the ball properties
    ball.coordinateX = paddle.block.left + paddle.block.width + ball.radius + 1;
    ball.coordinateY = paddle.block.top + (paddle.block.height / 2.0);
    ball.velocityX = 0.0;
    ball.velocityY = 0.0;
    ball.color = BALL_COLOR;


} // end setup

/**
 * convert user keyboard input into recognized integer values
 * for left=1/up=2/right=3/down=4
 * @return Direction - user input (default no-input= None, quit= Exit)
 */

Direction processInput() {
    Direction input = None;  // no input
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        input = Left;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
             sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        input = Up;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
             sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        input = Right;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
             sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        input = Down;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
        input = Exit;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        input = Start;
    }

    return input;
} // end getUserInput
/**
 * update the state of game objects and return a bool true if the ball hits the left or Right wall
 * @param input - user keyboard input
 * @param ball  - update ball position and speed. Also checking for a leftwall collision to enter gameOver.
 * @param walls - collision checking to see if the ball hit the left, top, right, and bottom wall.
 * @param paddle - adding the position of the paddle and the up/down movement
 * @param delta - current frame time
 * @param started - check to see if the user pressed the spacebar
 * @param return gameOver - returning a bool variable to check to see if the ball hit the leftwall
 */

bool update(Direction &input, Ball &ball, Borders walls, Moving_Block &paddle, Moving_Block &paddle_Ai, float delta, bool &started) {
    // adjust velocity directions for user input
    if (input) {
        switch (input) {
            case Start:
                // if statement
                if(!started){
                    ball.velocityX = BALL_SPEED_X;
                    ball.velocityY = BALL_SPEED_Y;

                    if((int(delta * 10) & 1)% 2){
                        ball.velocityY *= -1; // ball goes down if odd
                    }
                    started = true;
                }// end of if-statement
                break;
                //case Left:
                // paddle.velocityX -= BALL_SPEED_X;
                // break;
            case Up:
                paddle.velocityY -= PADDLE_SPEED;
                break;
                //case Right:
                // paddle.velocityX += BALL_SPEED_X;
                // break;
            case Down:
                paddle.velocityY += PADDLE_SPEED;
        }
        // clear input
        input = None;
    }
    // adjust the location of the paddle
    paddle.block.top += paddle.velocityY * delta;
    paddle_Ai.block.top += paddle_Ai.velocityY * delta;

    if(started) {
        // adjust the location of the ball for speed * time
        ball.coordinateX += ball.velocityX * delta;
        ball.coordinateY += ball.velocityY * delta;

        // Vertical center of the paddle
        float aiPaddleCenter = (paddle_Ai.block.top + paddle.block.height) / 2.0;

        if(ball.coordinateY < aiPaddleCenter){
            paddle_Ai.velocityY -= PADDLE_SPEED;
        }
        else if(ball.coordinateY > aiPaddleCenter){
            paddle_Ai.velocityY += PADDLE_SPEED;
        }
    }
    else {
        ball.coordinateX = paddle.block.left + paddle.block.width + ball.radius + 1;
        ball.coordinateY = paddle.block.top + (paddle.block.height / 2.0);
    } //end if-else statement

    return doCollisionCheck(ball,walls,paddle,paddle_Ai);
} // end update
/**
 * draw the ball on the graphics window
 * @param window - handle to open graphics window
 * @param ball   - structure variable with properties for the ball
 * @param delta  - amount of frame time plus lag (in ms)
 * @param wallBorders - drawing the wall borders
 * @param playerPaddle - drawing the paddles
 */

void render(sf::RenderWindow &window, Ball ball, float delta, Borders wallBorders, Moving_Block playerPaddle, Moving_Block paddle_AI) {
    // Render drawing objects
    // ------------------------------------------------
    // clear the window with the background color
    window.clear(WINDOW_COLOR);
    // draw the ball
    // ------------------------------------------------
    sf::CircleShape circle;
    circle.setFillColor(ball.color);
    circle.setRadius(ball.radius);

    // set screen coordinates relative to the center of the circle
    circle.setOrigin( ball.radius, ball.radius);
    // calculate current drawing location relative to speed and frame-time
    float xPosition = ball.coordinateX + ball.velocityX * delta;
    float yPosition = ball.coordinateY + ball.velocityY * delta;
    circle.setPosition(xPosition, yPosition);
    window.draw(circle);

    // draw wall rectangles
    window.draw(wallBorders.topWall.rectangle);
    window.draw(wallBorders.bottomWall.rectangle);
    window.draw(wallBorders.leftWall.rectangle);
    window.draw(wallBorders.rightWall.rectangle);

    // calculate player paddle position
    float xPaddlePosition = playerPaddle.block.left + playerPaddle.velocityX * delta;
    float yPaddlePosition = playerPaddle.block.top + playerPaddle.velocityY * delta;
    playerPaddle.block.rectangle.setPosition(xPaddlePosition, yPaddlePosition);
    window.draw(playerPaddle.block.rectangle);

    // calculate paddle_AI position
    float xAI_Position =  WINDOW_WIDTH - 2 * PADDLE_THICKNESS;
    float yAI_Position = paddle_AI.block.top + paddle_AI.velocityY * delta;
    paddle_AI.block.rectangle.setPosition(xAI_Position, yAI_Position);
    window.draw(paddle_AI.block.rectangle);

    // show the new window
    // ------------------------------------------------
    window.display();
} // end render
/**
 * checkCollision function is going to check if the ball hits the wall
 * @param ball bounding box around circle
 * @param wall rectangle to check hit against
 * @return bool - true if the ball hits the wall
 */

bool checkCollision(Ball ball, Block block){
    bool collision = false;

    float ballLeft = ball.coordinateX - ball.radius;
    float ballTop = ball.coordinateY - ball.radius;
    float ballRight = ball.coordinateX + ball.radius;
    float ballBottom = ball.coordinateY + ball.radius;

    float boxLeft = block.left;
    float boxTop = block.top;
    float boxRight = block.left + block.width;
    float boxBottom = block.top + block.height;

    if(ballLeft < boxRight &&
       ballRight > boxLeft &&
       ballTop < boxBottom &&
       ballBottom > boxTop){

        collision = true;
    }
    return collision;
} // end checkCollision
/**
 * checkBlockCollision function is going to check
 * @param moving
 * @param stationary
 * @return bool - true if the ball hits the moving and stationary block
 */

bool checkBlockCollision(Block moving, Block stationary){
    bool collision = false;

    float movingLeft = moving.left;
    float movingTop = moving.top;
    float movingRight = moving.left + moving.width;
    float movingBottom = moving.top + moving.height;

    float stationaryLeft = stationary.left;
    float stationaryTop = stationary.top;
    float stationaryRight = stationary.left + stationary.width;
    float stationaryBottom = stationary.top + stationary.height;

    if(movingLeft < stationaryRight &&
       movingRight > stationaryLeft &&
       movingTop < stationaryBottom &&
       movingBottom > stationaryTop){

        collision = true;
    }

    return collision;

}// end of checkBlockCollision
/**
 * checkCollision function is going to check if the ball hits the wall
 * @param ball bounding box around circle
 * @param wall rectangle to check hit against
 * @param paddle
 * @return bool - true if the ball hits the wall
 */

bool doCollisionCheck(Ball &ball, Borders walls, Moving_Block &paddle, Moving_Block &paddle_Ai){
    bool gameOver = false;

    // checking for player paddle collision first

    if(checkCollision(ball,paddle.block)){
        ball.velocityX *= - 1;
        ball.coordinateX = paddle.block.left + paddle.block.width + ball.radius + 1;
    }
    else if(checkCollision(ball,walls.rightWall)){
        ball.velocityX *= - 1;
        ball.coordinateX = walls.rightWall.left - ball.radius - 1;

        gameOver = true;
    }
    
    // checking the horizontal collisions

    if(checkCollision(ball,walls.leftWall)){
        ball.velocityX *= - 1;
        ball.coordinateX = walls.leftWall.left + walls.leftWall.width + ball.radius + 1;

        gameOver = true;
    }
    else if(checkCollision(ball,walls.rightWall)){
        ball.velocityX *= - 1;
        ball.coordinateX = walls.rightWall.left - ball.radius - 1;
    }

    // checking the vertical collisions

    if(checkCollision(ball,walls.topWall)){
        ball.velocityY *= - 1;
        ball.coordinateY = walls.topWall.top + walls.topWall.height + ball.radius + 1;
    }
    else if(checkCollision(ball,walls.bottomWall)){
        ball.velocityY *= - 1;
        ball.coordinateY = walls.bottomWall.top - ball.radius - 1;
    }

    // check collision of blocks

    if(checkBlockCollision(paddle.block,walls.topWall)){
        paddle.velocityY = 0.0;
        paddle.block.top = walls.topWall.top + walls.topWall.height + 1;
    }
    else if(checkBlockCollision(paddle.block,walls.bottomWall)){
        paddle.block.top = walls.bottomWall.top - paddle.block.height - 1;
        paddle.velocityY = 0.0;
    }
    if(checkBlockCollision(paddle_Ai.block,walls.topWall)){
        paddle_Ai.velocityY = 0.0;
        paddle_Ai.block.top = walls.topWall.top + walls.topWall.height + 1;
    }
    else if(checkBlockCollision(paddle_Ai.block,walls.bottomWall)){
        paddle_Ai.block.top = walls.bottomWall.top - paddle_Ai.block.height - 1;
        paddle_Ai.velocityY = 0.0;
    }

    return gameOver;


}// end of doCollisionCheck