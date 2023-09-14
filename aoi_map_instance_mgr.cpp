#include "aoi_map_instance_mgr.h"

namespace FXAOI
{
	AOIMapInstanceMgr AOIMapInstanceMgr::s_oInstace;

	MapInstance * AOIMapInstanceMgr::AddMap(unsigned int dwMapId)
	{
		return &(this->m_mapMaps[dwMapId] = MapInstance(dwMapId, 0, 0, 0));
	}

	MapInstance *AOIMapInstanceMgr::GetMap(unsigned int dwMapId)
	{
		std::unordered_map<NODE_ID, MapInstance>::iterator it = this->m_mapMaps.find(dwMapId);
		if (this->m_mapMaps.end() != it)
		{
			return 0;
		}

		return &it->second;
	}
} // namespace FXAOI
