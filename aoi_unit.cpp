#include "include/aoi_unit.h"
#include "include/aoi_define.h"

#include <iostream>

namespace FXAOI
{
	void AOIUnits::Divide(unsigned int dwX, unsigned int dwZ, unsigned int dwDivideNum, unsigned dwPosIdx)
	{
		if (MAX_DIVIDE_NUM <= dwDivideNum){return;}
		//最后一次分裂
		if (MAX_DIVIDE_NUM - 1 == dwDivideNum)
		{
			//左下
			m_mapUnits[AOICoordinate(dwX, dwZ)] = (dwPosIdx << 4 | 0x1);

			//左上
			m_mapUnits[AOICoordinate(dwX, dwZ + 1)] = (dwPosIdx << 4 | 0x2);
			
			//右下
			m_mapUnits[AOICoordinate(dwX + 1, dwZ)] = (dwPosIdx << 4 | 0x4);
					
			//右上
			m_mapUnits[AOICoordinate(dwX + 1, dwZ + 1)] = (dwPosIdx << 4 | 0x8);
		}
		else
		{
			//左下
			Divide(dwX, dwZ, dwDivideNum + 1, dwPosIdx << 4 | 0x1);
			//左上
			Divide(dwX, dwZ + (1 << (MAX_DIVIDE_NUM - 1 - dwDivideNum)), dwDivideNum + 1, dwPosIdx << 4 | 0x2);
			//右下
			Divide(dwX + (1 << (MAX_DIVIDE_NUM - 1 - dwDivideNum)), dwZ, dwDivideNum + 1, dwPosIdx << 4 | 0x4);
			//右上
			Divide(dwX + (1 << (MAX_DIVIDE_NUM - 1 - dwDivideNum)), dwZ + (1 << (MAX_DIVIDE_NUM - 1 - dwDivideNum)), dwDivideNum + 1, dwPosIdx << 4 | 0x8);
		}
	}

	void AOIUnits::DebugInfo()const
	{
		for (int z = (1 << 8) - 1; z >= 0; --z)
		{
			for(unsigned int x = 0; x < 1 << 8; ++x)
			{
				std::map<AOICoordinate, unsigned int, AOICoordinateLess>::const_iterator it = this->m_mapUnits.find(AOICoordinate(x, z));
				if (it == this->m_mapUnits.cend()){continue;}

				unsigned int pos = it->second;
				std::cout << "(" << x << "," << z << ")" ;
				std::cout << ((pos & (0xf << 28)) >> 28);
				std::cout << ((pos & (0xf << 24)) >> 24);
				std::cout << ((pos & (0xf << 20)) >> 20);
				std::cout << ((pos & (0xf << 16)) >> 16);
				std::cout << ((pos & (0xf << 12)) >> 12);
				std::cout << ((pos & (0xf << 8)) >> 8);
				std::cout << ((pos & (0xf << 4)) >> 4);
				std::cout << ((pos & (0xf << 0)) >> 0);
				std::cout << "\t";
			}
			std::cout << "\n";
		}
	}
}
