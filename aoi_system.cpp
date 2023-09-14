#include "include/aoi_system.h"
#include "aoi_map_info_mgr.h"
#include "aoi_map_instance_mgr.h"

namespace FXAOI
{
	bool AOISystem::AddMap(unsigned int dwMapId, unsigned int dwViweRadius, unsigned int dwLength, unsigned int dwWidth
#if AOI_USE_Y_AXIS
			, unsigned int dwHight
#endif
		)
	{
		AOIMapInfoMgr::Instance().AddMap(dwMapId, dwViweRadius, dwLength, dwWidth
#if AOI_USE_Y_AXIS
			, dwHight
#endif
		);

		AOIMapInstanceMgr::Instance().AddMap(dwMapId);
		
		return true;
	}
} // namespace FXAOI
