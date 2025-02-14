#pragma once
#include <vector>
#include <memory>
#include <string_view>
#include "Object.h"

struct GLFWwindow;

////////////////////

class App
{
public:
	App();
	App(std::string_view name);
	~App();

	App(const App& app) = delete;
	App& operator=(const App& app) = delete;

	void Run();

	void AddObjectToDraw(std::shared_ptr<IDrawableObject> object) { m_ObjectsToDraw.emplace_back(object); }

	GLFWwindow* GetWindow() { return m_Window; }

	ImVec2 GetWindowSize() { return ImVec2(m_Width, m_Height); }

private:
	void Init(std::string_view name = "");

	bool m_Running = false;

	float m_TimeStep = 0.0f;
	float m_FrameTime = 0.0f;
	float m_LastFrameTime = 0.0f;

	GLFWwindow* m_Window = nullptr;
	std::vector<std::shared_ptr<IDrawableObject>> m_ObjectsToDraw;

	static constexpr int m_Width = 1920;
	static constexpr int m_Height = 1080;
};