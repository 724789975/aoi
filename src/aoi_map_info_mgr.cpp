#include "aoi_map_info_mgr.h"

namespace FXAOI
{
	AOIMapInfoMgr AOIMapInfoMgr::s_oInstance;

	MapInfo * AOIMapInfoMgr::AddMap(unsigned int dwMapId, unsigned int dwViweRadius, unsigned int dwLength, unsigned int dwWidth
#if AOI_USE_Y_AXIS
			, unsigned int dwHight
#endif
			, double dOffsetX
#if AOI_USE_Y_AXIS
			, double dOffsetY
#endif
			, double dOffsetZ
		)
	{
		return &(this->m_mapMaps[dwMapId] = MapInfo(dwMapId, dwViweRadius, dwLength, dwWidth
#if AOI_USE_Y_AXIS
			, dwHight
#endif
			, dOffsetX
#if AOI_USE_Y_AXIS
			, dOffsetY
#endif
			, dOffsetZ
		));
	}

	MapInfo *AOIMapInfoMgr::GetMap(unsigned int dwMapId)
	{
		std::unordered_map<unsigned int, MapInfo>::iterator it = this->m_mapMaps.find(dwMapId);
		if (this->m_mapMaps.end() == it)
		{
			return 0;
		}

		return &it->second;
	}
} // namespace FXAOI
