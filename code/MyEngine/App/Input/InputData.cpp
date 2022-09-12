#include "InputData.h"

#include "InputReader.h"
#include "InputWriter.h"

MyEngine::App::Input::IInputWriter* MyEngine::App::Input::InputData::CreateWriter()
{
	return new InputWriter(*this);
}

MyEngine::App::Input::IInputReader* MyEngine::App::Input::InputData::CreateReader()
{
	return new InputReader(*this);
}

void MyEngine::App::Input::InputData::Update()
{
	for (size_t i = 0; i < NR_OF_KEYS; i++)
		m_KeyStates[KeyStatesType::Pressed][i] = false;
	for (size_t i = 0; i < NR_OF_KEYS; i++)
		m_KeyStates[KeyStatesType::Released][i] = false;
}
