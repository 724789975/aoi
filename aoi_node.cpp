#include "aoi_node.h"
#include "aoi_map_instance.h"
#include "aoi_node_mgr.h"

#include <unordered_map>
#include <assert.h>

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

	AOINode::AOINode(NODE_ID lNodeId, unsigned int dwAOIType, unsigned int dwWatchedRadius, unsigned int dwWatchingRadius)
		: m_lNodeId(lNodeId)
		, m_dwAOIType(dwAOIType)
		, m_oCoordinate(0, 0, 0)
		, m_dwWatchedRadius(dwWatchedRadius)
		, m_dwWatchingRadius(dwWatchingRadius)
	{}

	AOINode::AOINode()
		: m_lNodeId(0)
		, m_dwAOIType(0)
		, m_oCoordinate(0, 0, 0)
		, m_dwWatchedRadius(0)
		, m_dwWatchingRadius(0)
	{}

	void AOINode::AddWatching(AOI_UNIT_SUB_SCRIPT lPos)
	{
		this->m_setTempWatchingMap.insert(lPos);
	}

	void AOINode::CalcView(MapInstance* pMapRoot)
	{
		std::unordered_set<AOI_UNIT_SUB_SCRIPT> setTemp;
		setTemp.swap(this->m_setTempWatchingMap);

		std::unordered_set<NODE_ID> setWatchingNodes;
		//获取观察范围内地块中node
		for (std::unordered_set<AOI_UNIT_SUB_SCRIPT>::iterator it = setTemp.begin()
			; it != setTemp.end(); ++it)
		{
			if (MapInstance* pInstance = pMapRoot->GetInstance(*it))
			{
				pInstance->GetNodeInPos(*it, setWatchingNodes);
			}
		}
		
		//获取当前地块中 能被观察到的node
		AOI_UNIT_SUB_SCRIPT lPos;
		bool bRet = AOIUnits::Instance().GetMapPos(this->m_oCoordinate, lPos);
		assert(bRet);
		if (MapInstance* pInstance = pMapRoot->GetInstance(this->m_oCoordinate))
		{
			pInstance->GetNodeInPos(lPos, setWatchingNodes);
		}

		setWatchingNodes.erase(this->m_lNodeId);

		//新增进入视野
		std::unordered_set<NODE_ID> setAddWatching;
		for (std::unordered_set<NODE_ID>::iterator it = setWatchingNodes.begin()
			; it != setWatchingNodes.end(); ++it)
		{
			//已经在视野中的 不再做处理
			if (this->m_setWatching.end() == this->m_setWatching.find(*it))
			{
				setAddWatching.insert(*it);
			}
		}
		
		//离开视野
		std::unordered_set<NODE_ID> setDelWatching;
		for (std::unordered_set<NODE_ID>::iterator it = this->m_setWatching.begin()
			; it != m_setWatching.end(); ++it)
		{
			if (setWatchingNodes.end() == setWatchingNodes.find(*it))
			{
				setDelWatching.insert(*it);
			}
		}
	}

} // namespace FXAOI
