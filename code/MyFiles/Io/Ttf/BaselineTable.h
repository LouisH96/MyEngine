#pragma once

namespace MyEngine
{
	namespace Io
	{
		namespace Bin
		{
			class BigBinReader;
		}
	}
}

namespace MyEngine
{
	namespace Io
	{
		namespace Ttf
		{
			class BaselineTable
			{
			public:
				static void Read(Bin::BigBinReader& reader);

			private:
			};
		}
	}
}
