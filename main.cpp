#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;

void init()
{
	// initialise all the objects needed for the game. 
}

void update(float dt) 
{
	std::cout << "FPS: " << 1/dt << "\n";
}

void render(RenderWindow &window) 
{
	// Draw Everything
}

void clean()
{
	//free up the memory if necessary.
}

int main ()
{
	//create the window
	RenderWindow window(VideoMode({500, 500}), "PONG");
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