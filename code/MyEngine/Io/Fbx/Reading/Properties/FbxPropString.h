#pragma once
#include <Io/Fbx/Reading/FbxProperty.h>

namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
		{
			namespace Reading
			{
				class FbxPropString
					: public Reading::FbxProperty
				{
				public:
					FbxPropString(std::istream& stream);

					void Print(bool compact, int nrTabs) const override;
					const std::string& GetValue() const { return m_String; }
					std::string& GetValue() { return m_String; }
					static std::string Read(std::istream& stream);
				private:
					std::string m_String;
				};
			}
		}
	}
}
