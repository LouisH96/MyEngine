#include "pch.h"
#include "InputReader.h"

#include "InputData.h"

MyEngine::App::Input::InputReader::InputReader(InputData& data)
	: m_Data(data)
{
}

bool MyEngine::App::Input::InputReader::IsKeyPressed(char character)
{
	return m_Data.m_KeyStates[InputData::KeyStatesType::Pressed][static_cast<unsigned char>(character)];
}

bool MyEngine::App::Input::InputReader::IsKeyReleased(char character)
{
	return m_Data.m_KeyStates[InputData::KeyStatesType::Released][static_cast<unsigned char>(character)];
}

bool MyEngine::App::Input::InputReader::IsKeyDown(char character)
{
	return m_Data.m_KeyStates[InputData::KeyStatesType::Down][static_cast<unsigned char>(character)];
}
