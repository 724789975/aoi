#include "aoi_map_mgr.h"

namespace FXAOI
{
	AOIMapMgr AOIMapMgr::s_oInstace;

	MapInstance *FXAOI::AOIMapMgr::AddMap(unsigned int dwMapId)
	{
		return &(this->m_mapMaps[dwMapId] = MapInstance(dwMapId, 0, 0, 0));
	}

	MapInstance *AOIMapMgr::GetMap(unsigned int dwMapId)
	{
		std::unordered_map<NODE_ID, MapInstance>::iterator it = this->m_mapMaps.find(dwMapId);
		if (this->m_mapMaps.end() != it)
		{
			return 0;
		}

		return &it->second;
	}
} // namespace FXAOI
