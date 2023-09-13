#include "include/aoi_map_instance.h"
#include "include/aoi_unit.h"
#include "include/aoi_define.h"
#include "include/aoi_node_mgr.h"

#include <assert.h>
#include "aoi_map_instance.h"

namespace FXAOI
{
	MapInstance::MapInstance(unsigned int dwMapId, AOI_UNIT_SUB_SCRIPT lSubScript, unsigned int dwDivideNum, MapInstance* pRoot)
		: m_dwMapId(dwMapId)
		, m_lSubScript(lSubScript)
		, m_dwDivideNum(dwDivideNum)
		, m_bDivided(false)
		, m_pRoot(pRoot)
	{
		if (0 == m_pRoot){m_pRoot = this;}
	}

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
		
		unsigned int lChunk = unsigned int((lPos >> ((AOI_MAX_DIVIDE_NUM - 1 - this->m_dwDivideNum) * AOI_BIT_OFFSET)) & AOI_FLAG_MASK);
		assert(this->m_mapDividedMap.find(lChunk) != this->m_mapDividedMap.end());

		return this->m_mapDividedMap[lChunk].GetInstance(lPos);
	}

	void MapInstance::Enter(NODE_ID lNodeId, const AOICoordinate& refCoordinate
		, unsigned int dwWatchedRadius, unsigned int dwWatchingRadius)
	{
		AOI_UNIT_SUB_SCRIPT lPos = 0;
		bool bRet = AOIUnits::Instance().GetMapPos(refCoordinate, lPos);
		assert(bRet);
		if (this->m_bDivided)
		{
			unsigned int lChunk = unsigned int((lPos >> ((AOI_MAX_DIVIDE_NUM - 1 - this->m_dwDivideNum) * AOI_BIT_OFFSET)) & AOI_FLAG_MASK);
			assert(this->m_mapDividedMap.find(lChunk) != this->m_mapDividedMap.end());

			this->m_mapDividedMap[lChunk].Enter(lNodeId, refCoordinate, dwWatchedRadius, dwWatchingRadius);
			return;
		}

		this->m_mapNodeChunk[lNodeId] = lPos;
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

					// pInstance->m_mapWatched[lPos].insert(lNodeId);
					pInstance->AddWatched(lNodeId, lPos);
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

					// pInstance->m_mapWatching[lPos].insert(lNodeId);
					pInstance->AddWatching(lNodeId, lPos);
				}
			}
		}
		
		if (AOI_THRESHOLD_FOR_DIVIDE < this->m_mapNodeChunk.size())
		{
			this->Divide();
		}
		
	}

	void MapInstance::Leave(NODE_ID lNodeId, const AOICoordinate& refCoordinate
		, unsigned int dwWatchedRadius, unsigned int dwWatchingRadius)
	{
		AOI_UNIT_SUB_SCRIPT lPos = 0;
		bool bRet = AOIUnits::Instance().GetMapPos(refCoordinate, lPos);
		assert(bRet);
		if (this->m_bDivided)
		{
			unsigned int lChunk = unsigned int((lPos >> ((AOI_MAX_DIVIDE_NUM - 1 - this->m_dwDivideNum) * AOI_BIT_OFFSET)) & AOI_FLAG_MASK);
			assert(this->m_mapDividedMap.find(lChunk) != this->m_mapDividedMap.end());

			this->m_mapDividedMap[lChunk].Leave(lNodeId, refCoordinate, dwWatchedRadius, dwWatchingRadius);
		}
		else
		{
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
						// assert(pInstance->m_mapWatching[lPos].end() != pInstance->m_mapWatching[lPos].find(lNodeId));
						// pInstance->m_mapWatching[lPos].erase(lNodeId);
						// if(0 == pInstance->m_mapWatching[lPos].size()){pInstance->m_mapWatching.erase(lPos);}
						pInstance->RemoveWatched(lNodeId, lPos);
					}
				}
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
						// assert(pInstance->m_mapWatched[lPos].end() != pInstance->m_mapWatched[lPos].find(lNodeId));
						// pInstance->m_mapWatched[lPos].erase(lNodeId);
						// if(0 == pInstance->m_mapWatched[lPos].size()){pInstance->m_mapWatched.erase(lPos);}
						pInstance->RemoveWatching(lNodeId, lPos);
					}
				}
			}

			assert(this->m_mapNodeChunk.end() != this->m_mapNodeChunk.find(lNodeId));
			this->m_mapNodeChunk.erase(lNodeId);
		}

		if (!this->m_bDivided)
		{
			return;
		}
		
		size_t dwNodeCount = 0;

		for (std::unordered_map<unsigned int, MapInstance>::iterator it = this->m_mapDividedMap.begin();
			it != this->m_mapDividedMap.end(); ++it)
		{
			if (it->second.m_bDivided)
			{
				return;
			}
			dwNodeCount += it->second.m_mapNodeChunk.size();
		}

		if (dwNodeCount < AOI_THRESHOLD_FOR_MERGE)
		{
			this->Merge();
		}
	}

	void MapInstance::Move(NODE_ID lNodeId, const AOICoordinate& refFromCoordinate, const AOICoordinate& refToCoordinate
		, unsigned int dwWatchedRadius, unsigned int dwWatchingRadius)
	{
		class RemoveWatched
		{
		public:
			void operator ()(NODE_ID lNodeId, MapInstance* pInstance, AOI_UNIT_SUB_SCRIPT lPos)
			{
				MapInstance* pTargetInstance = pInstance->m_pRoot->GetInstance(lPos);
				assert(pTargetInstance);
				pInstance->RemoveWatched(lNodeId, lPos);
			}
		};
		class RemoveWatching
		{
		public:
			void operator ()(NODE_ID lNodeId, MapInstance* pInstance, AOI_UNIT_SUB_SCRIPT lPos) const
			{
				MapInstance* pTargetInstance = pInstance->m_pRoot->GetInstance(lPos);
				assert(pTargetInstance);
				pInstance->RemoveWatching(lNodeId, lPos);
			}
		};
		class AddWatched
		{
		public:
			void operator ()(NODE_ID lNodeId, MapInstance* pInstance, AOI_UNIT_SUB_SCRIPT lPos) const
			{
				MapInstance* pTargetInstance = pInstance->m_pRoot->GetInstance(lPos);
				assert(pTargetInstance);
				pInstance->AddWatched(lNodeId, lPos);
			}
		};
		class AddWatching
		{
		public:
			void operator ()(NODE_ID lNodeId, MapInstance* pInstance, AOI_UNIT_SUB_SCRIPT lPos) const
			{
				MapInstance* pTargetInstance = pInstance->m_pRoot->GetInstance(lPos);
				assert(pTargetInstance);
				pInstance->AddWatching(lNodeId, lPos);
			}
		};
		
		AOICoordinate oWatchedToCoordinateLeftDownBottom(refToCoordinate.GetX() > dwWatchedRadius? refToCoordinate.GetX() - dwWatchedRadius : 0
			, refToCoordinate.GetY() > dwWatchedRadius? refToCoordinate.GetY() - dwWatchedRadius : 0
			, refToCoordinate.GetZ() > dwWatchedRadius? refToCoordinate.GetZ() - dwWatchedRadius : 0
			);
		AOICoordinate oWatchedToCoordinateRigthUpTop(refToCoordinate.GetX() + dwWatchedRadius > (1 << AOI_MAX_DIVIDE_NUM) ? (1 << AOI_MAX_DIVIDE_NUM) : refToCoordinate.GetX() + dwWatchedRadius
			, refToCoordinate.GetY() + dwWatchedRadius > (1 << AOI_MAX_DIVIDE_NUM) ? (1 << AOI_MAX_DIVIDE_NUM) : refToCoordinate.GetY() + dwWatchedRadius
			, refToCoordinate.GetZ() + dwWatchedRadius > (1 << AOI_MAX_DIVIDE_NUM) ? (1 << AOI_MAX_DIVIDE_NUM) : refToCoordinate.GetZ() + dwWatchedRadius
			);

		for (unsigned int x = (refFromCoordinate.GetX() > dwWatchedRadius ? refFromCoordinate.GetX() - dwWatchedRadius : 0)
			; x < (refFromCoordinate.GetX() + dwWatchedRadius > (1 << AOI_MAX_DIVIDE_NUM) ? (1 << AOI_MAX_DIVIDE_NUM) : refFromCoordinate.GetX() + dwWatchedRadius); ++x)
		{
			for (unsigned int z = (refFromCoordinate.GetZ() > dwWatchedRadius ? refFromCoordinate.GetZ() - dwWatchedRadius : 0)
				; z < (refFromCoordinate.GetZ() + dwWatchedRadius > (1 << AOI_MAX_DIVIDE_NUM) ? (1 << AOI_MAX_DIVIDE_NUM) : refFromCoordinate.GetZ() + dwWatchedRadius); ++z)
			{
#if AOI_USE_Y_AXIS
				for (unsigned int y = (refFromCoordinate.GetY() > dwWatchedRadius ? refFromCoordinate.GetY() - dwWatchedRadius : 0)
					; y < (refFromCoordinate.GetY() + dwWatchedRadius > (1 << AOI_MAX_DIVIDE_NUM) ? (1 << AOI_MAX_DIVIDE_NUM) : refFromCoordinate.GetY() + dwWatchedRadius); ++y)
#endif
				{
					if (!(x >= oWatchedToCoordinateLeftDownBottom.GetX() && z >= oWatchedToCoordinateLeftDownBottom.GetZ()
						&& x < oWatchedToCoordinateRigthUpTop.GetX() && z < oWatchedToCoordinateRigthUpTop.GetZ()
#if AOI_USE_Y_AXIS
						&& y >= oWatchedToCoordinateLeftDownBottom.GetY()
						&& y < oWatchedToCoordinateRigthUpTop.GetY()
#endif
					))
					{
						AOI_UNIT_SUB_SCRIPT lPos;
						bool bRet = AOIUnits::Instance().GetMapPos(x
#if AOI_USE_Y_AXIS
							, y
#endif
							, z, lPos);
						assert(bRet);

						RemoveWatched()(lNodeId, this->m_pRoot, lPos);
					}
				}
			}
		}

		AOICoordinate oWatchingToCoordinateLeftDownBottom(refToCoordinate.GetX() > dwWatchedRadius? refToCoordinate.GetX() - dwWatchedRadius : 0
			, refToCoordinate.GetY() > dwWatchedRadius? refToCoordinate.GetY() - dwWatchedRadius : 0
			, refToCoordinate.GetZ() > dwWatchedRadius? refToCoordinate.GetZ() - dwWatchedRadius : 0
			);
		AOICoordinate oWatchingToCoordinateRigthUpTop(refToCoordinate.GetX() + dwWatchedRadius > (1 << AOI_MAX_DIVIDE_NUM) ? (1 << AOI_MAX_DIVIDE_NUM) : refToCoordinate.GetX() + dwWatchedRadius
			, refToCoordinate.GetY() + dwWatchedRadius > (1 << AOI_MAX_DIVIDE_NUM) ? (1 << AOI_MAX_DIVIDE_NUM) : refToCoordinate.GetY() + dwWatchedRadius
			, refToCoordinate.GetZ() + dwWatchedRadius > (1 << AOI_MAX_DIVIDE_NUM) ? (1 << AOI_MAX_DIVIDE_NUM) : refToCoordinate.GetZ() + dwWatchedRadius
			);
		for (unsigned int x = (refFromCoordinate.GetX() > dwWatchingRadius? refFromCoordinate.GetX() - dwWatchingRadius : 0)
			; x < (refFromCoordinate.GetX() + dwWatchingRadius > (1 << AOI_MAX_DIVIDE_NUM) ? (1 << AOI_MAX_DIVIDE_NUM) : refFromCoordinate.GetX() + dwWatchingRadius); ++x)
		{
			for (unsigned int z = (refFromCoordinate.GetZ() > dwWatchingRadius ? refFromCoordinate.GetZ() - dwWatchingRadius : 0)
				; z < (refFromCoordinate.GetZ() + dwWatchingRadius > (1 << AOI_MAX_DIVIDE_NUM) ? (1 << AOI_MAX_DIVIDE_NUM) : refFromCoordinate.GetZ() + dwWatchingRadius); ++z)
			{
#if AOI_USE_Y_AXIS
				for (unsigned int y = (refFromCoordinate.GetY() > dwWatchingRadius ? refFromCoordinate.GetY() - dwWatchingRadius : 0)
					; y < (refFromCoordinate.GetY() + dwWatchingRadius > (1 << AOI_MAX_DIVIDE_NUM) ? (1 << AOI_MAX_DIVIDE_NUM) : refFromCoordinate.GetY() + dwWatchingRadius); ++y)
#endif
				{
					if (!(x >= oWatchingToCoordinateLeftDownBottom.GetX() && z >= oWatchingToCoordinateLeftDownBottom.GetZ()
						&& x < oWatchingToCoordinateRigthUpTop.GetX() && z < oWatchingToCoordinateRigthUpTop.GetZ()
#if AOI_USE_Y_AXIS
						&& y >= oWatchingToCoordinateLeftDownBottom.GetY()
						&& y < oWatchingToCoordinateRigthUpTop.GetY()
#endif
					))
					{
						AOI_UNIT_SUB_SCRIPT lPos;
						bool bRet = AOIUnits::Instance().GetMapPos(x
#if AOI_USE_Y_AXIS
							, y
#endif
							, z, lPos);
						assert(bRet);

						RemoveWatching()(lNodeId, this->m_pRoot, lPos);
					}
				}
			}
		}

		AOICoordinate oWatchedFromCoordinateLeftDownBottom(refFromCoordinate.GetX() > dwWatchedRadius? refToCoordinate.GetX() - dwWatchedRadius : 0
			, refFromCoordinate.GetY() > dwWatchedRadius? refFromCoordinate.GetY() - dwWatchedRadius : 0
			, refFromCoordinate.GetZ() > dwWatchedRadius? refFromCoordinate.GetZ() - dwWatchedRadius : 0
			);
		AOICoordinate oWatchedFromCoordinateRigthUpTop(refFromCoordinate.GetX() + dwWatchedRadius > (1 << AOI_MAX_DIVIDE_NUM) ? (1 << AOI_MAX_DIVIDE_NUM) : refFromCoordinate.GetX() + dwWatchedRadius
			, refFromCoordinate.GetY() + dwWatchedRadius > (1 << AOI_MAX_DIVIDE_NUM) ? (1 << AOI_MAX_DIVIDE_NUM) : refFromCoordinate.GetY() + dwWatchedRadius
			, refFromCoordinate.GetZ() + dwWatchedRadius > (1 << AOI_MAX_DIVIDE_NUM) ? (1 << AOI_MAX_DIVIDE_NUM) : refFromCoordinate.GetZ() + dwWatchedRadius
			);

		for (unsigned int x = (refToCoordinate.GetX() > dwWatchedRadius ? refToCoordinate.GetX() - dwWatchedRadius : 0)
			; x < (refToCoordinate.GetX() + dwWatchedRadius > (1 << AOI_MAX_DIVIDE_NUM) ? (1 << AOI_MAX_DIVIDE_NUM) : refToCoordinate.GetX() + dwWatchedRadius); ++x)
		{
			for (unsigned int z = (refToCoordinate.GetZ() > dwWatchedRadius ? refToCoordinate.GetZ() - dwWatchedRadius : 0)
				; z < (refToCoordinate.GetZ() + dwWatchedRadius > (1 << AOI_MAX_DIVIDE_NUM) ? (1 << AOI_MAX_DIVIDE_NUM) : refToCoordinate.GetZ() + dwWatchedRadius); ++z)
			{
#if AOI_USE_Y_AXIS
				for (unsigned int y = (refToCoordinate.GetY() > dwWatchedRadius ? refToCoordinate.GetY() - dwWatchedRadius : 0)
					; y < (refToCoordinate.GetY() + dwWatchedRadius > (1 << AOI_MAX_DIVIDE_NUM) ? (1 << AOI_MAX_DIVIDE_NUM) : refToCoordinate.GetY() + dwWatchedRadius); ++y)
#endif
				{
					if (!(x >= oWatchedFromCoordinateLeftDownBottom.GetX() && z >= oWatchedFromCoordinateLeftDownBottom.GetZ()
						&& x < oWatchedFromCoordinateRigthUpTop.GetX() && z < oWatchedFromCoordinateRigthUpTop.GetZ()
#if AOI_USE_Y_AXIS
						&& y >= oWatchedFromCoordinateLeftDownBottom.GetY()
						&& y < oWatchedFromCoordinateRigthUpTop.GetY()
#endif
					))
					{
						AOI_UNIT_SUB_SCRIPT lPos;
						bool bRet = AOIUnits::Instance().GetMapPos(x
#if AOI_USE_Y_AXIS
							, y
#endif
							, z, lPos);
						assert(bRet);

						AddWatched()(lNodeId, this->m_pRoot, lPos);
					}
				}
			}
		}

		AOICoordinate oWatchingFromCoordinateLeftDownBottom(refFromCoordinate.GetX() > dwWatchingRadius? refToCoordinate.GetX() - dwWatchingRadius : 0
			, refFromCoordinate.GetY() > dwWatchingRadius? refFromCoordinate.GetY() - dwWatchingRadius : 0
			, refFromCoordinate.GetZ() > dwWatchingRadius? refFromCoordinate.GetZ() - dwWatchingRadius : 0
			);
		AOICoordinate oWatchingFromCoordinateRigthUpTop(refFromCoordinate.GetX() + dwWatchingRadius > (1 << AOI_MAX_DIVIDE_NUM) ? (1 << AOI_MAX_DIVIDE_NUM) : refFromCoordinate.GetX() + dwWatchingRadius
			, refFromCoordinate.GetY() + dwWatchingRadius > (1 << AOI_MAX_DIVIDE_NUM) ? (1 << AOI_MAX_DIVIDE_NUM) : refFromCoordinate.GetY() + dwWatchingRadius
			, refFromCoordinate.GetZ() + dwWatchingRadius > (1 << AOI_MAX_DIVIDE_NUM) ? (1 << AOI_MAX_DIVIDE_NUM) : refFromCoordinate.GetZ() + dwWatchingRadius
			);
		for (unsigned int x = (refToCoordinate.GetX() > dwWatchingRadius ? refToCoordinate.GetX() - dwWatchingRadius : 0)
			; x < (refToCoordinate.GetX() + dwWatchingRadius > (1 << AOI_MAX_DIVIDE_NUM) ? (1 << AOI_MAX_DIVIDE_NUM) : refToCoordinate.GetX() + dwWatchingRadius); ++x)
		{
			for (unsigned int z = (refToCoordinate.GetZ() > dwWatchingRadius ? refToCoordinate.GetZ() - dwWatchingRadius : 0)
				; z < (refToCoordinate.GetZ() + dwWatchingRadius > (1 << AOI_MAX_DIVIDE_NUM) ? (1 << AOI_MAX_DIVIDE_NUM) : refToCoordinate.GetZ() + dwWatchingRadius); ++z)
			{
#if AOI_USE_Y_AXIS
				for (unsigned int y = (refToCoordinate.GetY() > dwWatchingRadius ? refToCoordinate.GetY() - dwWatchingRadius : 0)
					; y < (refToCoordinate.GetY() + dwWatchingRadius > (1 << AOI_MAX_DIVIDE_NUM) ? (1 << AOI_MAX_DIVIDE_NUM) : refToCoordinate.GetY() + dwWatchingRadius); ++y)
#endif
				{
					if (!(x >= oWatchingFromCoordinateLeftDownBottom.GetX() && z >= oWatchingFromCoordinateLeftDownBottom.GetZ()
						&& x < oWatchingFromCoordinateRigthUpTop.GetX() && z < oWatchingFromCoordinateRigthUpTop.GetZ()
#if AOI_USE_Y_AXIS
						&& y >= oWatchingFromCoordinateLeftDownBottom.GetY()
						&& y < oWatchingFromCoordinateRigthUpTop.GetY()
#endif
					))
					{
						AOI_UNIT_SUB_SCRIPT lPos;
						bool bRet = AOIUnits::Instance().GetMapPos(x
#if AOI_USE_Y_AXIS
							, y
#endif
							, z, lPos);
						assert(bRet);

						AddWatching()(lNodeId, this->m_pRoot, lPos);
					}
				}
			}
		}

	}

	void MapInstance::GetNodeInPos(AOI_UNIT_SUB_SCRIPT lPos, std::unordered_map<unsigned int, std::unordered_set<NODE_ID>> &refWatchingNode)
	{
		for (std::unordered_map<NODE_ID, AOI_UNIT_SUB_SCRIPT>::iterator it = this->m_mapNodeChunk.begin();
			it != this->m_mapNodeChunk.end(); ++it)
		{
			if (lPos == it->second)
			{
				AOINode* pNode = AOINodeMgr::Instance().GetNode(it->first);
				assert(pNode);
				refWatchingNode[pNode->GetAOIType()].insert(it->first);
			}
		}
	}

	void MapInstance::GetWatchingInPos(AOI_UNIT_SUB_SCRIPT lPos, std::unordered_map<unsigned int, std::unordered_set<NODE_ID>> &refWatchingNode)
	{
		std::unordered_map<AOI_UNIT_SUB_SCRIPT, std::unordered_set<NODE_ID> >::iterator it = this->m_mapWatching.find(lPos);
		if (it == this->m_mapWatching.end())
		{
			return;
		}

		for (std::unordered_set<NODE_ID>::iterator it2 = it->second.begin();
			it2 != it->second.end(); ++it2)
		{
			AOINode* pNode = AOINodeMgr::Instance().GetNode(*it2);
			assert(pNode);
			refWatchingNode[pNode->GetAOIType()].insert(*it2);
		}
	}

	bool MapInstance::CanWatching(NODE_ID lNodeId, AOI_UNIT_SUB_SCRIPT lPos)
	{
		std::unordered_map<AOI_UNIT_SUB_SCRIPT, std::unordered_set<NODE_ID> >::iterator it = this->m_mapWatching.find(lPos);
		if (it == this->m_mapWatching.end())
		{
			return false;
		}
		
		return it->second.end() != it->second.find(lNodeId);
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
			unsigned int lChunk = unsigned int((it->second >> ((AOI_MAX_DIVIDE_NUM - 1 - this->m_dwDivideNum) * AOI_BIT_OFFSET)) & AOI_FLAG_MASK);
			assert(this->m_mapDividedMap.find(lChunk) != this->m_mapDividedMap.end());

			this->m_mapDividedMap[lChunk].m_mapNodeChunk[it->first] = it->second;
		}
		this->m_mapNodeChunk.clear();

		for (std::unordered_map<AOI_UNIT_SUB_SCRIPT, std::unordered_set<NODE_ID> >::iterator it = this->m_mapWatched.begin();
			it != this->m_mapWatched.end(); ++it)
		{
			unsigned int lChunk = unsigned int((it->first >> ((AOI_MAX_DIVIDE_NUM - 1 - this->m_dwDivideNum) * AOI_BIT_OFFSET)) & AOI_FLAG_MASK);
			assert(this->m_mapDividedMap.find(lChunk) != this->m_mapDividedMap.end());

			this->m_mapDividedMap[lChunk].m_mapWatched[it->first].swap(it->second);
		}
		this->m_mapWatched.clear();

		for (std::unordered_map<AOI_UNIT_SUB_SCRIPT, std::unordered_set<NODE_ID> >::iterator it = this->m_mapWatching.begin();
			it != this->m_mapWatching.end(); ++it)
		{
			unsigned int lChunk = unsigned int((it->first >> ((AOI_MAX_DIVIDE_NUM - 1 - this->m_dwDivideNum) * AOI_BIT_OFFSET)) & AOI_FLAG_MASK);
			assert(this->m_mapDividedMap.find(lChunk) != this->m_mapDividedMap.end());

			this->m_mapDividedMap[lChunk].m_mapWatching[it->first].swap(it->second);
		}
		this->m_mapWatching.clear();

		this->m_bDivided = true;
	}

	void MapInstance::Merge()
	{
		assert(AOI_DIVIDE_CHILDREN_NUM == this->m_mapDividedMap.size());
		
		for (std::unordered_map<unsigned int, MapInstance>::iterator it = this->m_mapDividedMap.begin();
			it != this->m_mapDividedMap.end(); ++it)
		{
			for (std::unordered_map<NODE_ID, AOI_UNIT_SUB_SCRIPT>::iterator it2 = it->second.m_mapNodeChunk.begin();
				it2 != it->second.m_mapNodeChunk.end(); ++it2)
			{
				this->m_mapNodeChunk[it2->first] = it2->second;
			}
			
			for (std::unordered_map<AOI_UNIT_SUB_SCRIPT, std::unordered_set<NODE_ID> >::iterator it2 = it->second.m_mapWatched.begin();
				it2 != it->second.m_mapWatched.end(); ++it2)
			{
				assert(this->m_mapWatched.find(it2->first) == this->m_mapWatched.end());

				this->m_mapWatched[it2->first].swap(it2->second);
			}

			for (std::unordered_map<AOI_UNIT_SUB_SCRIPT, std::unordered_set<NODE_ID> >::iterator it2 = it->second.m_mapWatching.begin();
				it2 != it->second.m_mapWatching.end(); ++it2)
			{
				assert(this->m_mapWatching.find(it2->first) == this->m_mapWatching.end());

				this->m_mapWatching[it2->first].swap(it2->second);
			}
		}

		this->m_mapDividedMap.clear();
		this->m_bDivided = false;
	}

	void MapInstance::AddWatched(NODE_ID lNodeId, AOI_UNIT_SUB_SCRIPT lPos)
	{
		assert(this->m_mapWatched[lPos].find(lNodeId) == this->m_mapWatched[lPos].end());
		this->m_mapWatched[lPos].insert(lNodeId);
		this->AfterAddWatched(lNodeId, lPos);
	}
	void MapInstance::AddWatching(NODE_ID lNodeId, AOI_UNIT_SUB_SCRIPT lPos)
	{
		assert(this->m_mapWatching[lPos].find(lNodeId) == this->m_mapWatching[lPos].end());
		this->m_mapWatching[lPos].insert(lNodeId);
		this->AfterAddWatching(lNodeId, lPos);
	}
	void MapInstance::RemoveWatched(NODE_ID lNodeId, AOI_UNIT_SUB_SCRIPT lPos)
	{
		assert(this->m_mapWatched[lPos].end() != this->m_mapWatched[lPos].find(lNodeId));
		assert(this->m_mapWatched[lPos].find(lNodeId) != this->m_mapWatched[lPos].end());
		this->m_mapWatched[lPos].erase(lNodeId);
		if (0 == this->m_mapWatched[lPos].size()){this->m_mapWatched.erase(lPos);}
		this->AfterRemoveWatched(lNodeId, lPos);
	}
	void MapInstance::RemoveWatching(NODE_ID lNodeId, AOI_UNIT_SUB_SCRIPT lPos)
	{
		assert(this->m_mapWatching[lPos].end() != this->m_mapWatching[lPos].find(lNodeId));
		assert(this->m_mapWatching[lPos].find(lNodeId) != this->m_mapWatching[lPos].end());
		this->m_mapWatching[lPos].erase(lNodeId);
		if (0 == this->m_mapWatching[lPos].size()){this->m_mapWatching.erase(lPos);}
		this->AfterRemoveWatching(lNodeId, lPos);
	}

	void MapInstance::AfterAddWatched(NODE_ID lNodeId, AOI_UNIT_SUB_SCRIPT lPos){}
	void MapInstance::AfterAddWatching(NODE_ID lNodeId, AOI_UNIT_SUB_SCRIPT lPos)
	{
		AOINode* pNode = AOINodeMgr::Instance().GetNode(lNodeId);
		assert(pNode);
		pNode->AddWatching(lPos);
	}
	void MapInstance::AfterRemoveWatched(NODE_ID lNodeId, AOI_UNIT_SUB_SCRIPT lPos){}
	void MapInstance::AfterRemoveWatching(NODE_ID lNodeId, AOI_UNIT_SUB_SCRIPT lPos){}

} // namespace FXAOI
