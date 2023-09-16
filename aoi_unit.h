#ifndef __AOI_UNIT_H__
#define __AOI_UNIT_H__

#include "include/aoi_define.h"

#include <map>

namespace FXAOI
{
	class AOICoordinateLess;
	class AOICoordinate
	{
	public:
		friend class AOICoordinateLess;
		AOICoordinate(unsigned int dwX
#if AOI_USE_Y_AXIS
			, unsigned int dwY
#endif
			, unsigned int dwZ)
			: m_dwX(dwX)
#if AOI_USE_Y_AXIS
			, m_dwY(dwY)
#endif
			, m_dwZ(dwZ)
			{}

		unsigned int GetX()const{return m_dwX;}
#if AOI_USE_Y_AXIS
		unsigned int GetY()const{return m_dwY;}
#endif
		unsigned int GetZ()const{return m_dwZ;}

	protected:
	private:
		unsigned int m_dwX;
#if AOI_USE_Y_AXIS
		unsigned int m_dwY;
#endif
		unsigned int m_dwZ;
	};

	class AOICoordinateLess
	{
	public:
		bool operator () (const AOICoordinate& l, const AOICoordinate& r) const
		{
			if (l.m_dwX == r.m_dwX)
			{
#if AOI_USE_Y_AXIS
				if(l.m_dwY == r.m_dwY)
					return l.m_dwZ < r.m_dwZ;
				return l.m_dwY < r.m_dwY;
#else
				return l.m_dwZ < r.m_dwZ;
#endif
			}
			return l.m_dwX < r.m_dwX;
		}
	};

	class AOIUnits
	{
	public:
		AOIUnits()
		{
			Divide(0, 0
#if AOI_USE_Y_AXIS
				, 0
#endif
				, 0, 0);
		}

		const static AOIUnits& Instance() {return s_oInstace;}

		bool GetMapPos(unsigned int x
#if AOI_USE_Y_AXIS
		, unsigned int y
#endif
		, unsigned int z, AOI_UNIT_SUB_SCRIPT& reflPos) const;

		bool GetMapPos(const AOICoordinate& refCoordinate, AOI_UNIT_SUB_SCRIPT& reflPos)const;

		void DebugInfo()const;
	protected:
	private:
		void Divide(unsigned int dwX
#if AOI_USE_Y_AXIS
			, unsigned int dwY
#endif
			, unsigned int dwZ
			, unsigned int dwDivideNum
			, AOI_UNIT_SUB_SCRIPT lPosIdx
		);

		typedef std::map<AOICoordinate
			, AOI_UNIT_SUB_SCRIPT
			, AOICoordinateLess> AOIUnitMap;

			AOIUnitMap m_mapUnits;

			static AOIUnits s_oInstace;
	};

}

#endif	//! __AOI_UNIT_H__