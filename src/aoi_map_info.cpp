#include "aoi_map_info.h"
#include "../include/aoi_system.h"

#include <assert.h>

namespace FXAOI
{
	MapInfo::MapInfo()
		: m_dwMapId(-1)
		, m_dwLengthViweRadius(-1)
		, m_dwWidthViweRadius(-1)
#if AOI_USE_Y_AXIS
		, m_dwHightViweRadius(-1)
#endif
		, m_dOffsetX(0)
#if AOI_USE_Y_AXIS
		, m_dOffsetY(0)
#endif
		, m_dOffsetZ(0)
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
		unsigned int dwX = unsigned int(refPosition.x - m_dOffsetX) / m_dwLengthViweRadius;
		assert(0 == (dwX & AOI_COORDINATE_FLAG_MASK));
#if AOI_USE_Y_AXIS
		unsigned int dwY = unsigned int(refPosition.y - m_dOffsetY) / m_dwHightViweRadius;
		assert(0 == (dwY & AOI_COORDINATE_FLAG_MASK));
#endif
		unsigned int dwZ = unsigned int(refPosition.z - m_dOffsetZ) / m_dwWidthViweRadius;
		assert(0 == (dwZ & AOI_COORDINATE_FLAG_MASK));
		return AOICoordinate((unsigned char)dwX
#if AOI_USE_Y_AXIS
			, (unsigned char)dwY
#endif
			, (unsigned char)dwZ
		);
	}
};


