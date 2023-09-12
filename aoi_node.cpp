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

	double Distance(const NodePosition &l, const NodePosition &r)
	{
		return (l.x - r.x) * (l.x - r.x) + (l.z - r.z) * (l.z - r.z)
#if AOI_USE_Y_AXIS
			+ (l.y - r.y) * (l.y - r.y)
#endif
		;
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

		std::unordered_map<unsigned int, std::unordered_set<NODE_ID> > mapWatchingNodes;
		//获取观察范围内地块中node
		for (std::unordered_set<AOI_UNIT_SUB_SCRIPT>::iterator it = setTemp.begin()
			; it != setTemp.end(); ++it)
		{
			MapInstance* pInstance = pMapRoot->GetInstance(*it);
			assert(pInstance);
			pInstance->GetNodeInPos(*it, mapWatchingNodes);
		}
		
		//获取当前地块中 能被观察到的node
		AOI_UNIT_SUB_SCRIPT lPos;
		bool bRet = AOIUnits::Instance().GetMapPos(this->m_oCoordinate, lPos);
		assert(bRet);
		MapInstance* pInstance = pMapRoot->GetInstance(this->m_oCoordinate);
		assert(pInstance);
		pInstance->GetNodeInPos(lPos, mapWatchingNodes);

		mapWatchingNodes[this->m_dwAOIType].erase(this->m_lNodeId);

		//新增进入视野
		std::unordered_map<unsigned int, std::unordered_set<NODE_ID> > mapAddWatching;
		for (std::unordered_map<unsigned int, std::unordered_set<NODE_ID> >::iterator it1 = mapWatchingNodes.begin()
			; it1 != mapWatchingNodes.end(); ++it1)
		{
			if (GetAOIVisibilityType(this->m_dwAOIType, it1->first) >= AOIVisibilityType::AOIVisibilityType_Invisible)
			{
				continue;
			}
			
			if (GetAOIVisibilityType(this->m_dwAOIType, it1->first) == AOIVisibilityType::AOIVisibilityType_Mutual_Visibility)
			{
				//优先级队列
				std::map<double, NODE_ID> mapFirstNode;
				for (std::unordered_set<NODE_ID>::iterator it2 = it1->second.begin()
					; it2 != it1->second.end();)
				{
					std::unordered_set<NODE_ID>::iterator tmpIt2 = it2++;
					//被观察者视野可及范围内才能加入
					if (!pInstance->CanWatching(*tmpIt2, lPos))
					{
						continue;
					}
					
					if (AOINode* pNode = AOINodeMgr::Instance().GetNode(*tmpIt2))
					{
						if (AOI_MAX_NODE_IN_VIEW > pNode->m_mapWatching[it1->first].size())
						{
							mapAddWatching[it1->first].insert(*tmpIt2);
							mapFirstNode[Distance(pNode->m_oPosition, this->m_oPosition)] = *tmpIt2;
						}
					}
				}

				if (this->m_mapWatching[it1->first].size() + mapAddWatching[it1->first].size() > AOI_MAX_NODE_IN_VIEW)
				{
					int dwNum = this->m_mapWatching[it1->first].size() + mapAddWatching[it1->first].size() - AOI_MAX_NODE_IN_VIEW;
					for (std::map<double, NODE_ID>::iterator it3 = mapFirstNode.begin()
						; it3 != mapFirstNode.end() && 0 < dwNum; ++it3, --dwNum)
					{
						mapAddWatching[it1->first].erase(it3->second);
					}
				}
			}
			else
			{
				for (std::unordered_set<NODE_ID>::iterator it2 = it1->second.begin()
					; it2 != it1->second.end(); ++it2)
				{
					if (this->m_mapWatching[it1->first].end() == this->m_mapWatching[it1->first].find(*it2))
					{
						mapAddWatching[it1->first].insert(*it2);
					}
				}
			}
		}

		//离开视野
		std::unordered_map<unsigned int, std::unordered_set<NODE_ID> > mapDelWatching;
		for (std::unordered_map<unsigned int, std::unordered_set<NODE_ID> >::iterator it1 = this->m_mapWatching.begin()
			; it1 != this->m_mapWatching.end(); ++it1)
		{
			for (std::unordered_set<NODE_ID>::iterator it2 = it1->second.begin()
				; it2 != it1->second.end(); ++it2)
			{
				if (mapWatchingNodes[it1->first].end() == mapWatchingNodes[it1->first].find(*it2))
				{
					mapDelWatching[it1->first].insert(*it2);
				}
			}
		}

		//TODO 处理被观察列表
		
	}

} // namespace FXAOI
