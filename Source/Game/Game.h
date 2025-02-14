#pragma once
#include "../App/App.h"
#include "Grid.h"

////////////////////

class Game
{
public:
	~Game();

	static void Init(std::string_view name = "");

	static void Shutdown();

	static Game& Get();

	void Run();

private:
	Game(std::string_view name);

	////////////////////

	App m_App;

	std::shared_ptr<Grid> m_Grid = std::make_shared<Grid>();
};