#pragma once
#include "Vector.h"

// Each mod defines these for itself.
class CViewVectors
{
public:
	CViewVectors() {}

	CViewVectors(
		Vector vView,
		Vector vHullMin,
		Vector vHullMax,
		Vector vDuckHullMin,
		Vector vDuckHullMax,
		Vector vDuckView,
		Vector vObsHullMin,
		Vector vObsHullMax,
		Vector vDeadViewHeight)
	{
		m_vView = vView;
		m_vHullMin = vHullMin;
		m_vHullMax = vHullMax;
		m_vDuckHullMin = vDuckHullMin;
		m_vDuckHullMax = vDuckHullMax;
		m_vDuckView = vDuckView;
		m_vObsHullMin = vObsHullMin;
		m_vObsHullMax = vObsHullMax;
		m_vDeadViewHeight = vDeadViewHeight;
	}

	// Height above entity position where the viewer's eye is.
	Vector m_vView;

	Vector m_vHullMin;
	Vector m_vHullMax;

	Vector m_vDuckHullMin;
	Vector m_vDuckHullMax;
	Vector m_vDuckView;

	Vector m_vObsHullMin;
	Vector m_vObsHullMax;

	Vector m_vDeadViewHeight;
};

