#ifndef __MAP_INFO_H__
#define __MAP_INFO_H__

#include "include/aoi_define.h"
#include "aoi_unit.h"

namespace FXAOI
{
	class MapInfo
	{
	public:
		MapInfo();
		MapInfo(unsigned int dwMapId, unsigned int dwViweRadius, unsigned int dwLength, unsigned int dwWidth
#if AOI_USE_Y_AXIS
			, unsigned int dwHight
#endif
		);

		unsigned int GetLengthViewRadius(){return m_dwLengthViweRadius;}
		unsigned int GetWidthViweRadius(){return m_dwWidthViweRadius;}
#if AOI_USE_Y_AXIS
		unsigned int GetHightViweRadius(){return m_dwHightViweRadius;}
#endif

	protected:
	private:
		unsigned int m_dwMapId;
		/**
		 * @brief 
		 * 视野半径 长 如果地图过大 视野半径也会变大
		 */
		unsigned int m_dwLengthViweRadius;
		/**
		 * @brief 
		 * 视野半径 宽 如果地图过大 视野半径也会变大
		 */
		unsigned int m_dwWidthViweRadius;
#if AOI_USE_Y_AXIS
		/**
		 * @brief 
		 * 视野半径 高 如果地图过大 视野半径也会变大
		 */
		unsigned int m_dwHightViweRadius;
#endif
	};
};


#endif  //!__MAP_INFO_H__
