#include "Draw.h"

void CImGuiRender::ClearData()
{
	if (!m_draw_data.empty())
		m_draw_data.clear();
}

void CImGuiRender::SwapData()
{
	std::unique_lock<std::shared_mutex> lock{ m_mutex };
	m_draw_data.swap(m_draw_data_safe);
}

void CImGuiRender::AddToRender(const std::function<void()>& fun)
{
	ClearData();
	fun();
	SwapData();
}

