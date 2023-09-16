#ifndef __AOI_MAP_MGR_H__
#define __AOI_MAP_MGR_H__

#include "aoi_define.h"
#include "aoi_map_instance.h"

#include <unordered_map>

namespace FXAOI
{
	class AOIMapInstanceMgr
	{
	public:
		static AOIMapInstanceMgr& Instance() {return s_oInstance;}

		MapInstance* AddMap(unsigned int dwMapId);

		MapInstance* GetMap(unsigned int dwMapId);
	protected:
	private:
		static AOIMapInstanceMgr s_oInstance;
		std::unordered_map<NODE_ID, MapInstance> m_mapMaps;
	};
} // namespace FXAOI


#endif