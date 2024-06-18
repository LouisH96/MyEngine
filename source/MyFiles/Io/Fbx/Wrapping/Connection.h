#pragma once
#include <string>

namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
		{
			namespace Reading
			{
				class FbxElement;
			}

			namespace Wrapping
			{
				class Model;

				class Connection
				{
				public:
					Connection() = default;
					explicit Connection(Reading::FbxElement& readerObject);

					std::string Relation; //'OO' (object-object) or 'OP' (object-property)
					int64_t ChildId{};
					int64_t ParentId{};
					std::string Property; //only if relation is OP

					void Print(int nrTabs = 0) const;
				};
			}
		}
	}
}
