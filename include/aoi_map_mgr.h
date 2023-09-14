#ifndef __AOI_NODE_MGR_H__
#define __AOI_NODE_MGR_H__

#include "aoi_define.h"
#include "aoi_map_instance.h"

#include <unordered_map>

namespace FXAOI
{
	class AOIMapMgr
	{
	public:
	static AOIMapMgr& Instance() {return s_oInstace;}

	MapInstance* AddMap(unsigned int dwMapId);

	MapInstance* GetMap(unsigned int dwMapId);
	protected:
	private:
		static AOIMapMgr s_oInstace;
		std::unordered_map<NODE_ID, MapInstance> m_mapMaps;
	};
} // namespace FXAOI


#endif