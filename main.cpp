//Includes
#include <SFML/Graphics.hpp>
#include <iostream> //for debug
#include <cstdlib> //for rand
#include <ctime> //for seeding rand
#include <cmath> 

//namespaces
using namespace sf;

//keybinds
Keyboard::Key controls[4] = 
{
	//player 1 binds
	Keyboard::W,
	Keyboard::S,

	//player 2 binds
	Keyboard::Up,
	Keyboard::Down
};

//params
const Vector2f paddleSize(25.f, 100.f);
const float ballRad = 10.f;
const int gameWidth = 800;
const int gameHeight = 600;
const float paddleSpd = 400.f;
const float paddleOffsetWall = 10.f;
const float time_step = 0.017f;
Vector2f ballVel;
bool player1Serve = true;
const int iniVel = 200.f;
const float velMult = 1.1f;
const int minXSpd = 70;

//game objects
CircleShape ball;
RectangleShape paddles[2];

void resetBall()
{
	//set ball pos
	ball.setPosition(gameWidth/2.f, gameHeight/2.f);
	
	Vector2f newBallVel((rand() % (iniVel-minXSpd-1))+minXSpd,0);
	newBallVel.y = sqrt(pow(iniVel, 2.f) - pow(newBallVel.x, 2.f));
	if (rand() % 2 == 1)
	{
		newBallVel.y *= -1;
	}

	//set ball vel
	ballVel = {(player1Serve ? -newBallVel.x : newBallVel.x), newBallVel.y};

	//swap who serves
	player1Serve = !player1Serve;
}

void init()
{
	//set paddle size and origin
	for (RectangleShape &p : paddles)
	{
		p.setSize(paddleSize);
		p.setOrigin(paddleSize/2.f);
	}

	//set ball size and origin
	ball.setRadius(ballRad);
	ball.setOrigin(ballRad,ballRad);

	//set paddle pos
	paddles[0].setPosition(paddleOffsetWall + paddleSize.x/2.f, gameHeight/2.f);
	paddles[1].setPosition(gameWidth-paddleOffsetWall-paddleSize.x/2.f, gameHeight/2.f);

	resetBall();
}

void update(float dt) 
{
	//player 1 movement
	float dir = 0.f;
	if (Keyboard::isKeyPressed(controls[0]))
	{
		dir--;
	}
	else if (Keyboard::isKeyPressed(controls[1]))
	{
		dir++;
	}
	paddles[0].move(Vector2f(0.f, dir * paddleSpd * dt));

	//player 2 movement
	dir = 0.f;
	if (Keyboard::isKeyPressed(controls[2]))
	{
		dir--;
	}
	else if (Keyboard::isKeyPressed(controls[3]))
	{
		dir++;
	}
	paddles[1].move(Vector2f(0.f, dir * paddleSpd * dt));

	//ball movement
	ball.move(ballVel * dt);

	//ball collision
	const Vector2f curPos = ball.getPosition();

	if (curPos.y > gameHeight - ballRad) //bottom
	{
		ballVel *= velMult;
		ballVel.y *= -1;
		ball.move(Vector2f(0.f, (gameHeight - ballRad) - curPos.y));
	}
	else if (curPos.y < ballRad) //top
	{
		ballVel *= velMult;
		ballVel.y *= -1;
		ball.move(Vector2f(0.f, ballRad - curPos.y));
	}

	if (curPos.x < ballRad || curPos.x > gameWidth-ballRad) //left or right
	{
		resetBall();
	}

	const Vector2f pad1Pos = paddles[0].getPosition();
	const Vector2f pad2Pos = paddles[1].getPosition();

	if (//left paddle
		curPos.x - ballRad < pad1Pos.x + paddleSize.x/2.f &&
		curPos.x + ballRad > pad1Pos.x - paddleSize.x/2.f &&
		curPos.y - ballRad < pad1Pos.y + paddleSize.y/2.f &&
		curPos.y + ballRad > pad1Pos.y - paddleSize.y/2.f
	)
	{
		ballVel *= velMult;
		ballVel.x *= -1;
		ball.move(Vector2f((pad1Pos.x+paddleSize.x/2.f+ballRad)-curPos.x,0.f));
	}

	if (//right paddle
		curPos.x - ballRad < pad2Pos.x + paddleSize.x/2.f &&
		curPos.x + ballRad > pad2Pos.x - paddleSize.x/2.f &&
		curPos.y - ballRad < pad2Pos.y + paddleSize.y/2.f &&
		curPos.y + ballRad > pad2Pos.y - paddleSize.y/2.f
	)
	{
		ballVel *= velMult;
		ballVel.x *= -1;
		ball.move(Vector2f((pad2Pos.x-paddleSize.x/2.f-ballRad)-curPos.x,0.f));
	}
}

void render(RenderWindow &window) 
{
	for (RectangleShape &p : paddles)
	{
		window.draw(p);
	}
	window.draw(ball);
}

void clean()
{
	//free up the memory if necessary.
}

int main ()
{
	//seed rand
	srand(time(0));

	//create the window
	RenderWindow window(VideoMode({gameWidth, gameHeight}), "PONG");
	window.setVerticalSyncEnabled(true);

    //initialise and load
	init();
	while(window.isOpen())
	{
		window.clear(); //clear last frame

		Event event;
     	while (window.pollEvent(event))
		{
      		if (event.type == Event::Closed)
			{
        		window.close();
			}
      	}

		//get delta time
		static Clock clock;
		const float dt = clock.restart().asSeconds();

		//call update and render
		update(dt);
		render(window);
		
		//wait for the time_step to finish before displaying the next frame.
		//sleep();
		//Wait for Vsync
		window.display();
	}
	//Unload and shutdown
	clean();
}