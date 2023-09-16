#include "include/aoi_system.h"
#include "aoi_map_info_mgr.h"
#include "aoi_map_instance_mgr.h"
#include "aoi_node_mgr.h"
#include "aoi_map_info_mgr.h"
#include "aoi_unit.h"

#include <assert.h>
#include "aoi_system.h"

namespace FXAOI
{
	std::unordered_map<unsigned int, std::unordered_map<unsigned int, AOIVisibilityType> > g_mapAOIVisibilityTypes;
	void SetAOIVisibilityType(unsigned int dwAOIType1, unsigned int dwAOIType2, AOIVisibilityType type)
	{
		g_mapAOIVisibilityTypes[dwAOIType1][dwAOIType2] = type;
	}

	AOIVisibilityType GetAOIVisibilityType(unsigned int dwAOIType1, unsigned int dwAOIType2)
	{
		return g_mapAOIVisibilityTypes[dwAOIType1][dwAOIType2];
	}

	std::unordered_map<unsigned int, std::unordered_map<unsigned int, unsigned int> > g_mapAOINodeLimit;
	void SetAOINodeLimit(unsigned int dwAOIType1, unsigned int dwAOIType2, unsigned int dwNum)
	{
		g_mapAOINodeLimit[dwAOIType1][dwAOIType2] = dwNum;
	}

	unsigned int GetAOINodeLimit(unsigned int dwAOIType1, unsigned int dwAOIType2)
	{
		return g_mapAOINodeLimit[dwAOIType1][dwAOIType2];
	}
	AOISystem AOISystem::s_oInstace;

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

		MapInstance* pInstance = AOIMapInstanceMgr().Instance().AddMap(dwMapId);
		assert(pInstance);
		pInstance = pInstance->GetInstance(pNode->GetCoordinate());
		assert(pInstance);
		pInstance->Enter(lNodeId, pNode->GetCoordinate(), pNode->GetWatchedRadius(), pNode->GetWatchingRadius());
		pNode->CalcView(pInstance);
	}

	void AOISystem::LeaveMap(NODE_ID lNodeId, const NodePosition &refPosition)
	{
		AOINode* pNode = AOINodeMgr::Instance().GetNode(lNodeId);
		assert(pNode);

		MapInstance* pInstance = AOIMapInstanceMgr().Instance().AddMap(pNode->GetMapId());
		assert(pInstance);
		pInstance->Leave(lNodeId, pNode->GetCoordinate(), pNode->GetWatchedRadius(), pNode->GetWatchingRadius());
		pNode->CalcView(pInstance);
	}

	void AOISystem::Move(NODE_ID lNodeId, const NodePosition &refPosition)
	{
		AOINode* pNode = AOINodeMgr::Instance().GetNode(lNodeId);
		assert(pNode);

		MapInfo* pInfo = AOIMapInfoMgr::Instance().GetMap(pNode->GetMapId());
		assert(pInfo);

		MapInstance* pInstance = AOIMapInstanceMgr().Instance().AddMap(pNode->GetMapId());
		assert(pInstance);
		const AOICoordinate& refCoordinate = pInfo->GetAOICoordinate(refPosition);
		pInstance->Move(lNodeId, pNode->GetCoordinate(), refCoordinate, pNode->GetWatchedRadius(), pNode->GetWatchingRadius());
		pNode->SetCoordinate(refCoordinate);
		pNode->CalcView(pInstance);
	}
}
// namespace FXAOI
