#include "Game.h"

static Game* s_Instance = nullptr;

Game::Game(std::string_view name) : m_App(name)
{
	m_Grid->InitStatsWindow(m_App.GetWindowSize());
	m_App.AddObjectToDraw(m_Grid);
}

Game::~Game()
{
}

void Game::Init(std::string_view name)
{
	assert(!s_Instance);
	s_Instance = new Game(name);
}

void Game::Run()
{
	m_App.Run();
}

void Game::Shutdown()
{
	delete s_Instance;
	s_Instance = nullptr;
}

Game& Game::Get()
{
	assert(s_Instance);
	return *s_Instance;
}
