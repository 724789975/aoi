#ifndef __AOI_MAP_MGR_H__
#define __AOI_MAP_MGR_H__

#include "../include/aoi_define.h"
#include "aoi_map_instance.h"

#include "arr_map.h"
#include <map>

namespace FXAOI
{
	class AOIMapInstanceMgr
	{
	public:
		static AOIMapInstanceMgr& Instance() {return s_oInstance;}

		MapInstance* AddMap(unsigned int dwMapId);

		void RemoveMap(unsigned int dwMapId);

		MapInstance* GetMap(unsigned int dwMapId);
	protected:
	private:
		static AOIMapInstanceMgr s_oInstance;
		ArrMap<NODE_ID, MapInstance*> m_mapMaps;
	};
} // namespace FXAOI


#endif