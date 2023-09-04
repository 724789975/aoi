#ifndef __AOI_UNIT_H__
#define __AOI_UNIT_H__

#include <map>

namespace FXAOI
{
	class AOIUnits
	{
	public:
		AOIUnits(){Divide(0, 0, 0, 0);}
		class AOICoordinateLess;
		class AOICoordinate
		{
		public:
			friend class AOICoordinateLess;
			AOICoordinate(unsigned int dwX, unsigned int dwZ)
				: m_dwX(dwX)
				, m_dwZ(dwZ)
				{}

		protected:
		private:
			unsigned int m_dwX;
			unsigned int m_dwZ;
		};

		class AOICoordinateLess
		{
		public:
			bool operator () (const AOICoordinate& l, const AOICoordinate& r) const
			{
				if (l.m_dwX == r.m_dwX) {return l.m_dwZ < r.m_dwZ;}
				return l.m_dwX < r.m_dwX;
			}
		};

		void DebugInfo()const;
	protected:
	private:
		void Divide(unsigned int dwX, unsigned int dwZ, unsigned int dwDivideNum, unsigned dwPosIdx);

		std::map<AOICoordinate, unsigned int, AOICoordinateLess> m_mapUnits;
	};

	static AOIUnits g_AoiUnits;
}

#endif	//! __AOI_UNIT_H__