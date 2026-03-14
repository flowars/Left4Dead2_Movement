#pragma once

#include <any>
#include <deque>
#include <mutex>
#include <functional>
#include <shared_mutex>
#include "Structures.h"

class CImGuiRender {
public:

	// add to surface
	void AddToRender(const std::function<void()>& fun);

private:
	void ClearData();
	void SwapData();

	std::deque<std::unique_ptr<drawing::Draw>> m_draw_data;
	std::deque<std::unique_ptr<drawing::Draw>> m_draw_data_safe;
	std::shared_mutex m_mutex;
};

inline CImGuiRender Render;