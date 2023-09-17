#include "aoi_unit.h"
#include "../include/aoi_define.h"

#include <iostream>
#include <assert.h>

namespace FXAOI
{
	AOIUnits AOIUnits::s_oInstance;

	void AOIUnits::Divide(unsigned int dwX
#if AOI_USE_Y_AXIS
		, unsigned int dwY
#endif
		, unsigned int dwZ
		, unsigned int dwDivideNum
		, AOI_UNIT_SUB_SCRIPT lPosIdx
		)
	{
		//最后一次分裂
		if (AOI_MAX_DIVIDE_NUM - 1 == dwDivideNum)
		{
#if AOI_USE_Y_AXIS
			//左下底
			m_mapUnits[AOICoordinate(dwX, dwY, dwZ)]				= (lPosIdx << AOI_BIT_OFFSET | AOI_LEFT_DOWN_BOTTOM);
			//左上底
			m_mapUnits[AOICoordinate(dwX, dwY, dwZ + 1)]			= (lPosIdx << AOI_BIT_OFFSET | AOI_LEFT_UP_BOTTOM);
			//右下底
			m_mapUnits[AOICoordinate(dwX + 1, dwY, dwZ)]			= (lPosIdx << AOI_BIT_OFFSET | AOI_RIGHT_DOWN_BOTTOM);
			//右上底
			m_mapUnits[AOICoordinate(dwX + 1, dwY, dwZ + 1)]		= (lPosIdx << AOI_BIT_OFFSET | AOI_RIGHT_UP_BOTTOM);
			//左下顶
			m_mapUnits[AOICoordinate(dwX, dwY + 1, dwZ)]			= (lPosIdx << AOI_BIT_OFFSET | AOI_LEFT_DOWN_TOP);
			//左上顶
			m_mapUnits[AOICoordinate(dwX, dwY + 1, dwZ + 1)]		= (lPosIdx << AOI_BIT_OFFSET | AOI_LEFT_UP_TOP);
			//右下顶
			m_mapUnits[AOICoordinate(dwX + 1, dwY + 1, dwZ)]		= (lPosIdx << AOI_BIT_OFFSET | AOI_RIGHT_DOWN_TOP);
			//右上顶
			m_mapUnits[AOICoordinate(dwX + 1, dwY + 1, dwZ + 1)]	= (lPosIdx << AOI_BIT_OFFSET | AOI_RIGHT_UP_TOP);
#else
			//左下
			m_mapUnits[AOICoordinate(dwX, dwZ)]				= (lPosIdx << AOI_BIT_OFFSET | AOI_LEFT_DOWN_BOTTOM);
			//左上
			m_mapUnits[AOICoordinate(dwX, dwZ + 1)]			= (lPosIdx << AOI_BIT_OFFSET | AOI_LEFT_UP_BOTTOM);
			//右下
			m_mapUnits[AOICoordinate(dwX + 1, dwZ)]			= (lPosIdx << AOI_BIT_OFFSET | AOI_RIGHT_DOWN_BOTTOM);
			//右上
			m_mapUnits[AOICoordinate(dwX + 1, dwZ + 1)]		= (lPosIdx << AOI_BIT_OFFSET | AOI_RIGHT_UP_BOTTOM);
#endif
		}
		else
		{
#if AOI_USE_Y_AXIS
			//左下底
			Divide(dwX, dwY, dwZ
				, dwDivideNum + 1, lPosIdx << AOI_BIT_OFFSET | AOI_LEFT_DOWN_BOTTOM);
			//左上底
			Divide(dwX, dwY, dwZ + (1 << (AOI_MAX_DIVIDE_NUM - 1 - dwDivideNum))
				, dwDivideNum + 1, lPosIdx << AOI_BIT_OFFSET | AOI_LEFT_UP_BOTTOM);
			//右下底
			Divide(dwX + (1 << (AOI_MAX_DIVIDE_NUM - 1 - dwDivideNum)), dwY, dwZ
				, dwDivideNum + 1, lPosIdx << AOI_BIT_OFFSET | AOI_RIGHT_DOWN_BOTTOM);
			//右上底
			Divide(dwX + (1 << (AOI_MAX_DIVIDE_NUM - 1 - dwDivideNum)), dwY, dwZ + (1 << (AOI_MAX_DIVIDE_NUM - 1 - dwDivideNum))
				, dwDivideNum + 1, lPosIdx << AOI_BIT_OFFSET | AOI_RIGHT_UP_BOTTOM);
			//左下顶
			Divide(dwX, dwY + (1 << (AOI_MAX_DIVIDE_NUM - 1 - dwDivideNum)), dwZ
				, dwDivideNum + 1, lPosIdx << AOI_BIT_OFFSET | AOI_LEFT_DOWN_TOP);
			//左上顶
			Divide(dwX, dwY + (1 << (AOI_MAX_DIVIDE_NUM - 1 - dwDivideNum)), dwZ + (1 << (AOI_MAX_DIVIDE_NUM - 1 - dwDivideNum))
				, dwDivideNum + 1, lPosIdx << AOI_BIT_OFFSET | AOI_LEFT_UP_TOP);
			//右下顶
			Divide(dwX + (1 << (AOI_MAX_DIVIDE_NUM - 1 - dwDivideNum)), dwY + (1 << (AOI_MAX_DIVIDE_NUM - 1 - dwDivideNum)), dwZ
				, dwDivideNum + 1, lPosIdx << AOI_BIT_OFFSET | AOI_RIGHT_DOWN_TOP);
			//右上顶
			Divide(dwX + (1 << (AOI_MAX_DIVIDE_NUM - 1 - dwDivideNum)), dwY + (1 << (AOI_MAX_DIVIDE_NUM - 1 - dwDivideNum)), dwZ + (1 << (AOI_MAX_DIVIDE_NUM - 1 - dwDivideNum))
				, dwDivideNum + 1, lPosIdx << AOI_BIT_OFFSET | AOI_RIGHT_UP_TOP);
#else
			//左下
			Divide(dwX, dwZ, dwDivideNum + 1, lPosIdx << AOI_BIT_OFFSET | AOI_LEFT_DOWN_BOTTOM);
			//左上
			Divide(dwX, dwZ + (1 << (AOI_MAX_DIVIDE_NUM - 1 - dwDivideNum)), dwDivideNum + 1, lPosIdx << AOI_BIT_OFFSET | AOI_LEFT_UP_BOTTOM);
			//右下
			Divide(dwX + (1 << (AOI_MAX_DIVIDE_NUM - 1 - dwDivideNum)), dwZ, dwDivideNum + 1, lPosIdx << AOI_BIT_OFFSET | AOI_RIGHT_DOWN_BOTTOM);
			//右上
			Divide(dwX + (1 << (AOI_MAX_DIVIDE_NUM - 1 - dwDivideNum)), dwZ + (1 << (AOI_MAX_DIVIDE_NUM - 1 - dwDivideNum)), dwDivideNum + 1, lPosIdx << AOI_BIT_OFFSET | AOI_RIGHT_UP_BOTTOM);
#endif
		}
	}

	bool AOIUnits::GetMapPos(unsigned int x
#if AOI_USE_Y_AXIS
		, unsigned int y
#endif
		, unsigned int z, AOI_UNIT_SUB_SCRIPT& reflPos) const
	{
		return this->GetMapPos(AOICoordinate(x
#if AOI_USE_Y_AXIS
			, y
#endif
			, z), reflPos);
	}

	bool AOIUnits::GetMapPos(const AOICoordinate& refCoordinate, AOI_UNIT_SUB_SCRIPT& reflPos)const
	{
		AOIUnitMap::const_iterator it = this->m_mapUnits.find(refCoordinate);

		if (it == this->m_mapUnits.cend())
		{
			return false;
		}

		reflPos = it->second;
		return true;
	}

	void AOIUnits::DebugInfo()const
	{
#if AOI_USE_Y_AXIS
		for (int y = 0; y < 1 << AOI_MAX_DIVIDE_NUM; ++y)
		{
			for (int z = (1 << AOI_MAX_DIVIDE_NUM) - 1; z >= 0; --z)
			{
				for(unsigned int x = 0; x < 1 << AOI_MAX_DIVIDE_NUM; ++x)
				{
					AOIUnitMap::const_iterator it = this->m_mapUnits.find(AOICoordinate(x, y, z));
					if (it == this->m_mapUnits.cend()){continue;}

					AOI_UNIT_SUB_SCRIPT pos = it->second;
					std::cout << "(" << x << "," << y << "," << z << ")" ;
					std::cout << ((pos & (0xffll << 56)) >> 56) << ",";
					std::cout << ((pos & (0xffll << 48)) >> 48) << ",";
					std::cout << ((pos & (0xffll << 40)) >> 40) << ",";
					std::cout << ((pos & (0xffll << 32)) >> 32) << ",";
					std::cout << ((pos & (0xffll << 24)) >> 24) << ",";
					std::cout << ((pos & (0xffll << 16)) >> 16) << ",";
					std::cout << ((pos & (0xffll << 8)) >> 8) << ",";
					std::cout << ((pos & (0xffll << 0)) >> 0) << ",";
					std::cout << " ";
				}
				std::cout << "\n";
			}
			std::cout << "--------------------------------------------------\n";
		}
#else
		for (int z = (1 << AOI_MAX_DIVIDE_NUM) - 1; z >= 0; --z)
		{
			for(unsigned int x = 0; x < 1 << AOI_MAX_DIVIDE_NUM; ++x)
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
#endif
	}

	void AOICoordinate::Debug(std::ostream& refOstream)
	{
		refOstream << "AOICoordinate:{x:" << this->m_dwX << ","
#if AOI_USE_Y_AXIS
			<< "y:" << this->m_dwY << ","
#endif
			<< "z:" << this->m_dwZ << "}";
	}
}
