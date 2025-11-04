#include "pch.h"
#include "Systems.h"

using namespace MyEngine;

Systems::~Systems()
{
	m_Systems.DeleteAll();
}

void Systems::Add(ISystem* pSystem)
{
	m_Systems.Add(pSystem);
}

void Systems::OnCanvasResized(const App::ResizedEvent& event)
{
	for (unsigned i{ 0 }; i < m_Systems.GetSize(); ++i)
		m_Systems[i]->OnCanvasResized(event);
}

void Systems::EarlyUpdate()
{
	for (unsigned i{ 0 }; i < m_Systems.GetSize(); ++i)
		m_Systems[i]->EarlyUpdate();
}

void Systems::Update()
{
	for (unsigned i{ 0 }; i < m_Systems.GetSize(); ++i)
		m_Systems[i]->Update();
}

void Systems::PreRender()
{
	for (unsigned i{ 0 }; i < m_Systems.GetSize(); ++i)
		m_Systems[i]->PreRender();
}

void Systems::Render()
{
	for (unsigned i{ 0 }; i < m_Systems.GetSize(); ++i)
		m_Systems[i]->Render();
}
