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
			, double dOffsetX
#if AOI_USE_Y_AXIS
			, double dOffsetY
#endif
			, double dOffsetZ
		);

		unsigned int GetLengthViewRadius(){return m_dwLengthViweRadius;}
		unsigned int GetWidthViweRadius(){return m_dwWidthViweRadius;}
#if AOI_USE_Y_AXIS
		unsigned int GetHightViweRadius(){return m_dwHightViweRadius;}
#endif

		AOICoordinate GetAOICoordinate(const NodePosition& refPosition);

	protected:
	private:
		unsigned int m_dwMapId;
		/**
		 * @brief 
		 * 视野半径x 长 如果地图过大 视野半径也会变大
		 */
		unsigned int m_dwLengthViweRadius;
		/**
		 * @brief 
		 * 视野半径z 宽 如果地图过大 视野半径也会变大
		 */
		unsigned int m_dwWidthViweRadius;
#if AOI_USE_Y_AXIS
		/**
		 * @brief 
		 * 视野半径y 高 如果地图过大 视野半径也会变大
		 */
		unsigned int m_dwHightViweRadius;
#endif
		/**
		 * @brief 
		 * 地图左下角坐标
		 */
		double m_dOffsetX;
		/**
		 * @brief 
		 * 地图左下角坐标
		 */
		double m_dOffsetY;
		/**
		 * @brief 
		 * 地图左下角坐标
		 */
		double m_dOffsetZ;
	};
};


#endif  //!__MAP_INFO_H__
