#include "HheaTable.h"

#include <iostream>

#include "Io/Binary/BigBinReader.h"

using namespace MyEngine::Io::Ttf;

void HheaTable::Read(Bin::BigBinReader& reader)
{
	reader.Read(Version);
	reader.Read(Ascent);
	reader.Read(Descent);
	reader.Read(LineGap);
	reader.Read(AdvanceWidthMax);
	reader.Read(MinLeftSideBearing);
	reader.Read(MinRightSideBearing);
	reader.Read(XMaxExtent);
	reader.Read(CaretSlopeRise);
	reader.Read(CaretSlopeRun);
	reader.Read(CaretOffset);
	reader.Read(Reserved[0]);
	reader.Read(Reserved[1]);
	reader.Read(Reserved[2]);
	reader.Read(Reserved[3]);
	reader.Read(MetricDataFormat);
	reader.Read(NumOfLongHorMetrics);
}

void HheaTable::Print() const
{
	std::cout << "HheaTable:\n";
	std::cout << " Version: " << Version << std::endl;
	std::cout << " Ascent: " << Ascent << std::endl;
	std::cout << " Descent: " << Descent << std::endl;
	std::cout << " NumOfLongHorMetrics: " << NumOfLongHorMetrics << std::endl;
}
