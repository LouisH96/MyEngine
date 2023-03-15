#pragma once

namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
		{
			class FbxProperty
			{
			public:
				FbxProperty() = default;
				virtual ~FbxProperty() = default;

				virtual void Print(int nrTabs) = 0;
				static FbxProperty* Read(std::istream& stream);

			protected:
				void BeginPrint(int nrTabs);
			};
		}
	}
}
