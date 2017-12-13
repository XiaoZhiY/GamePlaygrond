#pragma once

#ifndef _LTEXTURE_H
#include "LTexture.h"
#endif // !_LTEXTURE_H

#ifndef _VECTOR
#include <vector>
#endif // !_QUEUE

#ifndef _LTIMER_H
#include "LTimer.h"
#endif // !_LTIMER_H

#ifndef _SSTREAM
#include <sstream>
#endif // !1

#ifndef _SDL_TTF_H
#include <SDL_ttf.h>
#endif // !1

enum GameState
{
	TITLE = 0,
	CHOOSEMAP = 1,
	GAMING = 2,
	RESULT = 3,
	HIGHSCORES = 4
};
enum MoveState
{
	LEFT = 0,
	RIGHT = 1,
	UP = 2,
	DOWN = 3
};
struct Position
{
	int x;
	int y;
};

class MySnake
{
public:
	MySnake();
	~MySnake();
	
	void initCurrentStatement(SDL_Renderer * gRenderer);
	
	void free();
	bool LoadMedia(SDL_Renderer *gRenderer);
	void handleEvent(SDL_Event &e, bool &quit);
	void render(SDL_Renderer *gRenderer);

	//The next snake position and event handle
	void next();
	
	//Get the gameState();
	GameState getGameState();
	
private:
	//Get The next head position
	Position getNextPosition();

	//Rand the food's position
	void setFood();
	//Check food position is available
	bool checkFood(int x,int y);
	//Check collision with snake itself
	bool checkCollisionWithSnake(Position newhead);

	//RestartTheGame
	void initGameData();
	

	//The Game Time
	LTimer PlayTime;
	double totalTime;
	
	//GameStateNum
	GameState gs;

	//MoveStatement
	MoveState ms;

	//Snake and food
	std::vector<Position> snake;
	Position food;
	
	//The game scores
	int score;
	//The snake's speed
	int speed;
	int frame;
	
	//Something associated with choice
	int MapChoice;
	int TitleChoice;
	int ResultChoice;

	SDL_Rect a[3];

	//Texture
	LTexture gTitle;
	LTexture gChooseMap;
	LTexture gChooseIcon;
	LTexture gSnakeAndFood;
	LTexture gResult;
	LTexture gHighScore;
	LTexture gText;

	//The Font
	TTF_Font *gFont;
	//In memory text stream
	std::stringstream Text;

	std::string inputText;
};


inline MySnake::MySnake()
{
	gFont = NULL;
	gs = TITLE;
	TitleChoice = 0;
	MapChoice = 0;
	ResultChoice = 0;

	ms = RIGHT;
	snake.push_back(Position{ 320,240 });
	snake.push_back(Position{ 310,240 });
	
	score = 0;
	speed = 0;
	frame = 0;
	totalTime = 0;

	inputText = "Unknown";

	a[0] = { 0,0,10,10 };
	a[1] = { 0,10,10,10 };
	a[2] = { 0,20,10,10 };
	
}

inline MySnake::~MySnake()
{
}

inline void MySnake::initCurrentStatement(SDL_Renderer * gRenderer)
{
	if (!LoadMedia(gRenderer))
	{
		printf("Fail to load the source current statement need\n");
	}
}

inline void MySnake::initGameData()
{
	TitleChoice = 0;
	MapChoice = 0;
	ResultChoice = 0;

	ms = RIGHT;

	snake.clear();
	snake.push_back(Position{ 320,240 });
	snake.push_back(Position{ 310,240 });

	score = 0;
	speed = 0;
	totalTime = 0;
	frame = 0;
	inputText = "Unknown";
}

inline void MySnake::free()
{
	gTitle.free();
	gChooseMap.free();
	gChooseIcon.free();
	gSnakeAndFood.free();
	gResult.free();
	gHighScore.free();
	gText.free();

	TTF_CloseFont(gFont);
	gFont = NULL;
}

inline bool MySnake::LoadMedia(SDL_Renderer * gRenderer)
{
	//Loadint success flag

	bool success = true;
	//Open the font
	gFont = TTF_OpenFont("source/lazy.ttf", 28);
	if (gFont == NULL)
	{
		printf("Failed to load STXINGKA font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	if (!gTitle.loadFromFile("source/title.png", gRenderer))
	{
		printf("Failed to load title.png!\n");
		success = false;
	}
	if (!gChooseIcon.loadFromFile("source/icon.png", gRenderer))
	{
		printf("Failed to load icon.png!\n");
		success = false;
	}
	if (!gChooseMap.loadFromFile("source/choosemap.png", gRenderer))
	{
		printf("Failed to load choosemap.png!\n");
		success = false;
	}
	if (!gSnakeAndFood.loadFromFile("source/snake.png", gRenderer))
	{
		printf("Failed to load snake.png!\n");
		success = false;
	}
	if (!gResult.loadFromFile("source/result.png", gRenderer))
	{
		printf("Failed to load snake.png!\n");
		success = false;
	}

	return success;
}

inline void MySnake::handleEvent(SDL_Event & e,bool &quit)
{
	if (gs == TITLE)
	{
		if (e.type == SDL_KEYDOWN)
		{
			//Adjust the Choose on title
			switch (e.key.keysym.sym)
			{
			case SDLK_RIGHT:
				TitleChoice = (TitleChoice + 1) % 3;
				break;
			case SDLK_LEFT:
				if (TitleChoice == 0)
					TitleChoice = 2;
				else
					TitleChoice--;
				break;
			case SDLK_RETURN:
				if (TitleChoice == 0)
					gs = CHOOSEMAP;
				else if (TitleChoice == 1)
					gs = HIGHSCORES;
				else if (TitleChoice == 2)
					quit = true;
				break;
			}
		}
		
	}
	else if (gs == CHOOSEMAP)
	{
		if (e.type == SDL_KEYDOWN)
		{
			switch (e.key.keysym.sym)
			{
			case SDLK_RIGHT:
				MapChoice = (MapChoice + 1) % 3;
				break;
			case SDLK_LEFT:
				if (MapChoice == 0)
					MapChoice = 2;
				else
					MapChoice--;
				break;
			case SDLK_RETURN:
				gs = GAMING;
				setFood();
				PlayTime.start();
				break;
			}
		}
	}
	else if (gs == GAMING)
	{
		if (e.type == SDL_KEYDOWN)
		{
			switch (e.key.keysym.sym)
			{
			case SDLK_LEFT:
				if (ms != RIGHT)
					ms = LEFT;
				break;
			case SDLK_RIGHT:
				if (ms != LEFT)
					ms = RIGHT;
				break;
			case SDLK_UP:
				if (ms != DOWN)
					ms = UP;
				break;
			case SDLK_DOWN:
				if (ms != UP)
					ms = DOWN;
				break;
			case SDLK_p:
				//Press "p" to pause
				if (PlayTime.isPaused())
					PlayTime.unpause();
				else
					PlayTime.pause();
				break;
			}
		}
			
	}
	else if (gs == RESULT)
	{
		if (e.type == SDL_KEYDOWN)
		{
			if (e.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0)
			{
				//lop off character
				inputText.pop_back();
			}

			switch (e.key.keysym.sym)
			{
			case SDLK_RIGHT:
				ResultChoice = (ResultChoice + 1) % 3;
				break;
			case SDLK_LEFT:
				if (ResultChoice == 0)
					ResultChoice = 2;
				else
					ResultChoice--;
				break;
			case SDLK_RETURN:
				if (ResultChoice == 0)
				{
					gs = TITLE;
					initGameData();
				}
				else if (ResultChoice == 1)
					gs = HIGHSCORES;
				else if (ResultChoice == 2)
					quit = true;

				//Disable text input
				SDL_StopTextInput();
				break;
			}
		}
		//Special text input event
		else if (e.type == SDL_TEXTINPUT)
		{
			//Not copy or pasting
			if (!((e.text.text[0] == 'c' || e.text.text[0] == 'C') && (e.text.text[0] == 'v' || e.text.text[0] == 'V') && SDL_GetModState() & KMOD_CTRL))
			{
				//Append character
				inputText += e.text.text;
			}
		}
	}
	
}


inline void MySnake::render(SDL_Renderer * gRenderer)
{
	//Clear screen
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);

	if (gs == TITLE)
	{
		int a[3] = { 40,230,400 };
		gTitle.render(gRenderer, 0, 0);
		gChooseIcon.render(gRenderer, a[TitleChoice], 370);
	}
	else if(gs == CHOOSEMAP)
	{
		int a[3] = { 3,97,210 };
		gChooseMap.render(gRenderer, 0, 0);
		gChooseIcon.render(gRenderer, a[MapChoice], 190);
	}
	else if (gs == GAMING)
	{
		Text.str("");
		Text << "Time:" << (PlayTime.getTicks() / 1000.f);
		//Render time
		if (!gText.loadFromRenderedText(Text.str().c_str(), gFont, { 0,0,0,255 }, gRenderer))
		{
			printf("Unable to render time texture!\n");
		}
		gText.render(gRenderer, 0, 0);
		Text.str("");
		Text << "Score:" << score;
		//Render score
		if (!gText.loadFromRenderedText(Text.str().c_str(), gFont, { 0,0,0,255 }, gRenderer))
		{
			printf("Unable to render timetexture!\n");
		}
		gText.render(gRenderer, 500, 0);
		gSnakeAndFood.render(gRenderer, snake[0].x, snake[0].y, &a[0]);
		for (int i = 1;i < snake.size();i++)
		{
			gSnakeAndFood.render(gRenderer, snake[i].x, snake[i].y, &a[1]);
		}
		gSnakeAndFood.render(gRenderer, food.x, food.y, &a[2]);

	}
	else if (gs == RESULT)
	{
		int a[3] = { 45,235,488 };
		gResult.render(gRenderer, 0, 0);
		gChooseIcon.render(gRenderer, a[ResultChoice], 406);

		Text.str("");
		Text << totalTime << " s";
		//Render time
		if (!gText.loadFromRenderedText(Text.str().c_str(), gFont, { 0,0,0,255 }, gRenderer))
		{
			printf("Unable to render time texture!\n");
		}
		gText.render(gRenderer, 272, 178);
		Text.str("");
		Text << score;
		//Render score
		if (!gText.loadFromRenderedText(Text.str().c_str(), gFont, { 0,0,0,255 }, gRenderer))
		{
			printf("Unable to render timetexture!\n");
		}
		gText.render(gRenderer, 266, 243);

		//render your name
		if (inputText != "")
		{
			//Render new text
			gText.loadFromRenderedText(inputText.c_str(), gFont, { 0,0,0,255 }, gRenderer);
		}
		else
		{
			gText.loadFromRenderedText(" ", gFont, { 0,0,0,255 }, gRenderer);
		}
		gText.render(gRenderer, 360, 294);
	}

	//Update screen
	SDL_RenderPresent(gRenderer);
	
}

inline void MySnake::next()
{

	if (gs == GAMING)
	{
		if (!PlayTime.isPaused())
		{
			frame = (frame + 1) % 5;
			if (frame == 4)
			{
				Position NewHead = getNextPosition();
				if (NewHead.x == -10 || NewHead.x == 640 || NewHead.y == -10 || NewHead.y == 480)
				{
					totalTime = PlayTime.getTicks() / 1000.f;
					PlayTime.stop();
					gs = RESULT;
					//Enable text input
					SDL_StartTextInput();

				}
				else if (checkCollisionWithSnake(NewHead))
				{
					totalTime = PlayTime.getTicks() / 1000.f;
					PlayTime.stop();
					gs = RESULT;
					//Enable text input
					SDL_StartTextInput();
				}
				else if (NewHead.x == food.x&&NewHead.y == food.y)
				{
					setFood();
					snake.insert(snake.begin(), NewHead);
					score++;
				}
				else
				{
					snake.pop_back();
					snake.insert(snake.begin(), NewHead);
				}
			}
		}
	}
}

inline GameState MySnake::getGameState()
{
	return gs;
}

inline Position MySnake::getNextPosition()
{
	Position currentHead = snake[0];
	Position nextHead;
	if (ms == LEFT)
	{
		nextHead.x = currentHead.x - 10;
		nextHead.y = currentHead.y;
	}
	else if (ms == RIGHT)
	{
		nextHead.x = currentHead.x + 10;
		nextHead.y = currentHead.y;
	}
	else if (ms == UP)
	{
		nextHead.x = currentHead.x;
		nextHead.y = currentHead.y - 10;
	}
	else if (ms == DOWN)
	{
		nextHead.x = currentHead.x;
		nextHead.y = currentHead.y + 10;
	}
	return nextHead;
}

inline void MySnake::setFood()
{
	int x = rand() % 64;
	int y = rand() % 48;
	while (!checkFood(x, y))
	{
		x = rand() % 64;
		y = rand() % 48;
	}
	food.x = x * 10;
	food.y = y * 10;

}

inline bool MySnake::checkFood(int x,int y)
{
	for (int i = 0;i < snake.size();i++)
	{
		if (snake[i].x == x * 10 && snake[i].y == y * 10)
		{
			return false;
		}
	}
	return true;
}

inline bool MySnake::checkCollisionWithSnake(Position newhead)
{
	//Check Collision with snake body
	for (int i = 1;i < snake.size();i++)
	{
		if (newhead.x == snake[i].x&&newhead.y == snake[i].y)
		{
			return true;
		}
	}
	return false;
}

