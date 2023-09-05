#ifndef __AOI_UNIT_H__
#define __AOI_UNIT_H__

#include "include/aoi_define.h"

#include <map>

namespace FXAOI
{
	class AOIUnits
	{
	public:
		AOIUnits()
		{
			Divide(0, 0
#ifdef USE_Y_AXIS
				, 0
#endif
				, 0, 0);
		}
		class AOICoordinateLess;
		class AOICoordinate
		{
		public:
			friend class AOICoordinateLess;
			AOICoordinate(unsigned int dwX
#ifdef USE_Y_AXIS
				, unsigned int dwY
#endif
				, unsigned int dwZ)
				: m_dwX(dwX)
#ifdef USE_Y_AXIS
				, m_dwY(dwY)
#endif
				, m_dwZ(dwZ)
				{}

		protected:
		private:
			unsigned int m_dwX;
#ifdef USE_Y_AXIS
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
#ifdef USE_Y_AXIS
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

		void DebugInfo()const;
	protected:
	private:
		void Divide(unsigned int dwX
#ifdef USE_Y_AXIS
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
	};

	static AOIUnits g_AoiUnits;
}

#endif	//! __AOI_UNIT_H__