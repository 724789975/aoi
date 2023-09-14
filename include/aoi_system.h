#ifndef __AOI_SYSTEM_H__
#define __AOI_SYSTEM_H__

#include "aoi_define.h"

namespace FXAOI
{
	class AOISystem
	{
	public:
		bool AddMap(unsigned int dwMapId, unsigned int dwViweRadius, unsigned int dwLength, unsigned int dwWidth
#if AOI_USE_Y_AXIS
			, unsigned int dwHight
#endif
		);
	protected:
	private:

	};
} // namespace FXAOI


#endif