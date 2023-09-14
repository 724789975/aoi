#ifndef __AOI_MAP_INFO_MGR_H__
#define __AOI_MAP_INFO_MGR_H__

#include "aoi_define.h"
#include "aoi_map_info.h"

#include <unordered_map>

namespace FXAOI
{
	class AOIMapMgr
	{
	public:
		static AOIMapMgr& Instance() {return s_oInstace;}

		MapInfo* AddMap(unsigned int dwMapId, unsigned int dwViweRadius, unsigned int dwLength, unsigned int dwWidth
#if AOI_USE_Y_AXIS
			, unsigned int dwHight
#endif
		);

		MapInfo* GetMap(unsigned int dwMapId);
	protected:
	private:
		static AOIMapMgr s_oInstace;
		std::unordered_map<NODE_ID, MapInfo> m_mapMaps;
	};
} // namespace FXAOI


#endif