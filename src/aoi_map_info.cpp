#include "aoi_map_info.h"
#include "../include/aoi_system.h"

namespace FXAOI
{
	MapInfo::MapInfo()
		: m_dwMapId(-1)
		, m_dwLengthViweRadius(-1)
		, m_dwWidthViweRadius(-1)
#if AOI_USE_Y_AXIS
		, m_dwHightViweRadius(-1)
#endif
	{
	}

	MapInfo::MapInfo(unsigned int dwMapId, unsigned int dwViweRadius, unsigned int dwLength, unsigned int dwWidth
#if AOI_USE_Y_AXIS
		, unsigned int dwHight
#endif
		, double dOffsetX
#if AOI_USE_Y_AXIS
		, double dOffsetY
#endif
		, double dOffsetZ
	)
		: m_dwMapId(dwMapId)
		, m_dwLengthViweRadius(dwViweRadius)
		, m_dwWidthViweRadius(dwViweRadius)
#if AOI_USE_Y_AXIS
		, m_dwHightViweRadius(dwViweRadius)
#endif
		, m_dOffsetX(dOffsetX)
#if AOI_USE_Y_AXIS
		, m_dOffsetY(dOffsetY)
#endif
		, m_dOffsetZ(dOffsetZ)
	{
		if((m_dwLengthViweRadius << AOI_MAX_DIVIDE_NUM) < dwLength)
		{
			m_dwLengthViweRadius = (dwLength >> AOI_MAX_DIVIDE_NUM) + 1;
		}
		if((m_dwWidthViweRadius << AOI_MAX_DIVIDE_NUM) < dwWidth)
		{
			m_dwWidthViweRadius = (dwWidth >> AOI_MAX_DIVIDE_NUM) + 1;
		}
#if AOI_USE_Y_AXIS
		if((m_dwHightViweRadius << AOI_MAX_DIVIDE_NUM) < dwHight)
		{
			m_dwHightViweRadius = (dwHight >> AOI_MAX_DIVIDE_NUM) + 1;
		}
#endif
	}

	AOICoordinate MapInfo::GetAOICoordinate(const NodePosition &refPosition)
	{
		return AOICoordinate(unsigned int(refPosition.x - m_dOffsetX) / m_dwLengthViweRadius
#if AOI_USE_Y_AXIS
			, unsigned int(refPosition.y - m_dOffsetY) / m_dwHightViweRadius
#endif
			, unsigned int(refPosition.z - m_dOffsetZ) / m_dwWidthViweRadius
		);
	}
};


