#include "AsciiFbxReader.h"

#include "Io/Ascii/AsciiReader.h"
#include "Logger/Logger.h"
#include "Properties/FbxPropArray.h"
#include "Properties/FbxPropPrimitive.h"
#include "Properties/FbxPropString.h"

using namespace MyEngine::Io::Fbx::Reading;

FbxFile AsciiFbxReader::Read(std::istream& stream)
{
	//create output
	FbxFile file{ false };
	file.GetRoot().SetName("Root");

	//read
	AsciiReader reader{ stream };

	while (!stream.eof())
	{
		SkipUseless(reader);
		ReadElement(reader, file.GetRoot().CreateChild());
	}

	return file;
}

void AsciiFbxReader::SkipUseless(AsciiReader& reader)
{
	char next;
	while (reader.GetChar(next))
	{
		if (next == ';')
			reader.IgnoreLine();
		else if (next != ' ' && next != '\t' && next != '\n')
		{
			reader.Move(-1);
			return;
		}
	}
}

void AsciiFbxReader::ReadElement(AsciiReader& reader, FbxElement& element)
{
	element.SetName(reader.GetUntil(':'));
	//Logger::Print("New element", element.GetName());

	ReadProperties(reader, element);
	ReadChildren(reader, element);
}

void AsciiFbxReader::ReadChildren(AsciiReader& reader, FbxElement& parent)
{
	if (reader.GetChar() != '{')
	{
		reader.Move(-1);
		return;
	}
	SkipUseless(reader);

	while (reader.GetChar() != '}')
	{
		reader.Move(-1);
		ReadElement(reader, parent.CreateChild());
	}
	SkipUseless(reader);
}

void AsciiFbxReader::ReadProperties(AsciiReader& reader, FbxElement& element)
{
	SkipUseless(reader);

	char next;
	while (reader.GetChar(next))
	{
		switch (next)
		{
		case '{':
			reader.Move(-1);
			return;
		default:
			reader.Move(-1);
			element.AddProperty(ReadNextProperty(reader));
			SkipUseless(reader);
			if (reader.GetChar() != ',')
			{
				reader.Move(-1);
				return;
			}
			break;
		}
	}
}

FbxProperty* AsciiFbxReader::ReadNextProperty(AsciiReader& reader)
{
	SkipUseless(reader);
	switch (reader.DetectedValueType())
	{
	case AsciiReader::Number: return ReadNumberProperty(reader);
	case AsciiReader::String: return ReadStringProperty(reader);
	default:
		if (reader.PeekChar() == '*')
			return ReadArrayProperty(reader);
		if (reader.PeekChar() == 'T')
		{
			reader.Ignore(1);
			return new FbxPropString("T");
		}
		if (reader.PeekChar() == 'Y')
		{
			reader.Ignore(1);
			return new FbxPropString("Y");
		}
		Logger::PrintError("[AsciiFbxReader::ReadProperties] unexpected character: " + ToString::Convert(reader.PeekChar()));
		return nullptr;
	}
}

FbxProperty* AsciiFbxReader::ReadNumberProperty(AsciiReader& reader)
{
	return new FbxPropPrimitive<double>(reader.GetDouble());
}

FbxProperty* AsciiFbxReader::ReadStringProperty(AsciiReader& reader)
{
	return new FbxPropString(reader.GetString());
}

FbxProperty* AsciiFbxReader::ReadArrayProperty(AsciiReader& reader)
{
	FbxPropArray<double>* pProp{new FbxPropArray<double>()};
	Array<double>& a{pProp->GetValues()};

	reader.Ignore(1);
	a.EnsureSize(reader.GetInteger());
	reader.IgnoreUntil(':');

	for (unsigned i = 0; i < a.GetSizeU(); i++)
	{
		SkipUseless(reader);
		a[i] = reader.GetDouble();
		SkipUseless(reader);
		reader.Ignore(1);
	}

	return pProp;
}
