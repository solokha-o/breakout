//
// breakout.c
//
// Computer Science 50
// Problem Set 3
//

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include <spl/gevents.h>
#include <spl/gobjects.h>
#include <spl/gwindow.h>

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

// height and width of the bricks in pixels
#define BRICK_HEIGHT 10
#define BRICK_WIDTH 40

// spaces between the bricks in pixels
#define BRICK_SPACE (50/11)

// radius of ball in pixels
#define RADIUS 10

// lives
#define LIVES 3

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);

int main(void)
{
    // seed pseudorandom number generator
    srand48(time(NULL));

    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);

    // instantiate bricks
    initBricks(window);

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);

    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);

    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;

    // number of points initially
    int points = 0;

    //ball speed
    double xspeed =drand48() + 1.5;
    double yspeed = 2.5;
    
    // keep playing until game over
    while (lives > 0 && bricks > 0)
    {

        //Scoreboard
        updateScoreboard(window, label, points);
        
        // move ball
        move(ball, xspeed, yspeed);

        pause(10);
    
        GEvent event = getNextEvent(MOUSE_EVENT);
        
        if (event != NULL)
        {
            if (getEventType(event) == MOUSE_MOVED)
                {
                    // ensure circle follows top cursor
                    double x = getX(event) - getWidth(paddle) / 2;
                    double y = getY(paddle);
                    setLocation(paddle, x, y);
                }
        }  

        // update the ball's position
        
        
        // when ball hits left wall
        if (getX(ball) <= 0)
        {
            xspeed = -xspeed;
        }
        
        // when the ball hits the top wall.
        if (getY(ball) <= 0)
        {
            yspeed = -yspeed;
        }
   
        // when ball hits the right wall
        if (getX(ball) + getWidth(ball) >= 400)
        {
            xspeed = -xspeed;
        }
        
        // bottom wall below paddle
        if (getY(ball) + getHeight(ball) >= 600)
        {
            lives = lives - 1;
            // reset
            setLocation(ball, WIDTH / 2 - RADIUS, HEIGHT / 2 + RADIUS);
            setLocation(paddle, 160, 500);
            waitForClick();
        }
    }

    // wait for click before exiting
    waitForClick();

    // game over
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
    string colour[5] = {"RED", "ORANGE", "CYAN", "GREEN", "BLUE"};
	int offset = 30;
    	
    // TODO
    for (int i = 0; i < ROWS; i++)
    {
        for(int j = 0; j < COLS; j++)
        {
                GRect rect = newGRect((BRICK_SPACE + j*(BRICK_SPACE + BRICK_WIDTH)), (BRICK_SPACE + i*(BRICK_SPACE + BRICK_HEIGHT) + offset), BRICK_WIDTH, BRICK_HEIGHT);
                setColor(rect, colour[i]);
                setFilled(rect, true);
				add(window, rect);
        }    
    }
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    GOval ball = newGOval(200, 300, 3, 3);
    setFilled(ball, true);
    setColor(ball, "BLACK");
    add(window, ball);
    return ball;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    GRect rect = newGRect(175, 580, 50, 10);
    setFilled(rect, true);
    setColor(rect, "BLACK");
    add(window, rect);
    return rect;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    // TODO
    return NULL;
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}
