#include "aoi_map_instance_mgr.h"

#include <assert.h>

namespace FXAOI
{
	AOIMapInstanceMgr AOIMapInstanceMgr::s_oInstance;

	MapInstance * AOIMapInstanceMgr::AddMap(unsigned int dwMapId)
	{
		assert(this->m_mapMaps.end() == this->m_mapMaps.find(dwMapId));
		return &(this->m_mapMaps[dwMapId] = MapInstance(dwMapId, 0, 0, &this->m_mapMaps[dwMapId]));
	}

	MapInstance *AOIMapInstanceMgr::GetMap(unsigned int dwMapId)
	{
		std::map<NODE_ID, MapInstance>::iterator it = this->m_mapMaps.find(dwMapId);
		if (this->m_mapMaps.end() == it)
		{
			return 0;
		}

		return &it->second;
	}
} // namespace FXAOI
