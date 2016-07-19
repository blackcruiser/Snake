// Snake.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Game.h"

int main(void)
{
	Game *game = Game::GetInstance();

	game->Initialize();
	game->Run();
	game->Terminate();
	
	return 0;
}