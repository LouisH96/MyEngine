#include "InputData.h"

#include "InputReader.h"
#include "InputWriter.h"

MyEngine::App::Input::IInputWriter* MyEngine::App::Input::InputData::CreateWriter()
{
	return new InputWriter();
}

MyEngine::App::Input::IInputReader* MyEngine::App::Input::InputData::CreateReader()
{
	return new InputReader();
}
