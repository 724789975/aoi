#include "include/map_info.h"

namespace AOI
{
	MapInfo::MapInfo(unsigned int dwViweRadius, unsigned int dwLength, unsigned int dwWidth
#if AOI_USE_Y_AXIS
		, unsigned dwHight
#endif
	)
		: m_dwLengthViweRadius(dwViweRadius >> 1)
		, m_dwWidthViweRadius(dwViweRadius >> 1)
#if AOI_USE_Y_AXIS
		, m_dwHightViweRadius(dwViweRadius >> 1)
#endif
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

};


