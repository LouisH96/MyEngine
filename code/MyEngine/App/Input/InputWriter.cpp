#include "pch.h"
#include "InputWriter.h"

#include "InputData.h"

MyEngine::App::Input::InputWriter::InputWriter(InputData& inputData)
	: m_Data(inputData)
{
}

void MyEngine::App::Input::InputWriter::OnKeyPressed(char character) const
{
	m_Data.m_KeyStates[InputData::KeyStatesType::Down][static_cast<unsigned char>(character)] = true;
	m_Data.m_KeyStates[InputData::KeyStatesType::Pressed][static_cast<unsigned char>(character)] = true;
}

void MyEngine::App::Input::InputWriter::OnKeyReleased(char character) const
{
	m_Data.m_KeyStates[InputData::KeyStatesType::Down][static_cast<unsigned char>(character)] = false;
	m_Data.m_KeyStates[InputData::KeyStatesType::Released][static_cast<unsigned char>(character)] = true;
}