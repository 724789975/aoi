#include "include/aoi_system.h"
#include "include/aoi_define.h"

#include <iostream>
#include <bitset>
#include <map>
#include <set>

#include <chrono>

void AoiOperatorDefault (NODE_ID lNodeId
	, std::vector<FXAOI::AoiOperatorInfo> vecAddWatching
	, std::vector<FXAOI::AoiOperatorInfo> vecDelWatching
	, std::vector<FXAOI::AoiOperatorInfo> vecAddWatched
	, std::vector<FXAOI::AoiOperatorInfo> vecDelWatched
)
{
	return;
	FXAOI::AOISystem::Instance().DebugNode(lNodeId, std::cout);
	std::cout << "\n";
	if (vecAddWatching.size())
	{
		std::cout << " add_watching:{";
		for (auto &&i : vecAddWatching)
		{
			std::cout << "type_" << i.m_dwType << " : {";
			for (auto &&j : i.m_vecNodes)
			{
				std::cout << j << ",";
			}
			std::cout << "},";
		}
		std::cout << "}\n";
	}
	
	if (vecDelWatching.size())
	{
		std::cout << " del_watching:{";
		for (auto &&i : vecDelWatching)
		{
			std::cout << "type_" << i.m_dwType << " : {";
			for (auto &&j : i.m_vecNodes)
			{
				std::cout << j << ",";
			}
			std::cout << "},";
		}
		std::cout << "}\n";
	}
	
	if (vecAddWatched.size())
	{
		std::cout << " add_watched:{";
		for (auto &&i : vecAddWatched)
		{
			std::cout << "type_" << i.m_dwType << ":{";
			for (auto &&j : i.m_vecNodes)
			{
				std::cout << j << ",";
			}
			std::cout << "},";
		}
		std::cout << "}\n";
	}
	
	if (vecDelWatched.size())
	{
		std::cout << " del_watched:{";
		for (auto &&i : vecDelWatched)
		{
			std::cout << "type_" << i.m_dwType << " : {";
			for (auto &&j : i.m_vecNodes)
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
				FXAOI::AOISystem::Instance().AddNode(i, i % 3 + 1, rand() % 2 + 1, rand() % 2 + 1);
				//FXAOI::AOISystem::Instance().AddNode(i, 1, 1, 1);
				FXAOI::NodePosition pos = {0, 0, 0};
				// FXAOI::NodePosition pos = {x, z};
				FXAOI::AOISystem::Instance().EnterMap(i, 1, pos);
			}
			++z;
		}

		for (int _i = 0; _i < 1000; ++_i)
		{
			for (size_t i = 0; i < 1024; i += 128)
			{
				FXAOI::NodePosition pos = {rand() % 512, rand() % 512, rand() % 512 };
				// FXAOI::NodePosition pos = {x, z};
				for (int j = 0; j < 128; ++j)
				{
					FXAOI::AOISystem::Instance().Move(i + j, pos);
				}
			}
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