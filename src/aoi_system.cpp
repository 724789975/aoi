#include "../include/aoi_system.h"
#include "aoi_map_info_mgr.h"
#include "aoi_map_instance_mgr.h"
#include "aoi_node_mgr.h"
#include "aoi_map_info_mgr.h"
#include "aoi_unit.h"
#include "arr_map.h"

#include <assert.h>

namespace FXAOI
{
	void AoiOperatorDefault (NODE_ID lNodeId
		, std::vector<AoiOperatorInfo> vecAddWatching
		, std::vector<AoiOperatorInfo> vecDelWatching
		, std::vector<AoiOperatorInfo> vecAddWatched
		, std::vector<AoiOperatorInfo> vecDelWatched
	){}
	AOISystem AOISystem::s_oInstance;

	bool AOISystem::AddMap(unsigned int dwMapId, unsigned int dwViweRadius, unsigned int dwLength, unsigned int dwWidth
#if AOI_USE_Y_AXIS
			, unsigned int dwHight
#endif
 			, double dOffsetX
#if AOI_USE_Y_AXIS
			, double dOffsetY
#endif
			, double dOffsetZ
		)
	{
		AOIMapInfoMgr::Instance().AddMap(dwMapId, dwViweRadius, dwLength, dwWidth
#if AOI_USE_Y_AXIS
			, dwHight
#endif
 			, dOffsetX
#if AOI_USE_Y_AXIS
			, dOffsetY
#endif
			, dOffsetZ

		);

		AOIMapInstanceMgr::Instance().AddMap(dwMapId);
		
		return true;
	}

	void AOISystem::RemoveMap(unsigned int dwMapId)
	{
		AOIMapInstanceMgr::Instance().RemoveMap(dwMapId);
	}

	AOIMap<unsigned int, AOIMap<unsigned int, AOIVisibilityType> > g_mapAOIVisibilityTypes;
	void AOISystem::SetAOIVisibilityType(unsigned int dwAOIType1, unsigned int dwAOIType2, AOIVisibilityType type)
	{
		g_mapAOIVisibilityTypes[dwAOIType1][dwAOIType2] = type;
	}

	const AOIVisibilityType& AOISystem::GetAOIVisibilityType(unsigned int dwAOIType1, unsigned int dwAOIType2)
	{
		return g_mapAOIVisibilityTypes[dwAOIType1][dwAOIType2];
	}

	AOIMap<unsigned int, AOIMap<unsigned int, unsigned int> > g_mapAOINodeLimit;
	void AOISystem::SetAOINodeLimit(unsigned int dwAOIType1, unsigned int dwAOIType2, unsigned int dwNum)
	{
		g_mapAOINodeLimit[dwAOIType1][dwAOIType2] = dwNum;
	}

	const unsigned int& AOISystem::GetAOINodeLimit(unsigned int dwAOIType1, unsigned int dwAOIType2)
	{
		static unsigned int dwLimit = -1;
		if (g_mapAOINodeLimit.end() == g_mapAOINodeLimit.find(dwAOIType1))
		{
			return dwLimit;
		}

		if (g_mapAOINodeLimit[dwAOIType1].end() == g_mapAOINodeLimit[dwAOIType1].find(dwAOIType2))
		{
			return dwLimit;
		}
		
		return g_mapAOINodeLimit[dwAOIType1][dwAOIType2];
	}
	
	void AOISystem::AddNode(NODE_ID lNodeId, unsigned int dwAOIType, unsigned int dwWatchedRadius, unsigned int dwWatchingRadius)
	{
		AOINode* pNode = AOINodeMgr::Instance().AddNode(lNodeId, dwAOIType, dwWatchedRadius, dwWatchingRadius);
		assert(pNode);
	}

	void AOISystem::RemoveNode(NODE_ID lNodeId)
	{
		AOINodeMgr::Instance().RemoveNode(lNodeId);
	}

	void AOISystem::EnterMap(NODE_ID lNodeId, unsigned int dwMapId, const NodePosition &refPosition)
	{
		MapInfo* pInfo = AOIMapInfoMgr::Instance().GetMap(dwMapId);
		assert(pInfo);

		AOINode* pNode = AOINodeMgr::Instance().GetNode(lNodeId);
		assert(pNode);
		pNode->SetMapId(dwMapId);
		pNode->SetPosition(refPosition);
		pNode->SetCoordinate(pInfo->GetAOICoordinate(refPosition));

		MapInstance* pInstance = AOIMapInstanceMgr().Instance().GetMap(dwMapId);
		assert(pInstance);
		MapInstance* pInstance1 = pInstance->GetInstance(pNode->GetCoordinate());
		assert(pInstance1);
		pInstance1->Enter(lNodeId, pNode->GetCoordinate(), pNode->GetWatchedRadius(), pNode->GetWatchingRadius());
		pNode->CalcView(pInstance);
	}

	void AOISystem::LeaveMap(NODE_ID lNodeId)
	{
		AOINode* pNode = AOINodeMgr::Instance().GetNode(lNodeId);
		assert(pNode);

		MapInstance* pInstance = AOIMapInstanceMgr().Instance().GetMap(pNode->GetMapId());
		assert(pInstance);
		pInstance->Leave(lNodeId, pNode->GetCoordinate(), pNode->GetWatchedRadius(), pNode->GetWatchingRadius());
		pNode->CalcView(pInstance);
		pNode->AfterLeaveMap();
	}

	void AOISystem::Move(NODE_ID lNodeId, const NodePosition &refPosition)
	{
		//如果这个节点有父节点 那么aoi内的移动是无效的
		if (this->m_mapNodeParent.end() != this->m_mapNodeParent.find(lNodeId))
		{
			return;
		}
		AOINode* pNode = AOINodeMgr::Instance().GetNode(lNodeId);
		assert(pNode);

		MapInfo* pInfo = AOIMapInfoMgr::Instance().GetMap(pNode->GetMapId());
		assert(pInfo);

		const AOICoordinate& refCoordinate = pInfo->GetAOICoordinate(refPosition);
		if (refCoordinate.GetCoordinate() == pNode->GetCoordinate().GetCoordinate())
		{
			//在aoi格子上没有变化
			return;
		}
		
		MapInstance* pInstance = AOIMapInstanceMgr().Instance().GetMap(pNode->GetMapId());
		assert(pInstance);
		pInstance->Move(lNodeId, pNode->GetCoordinate(), refCoordinate, pNode->GetWatchedRadius(), pNode->GetWatchingRadius());
		pNode->SetCoordinate(refCoordinate);
		pNode->CalcView(pInstance);
	}

	void AOISystem::AddChild(NODE_ID lNodeId, NODE_ID lChildId)
	{
		AOINode* pNode = AOINodeMgr::Instance().GetNode(lNodeId);
		assert(pNode);
		this->m_mapNodeParent[lChildId] = lNodeId;
		pNode->AddChild(lChildId);
	}

	void AOISystem::RemoveChild(NODE_ID lNodeId, NODE_ID lChildId)
	{
		AOINode* pNode = AOINodeMgr::Instance().GetNode(lNodeId);
		assert(pNode);
		pNode->RemoveChild(lChildId);
		this->m_mapNodeParent.erase(lChildId);
	}

	void AOISystem::DebugNode(NODE_ID lNodeId, std::ostream& refOstream)
	{
		if (this->m_mapNodeParent.end() != this->m_mapNodeParent.find(lNodeId))
		{
			refOstream << lNodeId;
			refOstream << ",parent:" << (lNodeId = this->m_mapNodeParent[lNodeId]) << ",";
		}
		
		AOINode* pNode = AOINodeMgr::Instance().GetNode(lNodeId);
		assert(pNode);
		pNode->Debug(refOstream);
	}
}
// namespace FXAOI
