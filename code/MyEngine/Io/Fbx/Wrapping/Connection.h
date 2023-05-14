#pragma once


namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
		{
			namespace Reading
			{
				class FbxObject;
			}

			namespace Wrapping
			{
				class Model;

				class Connection
				{
				public:
					Connection() = default;
					explicit Connection(Reading::FbxObject& readerObject);

					std::string Relation; //'OO' (object-object) or 'OP' (object-property)
					int64_t Id;
					int64_t ParentId;
					std::string Property; //only if relation is OP
				};
			}
		}
	}
}

