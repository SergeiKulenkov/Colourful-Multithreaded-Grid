#include <chrono>
#include <iostream>

#include "../glm/glm/glm.hpp"
#include "Grid.h"
#include "Game.h"

////////////////////

Grid::Grid()
{
	uint16_t rowStartIndex = 0;
	for (uint16_t row = 0; row < m_GridSize; row++)
	{
		rowStartIndex = row * m_GridSize;
		for (uint16_t column = 0; column < m_GridSize; column++)
		{
			m_Cells[rowStartIndex + column].m_TopLeft = ImVec2(Cell::cellSize * column + m_OffsetX, Cell::cellSize * row + m_OffsetY);
			m_Cells[rowStartIndex + column].m_BottomRight = ImVec2(Cell::cellSize * (column + 1) + m_OffsetX, Cell::cellSize * (row + 1) + m_OffsetY);
			m_Cells[rowStartIndex + column].m_Center = ImVec2(Cell::cellSize / 2 + m_Cells[row * m_GridSize + column].m_TopLeft.x,
																Cell::cellSize / 2 + m_Cells[row * m_GridSize + column].m_TopLeft.y);
		}
	}
}

Grid::~Grid()
{
	m_DrawList = nullptr;
}

void Grid::InitStatsWindow(const ImVec2& windowSize)
{
	m_StatsWindowHeight = windowSize.y;
	m_StatsWindowPosition = ImVec2(windowSize.x - m_StatsWindowWidth, 0);
	m_WindowFlags |= ImGuiWindowFlags_NoMove;
	m_WindowFlags |= ImGuiWindowFlags_NoResize;
	m_WindowFlags |= ImGuiWindowFlags_NoCollapse;
}

void Grid::Update(float deltaTime)
{
	if (m_HasStarted)
	{
		const auto start = std::chrono::high_resolution_clock::now();

		if (m_ThreadsEnbaled)
		{
			RandomlyChangeGridWithThreads();
		}
		else
		{
			RandomlyChangeGrid();
		}

		const auto end = std::chrono::high_resolution_clock::now();
		m_Duration = end - start;
	}
}

void Grid::Draw()
{
	m_DrawList = ImGui::GetBackgroundDrawList();
	if (m_DrawList != nullptr)
	{
		for (const Cell& cell : m_Cells)
		{
			m_DrawList->AddRect(cell.m_TopLeft, cell.m_BottomRight, m_GridColour, 0, 0, m_GridThickness);
			m_DrawList->AddCircleFilled(cell.m_Center, cell.m_Radius, cell.m_Colour);
		}
	}

	ImGui::SetNextWindowPos(m_StatsWindowPosition);
	ImGui::SetNextWindowSize(ImVec2(m_StatsWindowWidth, m_StatsWindowHeight));
	ImGui::Begin(m_StatsWindowName.data(), NULL, m_WindowFlags);
	ImGui::Text(m_GridSizeText.data(), m_GridSize);
	ImGui::Text(m_CellSizeText.data(), Cell::cellSize);

	if (m_HasStarted)
	{
		std::cout << "time = " << m_Duration.count() * 1000 << std::endl;

		ImGui::BeginDisabled();
		ImGui::Checkbox(m_EnbaleThreadsText.data(), &m_ThreadsEnbaled);
		ImGui::EndDisabled();

		if (ImGui::Button(m_StopButtonText.data(), m_ButtonSize))
		{
			m_HasStarted = false;
			m_ThreadPool.Stop();
			std::cout << "\n\nSTOPPED\n\n";
		}

		ImGui::Text(m_TimerText.data(), m_Duration.count() * 1000);
	}
	else
	{
		ImGui::Checkbox(m_EnbaleThreadsText.data(), &m_ThreadsEnbaled);
		if (ImGui::Button(m_StartButtonText.data(), m_ButtonSize))
		{
			m_HasStarted = true;
			std::cout << "\n\nSTARTED\n\n";
		}
	}

	ImGui::End();
}

void Grid::RandomlyChangeGrid()
{
	uint32_t randomSeed = (uint32_t)std::chrono::duration<float>(std::chrono::steady_clock::now().time_since_epoch()).count();
	int8_t direction = Random::RandomInRange(-1, 1);
	uint16_t rowIndex = 0;

	for (Cell& cell : m_Cells)
	{
		if (rowIndex % m_GridSize == 0)
		{
			direction = Random::RandomInRange(-1, 1);
		}

		randomSeed += rowIndex;
		RandomlyChangeCell(cell, randomSeed, direction);
		rowIndex++;
	}
}

void Grid::RandomlyChangeGridWithThreads()
{
	for (uint16_t rowIndex = 0; rowIndex < m_GridSize; rowIndex++)
	{
		m_ThreadPool.AddTask([this, rowIndex] { this->RandomlyChangeRow(rowIndex); });
	}
}

void Grid::RandomlyChangeRow(const uint16_t row)
{
	uint32_t randomSeed = (uint32_t)std::chrono::duration<float>(std::chrono::steady_clock::now().time_since_epoch()).count();
	const uint16_t startIndex = row * m_GridSize;
	const int8_t direction = Random::RandomInRange(-1, 1);

	for (uint16_t column = 0; column < m_GridSize; column++)
	{
		randomSeed = startIndex + column;
		RandomlyChangeCell(m_Cells[startIndex + column], randomSeed, direction);
	}
}

void Grid::RandomlyChangeCell(Cell& cell, uint32_t& randomSeed, const int8_t direction)
{
	cell.m_TopLeft.x += Cell::positionChange * direction;
	cell.m_BottomRight.x += Cell::positionChange * direction;
	cell.m_Center.x += Cell::positionChange * direction;

	randomSeed += static_cast<uint32_t>(cell.m_Center.x + cell.m_Center.y);
	ImVec4 randomColour = ImGui::ColorConvertU32ToFloat4(Random::RandomNumber(randomSeed));
	randomColour.x = glm::clamp(randomColour.x, Cell::minColour.x, Cell::maxColour.x);
	randomColour.y = glm::clamp(randomColour.y, Cell::minColour.y, Cell::maxColour.y);
	randomColour.z = glm::clamp(randomColour.z, Cell::minColour.z, Cell::maxColour.z);
	cell.m_Colour = ImGui::GetColorU32(randomColour) + Random::RandomInRange((int16_t)(-Cell::colourChange), Cell::colourChange);
}