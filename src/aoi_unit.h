#ifndef __AOI_UNIT_H__
#define __AOI_UNIT_H__

#include "../include/aoi_define.h"

#include <map>
#include <ostream>

namespace FXAOI
{
	class AOICoordinate
	{
	public:
		AOICoordinate(unsigned char dwX
#if AOI_USE_Y_AXIS
			, unsigned char dwY
#endif
			, unsigned char dwZ)
			: m_dwCoordinate(0)
			{
				m_dwX = dwX;
#if AOI_USE_Y_AXIS
				m_dwY = dwY;
#endif
				m_dwZ = dwZ;
			}

		unsigned char GetX()const{return this->m_dwX;}
#if AOI_USE_Y_AXIS
		unsigned char GetY()const{return this->m_dwY;}
#endif
		unsigned char GetZ()const{return this->m_dwZ;}

#if AOI_USE_Y_AXIS
		unsigned int
#else
		unsigned short
#endif
					GetCoordinate() const {return this->m_dwCoordinate;}

		void Debug(std::ostream& refOstream);

	protected:
	private:
		union
		{
			struct
			{
				unsigned char m_dwX;
#if AOI_USE_Y_AXIS
				unsigned char m_dwY;
#endif
				unsigned char m_dwZ;
			};
#if AOI_USE_Y_AXIS
			unsigned int m_dwCoordinate;
#else
			unsigned short m_dwCoordinate;
#endif
		};
	};

// 	class AOICoordinateLess
// 	{
// 	public:
// 		bool operator () (const AOICoordinate& l, const AOICoordinate& r) const
// 		{
// 			if (l.m_dwX == r.m_dwX)
// 			{
// #if AOI_USE_Y_AXIS
// 				if(l.m_dwY == r.m_dwY)
// 					return l.m_dwZ < r.m_dwZ;
// 				return l.m_dwY < r.m_dwY;
// #else
// 				return l.m_dwZ < r.m_dwZ;
// #endif
// 			}
// 			return l.m_dwX < r.m_dwX;
// 		}
// 	};

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

		const static AOIUnits& Instance() {return s_oInstance;}

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

		// typedef std::map<AOICoordinate
		// 	, AOI_UNIT_SUB_SCRIPT
		// 	, AOICoordinateLess> AOIUnitMap;

			// AOIUnitMap m_mapUnits;
#if AOI_USE_Y_AXIS
			AOI_UNIT_SUB_SCRIPT m_arrUnits[ 1 << AOI_MAX_DIVIDE_NUM << AOI_MAX_DIVIDE_NUM << AOI_MAX_DIVIDE_NUM];
#else
			AOI_UNIT_SUB_SCRIPT m_arrUnits[1 << AOI_MAX_DIVIDE_NUM << AOI_MAX_DIVIDE_NUM];
#endif

			static AOIUnits s_oInstance;
	};

}

#endif	//! __AOI_UNIT_H__