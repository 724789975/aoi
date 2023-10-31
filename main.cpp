#include "include/aoi_system.h"
#include "include/aoi_define.h"

#include "include/arr_map.h"
#include "include/arr_set.h"
#include <iostream>
#include <bitset>
#include <map>
#include <set>

#include <chrono>

void AoiOperatorDefault (NODE_ID lNodeId
	, FXAOI::AOIMap<unsigned int, FXAOI::AOISet<NODE_ID> >& mapAddWatching
	, FXAOI::AOIMap<unsigned int, FXAOI::AOISet<NODE_ID> >& mapDelWatching
	, FXAOI::AOIMap<unsigned int, FXAOI::AOISet<NODE_ID> >& mapAddWatched
	, FXAOI::AOIMap<unsigned int, FXAOI::AOISet<NODE_ID> >& mapDelWatched
)
{
	return;
	FXAOI::AOISystem::Instance().DebugNode(lNodeId, std::cout);
	std::cout << "\n";
	if (mapAddWatching.size())
	{
		std::cout << " add_watching:{";
		for (auto &&i : mapAddWatching)
		{
			std::cout << "type_" << i.first << " : {";
			for (auto &&j : i.second)
			{
				std::cout << j << ",";
			}
			std::cout << "},";
		}
		std::cout << "}\n";
	}
	
	if (mapDelWatching.size())
	{
		std::cout << " del_watching:{";
		for (auto &&i : mapDelWatching)
		{
			std::cout << "type_" << i.first << " : {";
			for (auto &&j : i.second)
			{
				std::cout << j << ",";
			}
			std::cout << "},";
		}
		std::cout << "}\n";
	}
	
	if (mapAddWatched.size())
	{
		std::cout << " add_watched:{";
		for (auto &&i : mapAddWatched)
		{
			std::cout << "type_" << i.first << ":{";
			for (auto &&j : i.second)
			{
				std::cout << j << ",";
			}
			std::cout << "},";
		}
		std::cout << "}\n";
	}
	
	if (mapDelWatched.size())
	{
		std::cout << " del_watched:{";
		for (auto &&i : mapDelWatched)
		{
			std::cout << "type_" << i.first << " : {";
			for (auto &&j : i.second)
			{
				std::cout << j << ",";
			}
			std::cout << "},";
		}
		std::cout << "}\n";
	}
}

int main()
{
	FXAOI::AOISystem::Instance().SetAoiOperator(AoiOperatorDefault);
	FXAOI::AOISystem::Instance().AddMap(1, 1, 512, 512, 512, 0, 0, 0);
	// FXAOI::AOISystem::Instance().AddMap(1, 1, 512, 512, 0, 0);
	FXAOI::AOISystem::Instance().SetAOIVisibilityType(1, 1, FXAOI::AOIVisibilityType::AOIVisibilityType_Mutual_Visibility);
	FXAOI::AOISystem::Instance().SetAOIVisibilityType(1, 2, FXAOI::AOIVisibilityType::AOIVisibilityType_Visible);
	FXAOI::AOISystem::Instance().SetAOIVisibilityType(1, 3, FXAOI::AOIVisibilityType::AOIVisibilityType_Visible);
	FXAOI::AOISystem::Instance().SetAOIVisibilityType(2, 1, FXAOI::AOIVisibilityType::AOIVisibilityType_Visible);
	FXAOI::AOISystem::Instance().SetAOIVisibilityType(2, 2, FXAOI::AOIVisibilityType::AOIVisibilityType_Invisible);
	FXAOI::AOISystem::Instance().SetAOIVisibilityType(2, 3, FXAOI::AOIVisibilityType::AOIVisibilityType_Invisible);
	FXAOI::AOISystem::Instance().SetAOIVisibilityType(3, 1, FXAOI::AOIVisibilityType::AOIVisibilityType_Visible);
	FXAOI::AOISystem::Instance().SetAOIVisibilityType(3, 2, FXAOI::AOIVisibilityType::AOIVisibilityType_Visible);
	FXAOI::AOISystem::Instance().SetAOIVisibilityType(3, 3, FXAOI::AOIVisibilityType::AOIVisibilityType_Visible);
	FXAOI::AOISystem::Instance().SetAOINodeLimit(1, 1, 100);
	// FXAOI::AOIUnits::Instance();//.DebugInfo();

	// FXAOI::MapInstance oMapInstance(0, 0, 0, 0);

	auto utc_start = std::chrono::steady_clock::now();
	for (size_t i_ = 0; i_ < 10; i_++)
	{
		int x = 10, y = 10, z = 10;
		for (size_t i = 0; i < 1024; i++)
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
			//if (1 == i % 5)
			//{
			//	FXAOI::AOISystem::Instance().AddChild(i - 1, i);
			//}
			//else
			{
				FXAOI::AOISystem::Instance().AddNode(i, i % 3 + 1, 1, 1);
				//FXAOI::AOISystem::Instance().AddNode(i, 1, 1, 1);
				FXAOI::NodePosition pos = {0, 0, 0};
				// FXAOI::NodePosition pos = {x, z};
				FXAOI::AOISystem::Instance().EnterMap(i, 1, pos);
			}
			++z;
		}

		x = 11, y = 12, z = 13;
		for (size_t i = 0; i < 1024; i++)
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
			//if (1 != i % 5)
			{
				//FXAOI::NodePosition pos = {512, 512, 512};
				FXAOI::NodePosition pos = {x >> 1, y >> 1, z >> 1};
				// FXAOI::NodePosition pos = {x, z};
				FXAOI::AOISystem::Instance().Move(i, pos);
			}
			++z;
		}

		for (size_t i = 0; i < 1024; i++)
		{
			//if (1 != i % 5)
			{
				FXAOI::NodePosition pos = {0, 0, 0};
				// FXAOI::NodePosition pos = {x, z};
				FXAOI::AOISystem::Instance().Move(i, pos);
			}
			++z;
		}

		x = 51, y = 12, z = 13;
		for (size_t i = 0; i < 1024; i++)
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
			//if (1 != i % 5)
			{
				//FXAOI::NodePosition pos = {512, 512, 512};
				FXAOI::NodePosition pos = { x >> 1, y >> 1, z >> 1 };
				// FXAOI::NodePosition pos = {x, z};
				FXAOI::AOISystem::Instance().Move(i, pos);
			}
			++z;
		}

		for (size_t i = 0; i < 1024; i++)
		{
			//if (1 != i % 5)
			{
				FXAOI::NodePosition pos = { 0, 0, 0 };
				// FXAOI::NodePosition pos = {x, z};
				FXAOI::AOISystem::Instance().Move(i, pos);
			}
			++z;
		}

		for (size_t i = 0; i < 1024; i++)
		{
			//if (1 != i % 5)
			{
				FXAOI::AOISystem::Instance().LeaveMap(i);
			}
		}
	}
	auto utc_end = std::chrono::steady_clock::now();
	auto time_diff = utc_end - utc_start;
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(time_diff);
	std::cout << "Operation cost : " << duration.count() << "microseconds\n";

	return 0;
}