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
		 * ��Ұ�뾶x �� �����ͼ���� ��Ұ�뾶Ҳ����
		 */
		unsigned int m_dwLengthViweRadius;
		/**
		 * @brief 
		 * ��Ұ�뾶z �� �����ͼ���� ��Ұ�뾶Ҳ����
		 */
		unsigned int m_dwWidthViweRadius;
#if AOI_USE_Y_AXIS
		/**
		 * @brief 
		 * ��Ұ�뾶y �� �����ͼ���� ��Ұ�뾶Ҳ����
		 */
		unsigned int m_dwHightViweRadius;
#endif
		/**
		 * @brief 
		 * ��ͼ���½�����
		 */
		double m_dOffsetX;
		/**
		 * @brief 
		 * ��ͼ���½�����
		 */
		double m_dOffsetY;
		/**
		 * @brief 
		 * ��ͼ���½�����
		 */
		double m_dOffsetZ;
	};
};


#endif  //!__MAP_INFO_H__
