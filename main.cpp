#include "include/aoi_system.h"

#include <iostream>
#include <bitset>

int main()
{
	FXAOI::AOISystem::Instance().AddMap(1, 1, 512, 512, 512, 0, 0, 0);
	FXAOI::SetAOIVisibilityType(1, 1, FXAOI::AOIVisibilityType::AOIVisibilityType_Mutual_Visibility);
	FXAOI::SetAOIVisibilityType(1, 2, FXAOI::AOIVisibilityType::AOIVisibilityType_Visible);
	FXAOI::SetAOIVisibilityType(1, 3, FXAOI::AOIVisibilityType::AOIVisibilityType_Visible);
	FXAOI::SetAOIVisibilityType(2, 1, FXAOI::AOIVisibilityType::AOIVisibilityType_Visible);
	FXAOI::SetAOIVisibilityType(2, 2, FXAOI::AOIVisibilityType::AOIVisibilityType_Invisible);
	FXAOI::SetAOIVisibilityType(2, 3, FXAOI::AOIVisibilityType::AOIVisibilityType_Invisible);
	FXAOI::SetAOIVisibilityType(3, 1, FXAOI::AOIVisibilityType::AOIVisibilityType_Visible);
	FXAOI::SetAOIVisibilityType(3, 2, FXAOI::AOIVisibilityType::AOIVisibilityType_Visible);
	FXAOI::SetAOIVisibilityType(3, 3, FXAOI::AOIVisibilityType::AOIVisibilityType_Visible);
	// FXAOI::AOIUnits::Instance();//.DebugInfo();

	// FXAOI::MapInstance oMapInstance(0, 0, 0, 0);

	int x = 0, y = 0, z = 0;
	for (size_t i = 0; i < 1376560; i++)
	{
		if (z >= 512)
		{
			++x;
			z = 0;
		}
		if (x >= 512)
		{
			++y;
			x = 0;
		}
		FXAOI::AOISystem::Instance().AddNode(i, i % 3 + 1, 1, 1);
		FXAOI::NodePosition pos = {x, y, z};
		FXAOI::AOISystem::Instance().EnterMap(i, 1, pos);
		// oMapInstance.Enter(i, FXAOI::AOICoordinate(x, y, z), 1, 1);
		// oMapInstance.Enter(i, FXAOI::AOICoordinate(x, z), 1, 1);
		++z;
	}

	// x = 0, y = 0, z = 0;
	// for (size_t i = 0; i < 1376560; i++)
	// {
	// 	if (z >= 256)
	// 	{
	// 		++x;
	// 		z = 0;
	// 	}
	// 	if (x >= 256)
	// 	{
	// 		++y;
	// 		x = 0;
	// 	}
	// 	oMapInstance.Leave(i, FXAOI::AOICoordinate(x, y, z), 1, 1);
	// 	// oMapInstance.Leave(i, FXAOI::AOICoordinate(x, z), 1, 1);
	// 	++z;
	// }
	
	// oMapInstance.Enter()

	return 0;
}