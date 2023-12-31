#ifndef __AOI_MAP_INFO_MGR_H__
#define __AOI_MAP_INFO_MGR_H__

#include "../include/aoi_define.h"
#include "aoi_map_info.h"

#include "arr_map.h"
#include <map>

namespace FXAOI
{
	class AOIMapInfoMgr
	{
	public:
		static AOIMapInfoMgr& Instance() {return s_oInstance;}

		MapInfo* AddMap(unsigned int dwMapId, unsigned int dwViweRadius, unsigned int dwLength, unsigned int dwWidth
#if AOI_USE_Y_AXIS
			, unsigned int dwHight
#endif
  			, double dOffsetX
#if AOI_USE_Y_AXIS
			, double dOffsetY
#endif
			, double dOffsetZ
		);

		MapInfo* GetMap(unsigned int dwMapId);
	protected:
	private:
		static AOIMapInfoMgr s_oInstance;
		AOIMap<unsigned int, MapInfo> m_mapMaps;
	};
} // namespace FXAOI


#endif