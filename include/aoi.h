#ifndef __MAP_INFO_H__
#define __MAP_INFO_H__

#include "aoi_define.h"

/**
 * @brief 
 * 
 */
class MapInfo
{
public:
	MapInfo(unsigned int dwViweRadius, unsigned int dwLength, unsigned int dwWidth, unsigned dwHight)
		: m_dwLengthViweRadius(dwViweRadius)
		, m_dwWidthViweRadius(dwViweRadius)
		, m_dwHightViweRadius(dwViweRadius)
	{
		if(m_dwLengthViweRadius << AOI_MAX_DIVIDE_NUM < dwLength)
		{
			m_dwLengthViweRadius = dwLength >> 8 + 1;
		}
		if(m_dwWidthViweRadius << AOI_MAX_DIVIDE_NUM < dwWidth)
		{
			m_dwWidthViweRadius = dwWidth >> 8 + 1;
		}
		if(m_dwHightViweRadius << AOI_MAX_DIVIDE_NUM < dwHight)
		{
			m_dwHightViweRadius = dwHight >> 8 + 1;
		}
	}

	unsigned int GetLengthViewRadius(){return m_dwLengthViweRadius;}
	unsigned int GetWidthViweRadius(){return m_dwWidthViweRadius;}
	unsigned int GetHightViweRadius(){return m_dwHightViweRadius;}

	//��ֻ����2d??
	unsigned int GetMapPos(unsigned int x, unsigned int y, unsigned int z)
	{
	}

protected:
private:
	/**
	 * @brief 
	 * ��Ұ�뾶 �� �����ͼ���� ��Ұ�뾶Ҳ����
	 */
	unsigned int m_dwLengthViweRadius;
	/**
	 * @brief 
	 * ��Ұ�뾶 �� �����ͼ���� ��Ұ�뾶Ҳ����
	 */
	unsigned int m_dwWidthViweRadius;
	/**
	 * @brief 
	 * ��Ұ�뾶 �� �����ͼ���� ��Ұ�뾶Ҳ����
	 */
	unsigned int m_dwHightViweRadius;
};

#endif  //!__MAP_INFO_H__
