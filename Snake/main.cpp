// Snake.cpp : �������̨Ӧ�ó������ڵ㡣
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