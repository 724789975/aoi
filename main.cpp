#include "include/aoi_unit.h"
#include "include/aoi_map_instance.h"

#include <iostream>
#include <bitset>

int main()
{
	FXAOI::AOIUnits::Instance();//.DebugInfo();

	FXAOI::MapInstance oMapInstance(0, 0, 0, 0);

	int x = 0, y = 0, z = 0;
	for (size_t i = 0; i < 33776960; i++)
	{
		if (z >= 256)
		{
			++x;
			z = 0;
		}
		if (x >= 256)
		{
			++y;
			x = 0;
		}
		oMapInstance.Enter(i, FXAOI::AOICoordinate(x, y, z), 1, 1);
		// oMapInstance.Enter(i, FXAOI::AOICoordinate(x, z), 1, 1);
		++z;
	}
	
	// oMapInstance.Enter()

	return 0;
}