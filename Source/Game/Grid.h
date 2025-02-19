#pragma once
#include <string_view>
#include <array>

#include "../App/Object.h"
#include "ThreadPool.h"

////////////////////

struct Cell
{
	Cell() { m_Colour = defaultColour; }

	Cell(ImU32 colour) : m_Colour(colour) {}


	////////////////////

	static constexpr float cellSize = 7.15;
	static constexpr float radius = 3.5;
	static constexpr ImU32 defaultColour = 4285452835; // converted from ImVec4(35 / 255.f, 210 / 255.f, 110 / 255.f, 1.f)
	static constexpr ImVec4 minColour = ImVec4(70 / 255.f, 90 / 255.f, 70 / 255.f, 1.f);
	static constexpr ImVec4 maxColour = ImVec4(190 / 255.f, 180 / 255.f, 180 / 255.f, 1.f);;
	static constexpr int16_t colourChange = 10;
	static constexpr float positionChange = 2.25;

	////////////////////

	ImU32 m_Colour;
	ImVec2 m_Center;
	ImVec2 m_TopLeft;
	ImVec2 m_BottomRight;
	float m_Radius = radius;
};

////////////////////

class Grid : public IDrawableObject
{
public:
	Grid();
	~Grid();

	void InitStatsWindow(const ImVec2 windowSize);

	virtual void Update(float deltaTime) override;
	virtual void Draw() override;

	void RandomlyChangeRow(const uint16_t row);

private:
	void RandomlyChangeGridWithThreads();

	void RandomlyChangeGrid();

	void RandomlyChangeCell(Cell& cell, uint32_t& randomSeed, const int8_t direction);

	////////////////////

	static constexpr uint16_t m_GridSize = 150;
	static constexpr float m_OffsetX = 5;
	static constexpr float m_OffsetY = 5;

	static constexpr float m_GridThickness = 2.5f;
	static constexpr ImU32 m_GridColour = 4289996408; // converted from ImVec4(120 / 255.f, 38 / 255.f, 180 / 255.f, 1.f)

	static constexpr float m_StatsWindowWidth = 300.f;
	static constexpr ImVec2 m_ButtonSize = ImVec2(120, 65);

	static constexpr std::string_view m_StatsWindowName = "Controls and Stats";
	static constexpr std::string_view m_GridSizeText = "Grid Size = %d";
	static constexpr std::string_view m_CellSizeText = "Cell Size = %f";
	static constexpr std::string_view m_TimerText = "Time = %f ms";
	static constexpr std::string_view m_EnbaleThreadsText = "Enable Threads";
	static constexpr std::string_view m_StartButtonText = "Start";
	static constexpr std::string_view m_StopButtonText = "Stop";

	////////////////////

	ImGuiWindowFlags m_WindowFlags = 0;
	ImVec2 m_StatsWindowPosition;
	float m_StatsWindowHeight = 0.f;

	bool m_HasStarted = false;
	bool m_ThreadsEnbaled = false;
	std::chrono::duration<float> m_Duration;

	std::array<Cell, m_GridSize * m_GridSize> m_Cells;

	ImDrawList* m_DrawList;

	ThreadPool m_ThreadPool;
};