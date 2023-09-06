#include "include/aoi_map_instance.h"

#include <assert.h>

namespace FXAOI
{
	MapInstance::MapInstance(unsigned int dwMapId, AOI_UNIT_SUB_SCRIPT lSubScript, unsigned int dwDivideNum, MapInstance* pRoot)
		: m_dwMapId(dwMapId)
		, m_lSubScript(lSubScript)
		, m_dwDivideNum(dwDivideNum)
		, m_bDivided(false)
		, m_pRoot(pRoot)
	{}

	MapInstance* MapInstance::GetInstance(const AOICoordinate& refCoordinate)
	{
		AOI_UNIT_SUB_SCRIPT lPos;
		bool bRet = AOIUnits::Instance().GetMapPos(refCoordinate, lPos);
		assert(bRet);

		return this->GetInstance(lPos);
	}

	MapInstance* MapInstance::GetInstance(AOI_UNIT_SUB_SCRIPT lPos)
	{
		if (!this->m_bDivided)
		{
			return this;
		}
		
		unsigned int lChunk = unsigned int((lPos >> ((AOI_MAX_DIVIDE_NUM - 1 - this->m_dwDivideNum) * AOI_BIT_OFFSET)) & AOI_FLAF_MASK);
		assert(this->m_mapDividedMap.find(lChunk) != this->m_mapDividedMap.end());

		return this->m_mapDividedMap[lChunk].GetInstance(lPos);
	}

	void MapInstance::Divide()
	{
		if (AOI_MAX_DIVIDE_NUM <= m_dwDivideNum)
		{
			return;
		}
		assert(0 == this->m_mapDividedMap.size());
		
		this->m_mapDividedMap[AOI_LEFT_DOWN_BOTTOM]		= MapInstance(this->m_dwMapId, this->m_lSubScript | (AOI_LEFT_DOWN_BOTTOM	<< (AOI_MAX_DIVIDE_NUM - m_dwDivideNum - 1) * AOI_BIT_OFFSET), m_dwDivideNum + 1, m_pRoot);
		this->m_mapDividedMap[AOI_LEFT_UP_BOTTOM]		= MapInstance(this->m_dwMapId, this->m_lSubScript | (AOI_LEFT_UP_BOTTOM		<< (AOI_MAX_DIVIDE_NUM - m_dwDivideNum - 1) * AOI_BIT_OFFSET), m_dwDivideNum + 1, m_pRoot);
		this->m_mapDividedMap[AOI_RIGHT_DOWN_BOTTOM]	= MapInstance(this->m_dwMapId, this->m_lSubScript | (AOI_RIGHT_DOWN_BOTTOM	<< (AOI_MAX_DIVIDE_NUM - m_dwDivideNum - 1) * AOI_BIT_OFFSET), m_dwDivideNum + 1, m_pRoot);
		this->m_mapDividedMap[AOI_RIGHT_UP_BOTTOM]		= MapInstance(this->m_dwMapId, this->m_lSubScript | (AOI_RIGHT_UP_BOTTOM	<< (AOI_MAX_DIVIDE_NUM - m_dwDivideNum - 1) * AOI_BIT_OFFSET), m_dwDivideNum + 1, m_pRoot);
#if AOI_USE_Y_AXIS
		this->m_mapDividedMap[AOI_LEFT_DOWN_TOP]		= MapInstance(this->m_dwMapId, this->m_lSubScript | (AOI_LEFT_DOWN_TOP		<< (AOI_MAX_DIVIDE_NUM - m_dwDivideNum - 1) * AOI_BIT_OFFSET), m_dwDivideNum + 1, m_pRoot);
		this->m_mapDividedMap[AOI_LEFT_UP_TOP]			= MapInstance(this->m_dwMapId, this->m_lSubScript | (AOI_LEFT_UP_TOP		<< (AOI_MAX_DIVIDE_NUM - m_dwDivideNum - 1) * AOI_BIT_OFFSET), m_dwDivideNum + 1, m_pRoot);
		this->m_mapDividedMap[AOI_RIGHT_DOWN_TOP]		= MapInstance(this->m_dwMapId, this->m_lSubScript | (AOI_RIGHT_DOWN_TOP		<< (AOI_MAX_DIVIDE_NUM - m_dwDivideNum - 1) * AOI_BIT_OFFSET), m_dwDivideNum + 1, m_pRoot);
		this->m_mapDividedMap[AOI_RIGHT_UP_TOP]			= MapInstance(this->m_dwMapId, this->m_lSubScript | (AOI_RIGHT_UP_TOP		<< (AOI_MAX_DIVIDE_NUM - m_dwDivideNum - 1) * AOI_BIT_OFFSET), m_dwDivideNum + 1, m_pRoot);
#endif // AOI_USE_Y_AXIS

		for (std::unordered_map<NODE_ID, AOI_UNIT_SUB_SCRIPT>::iterator it = this->m_mapNodeChunk.begin();
			it != this->m_mapNodeChunk.end(); ++it)
		{
			unsigned int lChunk = unsigned int((it->second >> ((AOI_MAX_DIVIDE_NUM - 1 - this->m_dwDivideNum) * AOI_BIT_OFFSET)) & AOI_FLAF_MASK);
			assert(this->m_mapDividedMap.find(lChunk) != this->m_mapDividedMap.end());

			this->m_mapDividedMap[lChunk].m_mapNodeChunk[it->first] = it->second;
		}
		this->m_mapNodeChunk.clear();

		for (std::unordered_map<AOI_UNIT_SUB_SCRIPT, std::unordered_set<NODE_ID> >::iterator it = this->m_mapWatched.begin();
			it != this->m_mapWatched.end(); ++it)
		{
			unsigned int lChunk = unsigned int((it->first >> ((AOI_MAX_DIVIDE_NUM - 1 - this->m_dwDivideNum) * AOI_BIT_OFFSET)) & AOI_FLAF_MASK);
			assert(this->m_mapDividedMap.find(lChunk) != this->m_mapDividedMap.end());

			this->m_mapDividedMap[lChunk].m_mapWatched[it->first].swap(it->second);
		}
		this->m_mapWatched.clear();

		for (std::unordered_map<AOI_UNIT_SUB_SCRIPT, std::unordered_set<NODE_ID> >::iterator it = this->m_mapWatching.begin();
			it != this->m_mapWatching.end(); ++it)
		{
			unsigned int lChunk = unsigned int((it->first >> ((AOI_MAX_DIVIDE_NUM - 1 - this->m_dwDivideNum) * AOI_BIT_OFFSET)) & AOI_FLAF_MASK);
			assert(this->m_mapDividedMap.find(lChunk) != this->m_mapDividedMap.end());

			this->m_mapDividedMap[lChunk].m_mapWatching[it->first].swap(it->second);
		}
		this->m_mapWatching.clear();

		this->m_bDivided = true;
	}

	void MapInstance::Merge(){}

	void MapInstance::Enter(NODE_ID lNodeId, const AOICoordinate& refCoordinate
		, unsigned int dwWatchedRadius, unsigned int dwWatchingRadius)
	{
		if (!this->m_bDivided)
		{
			bool bRet = AOIUnits::Instance().GetMapPos(refCoordinate, this->m_mapNodeChunk[lNodeId]);
			assert(bRet);
		}
		for (unsigned int x = (refCoordinate.GetX() > dwWatchedRadius ? refCoordinate.GetX() - dwWatchedRadius : 0)
			; x < (refCoordinate.GetX() + dwWatchedRadius > (1 << AOI_MAX_DIVIDE_NUM) ? (1 << AOI_MAX_DIVIDE_NUM) : refCoordinate.GetX() + dwWatchedRadius); ++x)
		{
			for (unsigned int z = (refCoordinate.GetZ() > dwWatchedRadius ? refCoordinate.GetZ() - dwWatchedRadius : 0)
				; z < (refCoordinate.GetZ() + dwWatchedRadius > (1 << AOI_MAX_DIVIDE_NUM) ? (1 << AOI_MAX_DIVIDE_NUM) : refCoordinate.GetZ() + dwWatchedRadius); ++z)
			{
#if AOI_USE_Y_AXIS
				for (unsigned int y = (refCoordinate.GetY() > dwWatchedRadius ? refCoordinate.GetY() - dwWatchedRadius : 0)
					; y < (refCoordinate.GetY() + dwWatchedRadius > (1 << AOI_MAX_DIVIDE_NUM) ? (1 << AOI_MAX_DIVIDE_NUM) : refCoordinate.GetY() + dwWatchedRadius); ++y)
#endif
				{
					AOI_UNIT_SUB_SCRIPT lPos;
					bool bRet = AOIUnits::Instance().GetMapPos(AOICoordinate(x
#if AOI_USE_Y_AXIS
						, y
#endif
						, z), lPos);
					assert(bRet);
					MapInstance* pInstance = this->m_pRoot->GetInstance(lPos);

					assert(pInstance);

					pInstance->m_mapWatched[lPos].insert(lNodeId);
				}
			}
		}

		for (unsigned int x = (refCoordinate.GetX() > dwWatchingRadius ? refCoordinate.GetX() - dwWatchingRadius : 0)
			; x < (refCoordinate.GetX() + dwWatchingRadius > (1 << AOI_MAX_DIVIDE_NUM) ? (1 << AOI_MAX_DIVIDE_NUM) : refCoordinate.GetX() + dwWatchingRadius); ++x)
		{
			for (unsigned int z = (refCoordinate.GetZ() > dwWatchingRadius ? refCoordinate.GetZ() - dwWatchingRadius : 0)
				; z < (refCoordinate.GetZ() + dwWatchingRadius > (1 << AOI_MAX_DIVIDE_NUM) ? (1 << AOI_MAX_DIVIDE_NUM) : refCoordinate.GetZ() + dwWatchingRadius); ++z)
			{
#if AOI_USE_Y_AXIS
				for (unsigned int y = (refCoordinate.GetY() > dwWatchingRadius ? refCoordinate.GetY() - dwWatchingRadius : 0)
					; y < (refCoordinate.GetY() + dwWatchingRadius > (1 << AOI_MAX_DIVIDE_NUM) ? (1 << AOI_MAX_DIVIDE_NUM) : refCoordinate.GetY() + dwWatchingRadius); ++y)
#endif
				{
					AOI_UNIT_SUB_SCRIPT lPos;
					bool bRet = AOIUnits::Instance().GetMapPos(AOICoordinate(x
#if AOI_USE_Y_AXIS
						, y
#endif
						, z), lPos);
					assert(bRet);
					MapInstance* pInstance = this->m_pRoot->GetInstance(lPos);

					assert(pInstance);

					pInstance->m_mapWatching[lPos].insert(lNodeId);
				}
			}
		}
		

	}

	void MapInstance::Leave(NODE_ID lNodeId, const AOICoordinate& refCoordinate
			, unsigned int dwWatchedRadius, unsigned int dwWatchingRadius){}


} // namespace FXAOI
