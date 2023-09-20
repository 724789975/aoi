#include "aoi_node.h"
#include "aoi_map_instance.h"
#include "aoi_node_mgr.h"
#include "../include/aoi_define.h"
#include "../include/aoi_system.h"

#include "../include/arr_map.h"
#include "../include/arr_set.h"
#include <map>
#include <assert.h>

namespace FXAOI
{
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
		, m_dwMapId(0)
		, m_oCoordinate(0
#if AOI_USE_Y_AXIS
			, 0
#endif
			, 0)
		, m_oPosition{ 0, 0, 0 }
		, m_dwWatchedRadius(dwWatchedRadius)
		, m_dwWatchingRadius(dwWatchingRadius)
	{}

	AOINode::AOINode()
		: m_lNodeId(0)
		, m_dwAOIType(0)
		, m_dwMapId(0)
		, m_oCoordinate(0
#if AOI_USE_Y_AXIS
			, 0
#endif
			, 0)
		, m_oPosition{ 0, 0, 0 }
		, m_dwWatchedRadius(0)
		, m_dwWatchingRadius(0)
	{}

	void AOINode::SetPosition(const NodePosition &refPosition)
	{
		m_oPosition = refPosition;
	}

	void AOINode::AddWatching(AOI_UNIT_SUB_SCRIPT lPos)
	{
		this->m_setTempWatchingMap.insert(lPos);
	}

	void AOINode::CalcView(MapInstance* pMapRoot)
	{
		std::set<AOI_UNIT_SUB_SCRIPT> setTemp;
		setTemp.swap(this->m_setTempWatchingMap);

		AOIMap<unsigned int, AOISet<NODE_ID> > mapWatchingNodes;
		//��ȡ�۲췶Χ�ڵؿ���node
		for (std::set<AOI_UNIT_SUB_SCRIPT>::iterator it = setTemp.begin()
			; it != setTemp.end(); ++it)
		{
			MapInstance* pInstance = pMapRoot->GetInstance(*it);
			assert(pInstance);
			pInstance->GetNodeInPos(*it, mapWatchingNodes);
		}
		
		//��ȡ��ǰ�ؿ��� �ܱ��۲쵽��node
		AOI_UNIT_SUB_SCRIPT lPos;
		bool bRet = AOIUnits::Instance().GetMapPos(this->m_oCoordinate, lPos);
		assert(bRet);
		MapInstance* pInstance = pMapRoot->GetInstance(this->m_oCoordinate);
		assert(pInstance);

		mapWatchingNodes[this->m_dwAOIType].erase(this->m_lNodeId);

		//����������Ұ
		AOIMap<unsigned int, AOISet<NODE_ID> > mapAddWatching;
		AOIMap<unsigned int, AOISet<NODE_ID> > mapAddMutualWatching;	//���࿴���� ���۲�ľͲ�������
		for (AOIMap<unsigned int, AOISet<NODE_ID> >::iterator it1 = mapWatchingNodes.begin()
			; it1 != mapWatchingNodes.end(); ++it1)
		{
			if (GetAOIVisibilityType(this->m_dwAOIType, it1->first) <= AOIVisibilityType::AOIVisibilityType_Invisible)
			{
				continue;
			}
			
			if (GetAOIVisibilityType(this->m_dwAOIType, it1->first) == AOIVisibilityType::AOIVisibilityType_Mutual_Visibility)
			{
				//���ȼ�����
				AOIMap<double, NODE_ID> mapFirstNode;
				for (AOISet<NODE_ID>::iterator it2 = it1->second.begin()
					; it2 != it1->second.end(); ++it2)
				{
					if (this->m_mapWatching[it1->first].end() != this->m_mapWatching[it1->first].find(*it2))
					{
						continue;
					}
					//���۲�����Ұ�ɼ���Χ�ڲ��ܼ���
					if (!pInstance->CanWatching(*it2, lPos))
					{
						continue;
					}
					
					if (AOINode* pNode = AOINodeMgr::Instance().GetNode(*it2))
					{
						if (AOI_MAX_NODE_IN_VIEW > pNode->m_mapWatching[it1->first].size())
						{
							mapAddMutualWatching[it1->first].insert(*it2);
							mapFirstNode[Distance(pNode->m_oPosition, this->m_oPosition)] = *it2;
						}
					}
				}

				if (this->m_mapWatching[it1->first].size() + mapAddMutualWatching[it1->first].size() > AOI_MAX_NODE_IN_VIEW)
				{
					int dwNum = int(this->m_mapWatching[it1->first].size() + mapAddMutualWatching[it1->first].size() - AOI_MAX_NODE_IN_VIEW);
					for (AOIMap<double, NODE_ID>::iterator it3 = mapFirstNode.begin()
						; it3 != mapFirstNode.end() && 0 < dwNum; ++it3, --dwNum)
					{
						mapAddMutualWatching[it1->first].erase(it3->second);
					}
				}
			}
			else
			{
				for (AOISet<NODE_ID>::iterator it2 = it1->second.begin()
					; it2 != it1->second.end(); ++it2)
				{
					if (this->m_mapWatching[it1->first].end() == this->m_mapWatching[it1->first].find(*it2))
					{
						mapAddWatching[it1->first].insert(*it2);
					}
				}
			}
		}

		//�뿪��Ұ
		AOIMap<unsigned int, AOISet<NODE_ID> > mapDelWatching;
		AOIMap<unsigned int, AOISet<NODE_ID> > mapDelMutualWatching;
		for (std::map<unsigned int, std::set<NODE_ID> >::iterator it1 = this->m_mapWatching.begin()
			; it1 != this->m_mapWatching.end(); ++it1)
		{
			for (std::set<NODE_ID>::iterator it2 = it1->second.begin()
				; it2 != it1->second.end(); ++it2)
			{
				if (mapWatchingNodes[it1->first].end() == mapWatchingNodes[it1->first].find(*it2))
				{
					if (GetAOIVisibilityType(this->m_dwAOIType, it1->first) == AOIVisibilityType::AOIVisibilityType_Mutual_Visibility)
					{
						assert(mapAddMutualWatching[it1->first].end() == mapAddMutualWatching[it1->first].find(*it2));
						mapDelMutualWatching[it1->first].insert(*it2);
					}
					else
					{
						mapDelWatching[it1->first].insert(*it2);
					}
				}
			}
		}

		//���������ڵ�۲��б�
		AOIMap<unsigned int, AOISet<NODE_ID> > mapWatchedNodes;
		pInstance->GetWatchingInPos(lPos, mapWatchedNodes);
		mapWatchedNodes[this->m_dwAOIType].erase(this->m_lNodeId);
		
		//�����Ŀ��Կ�����ǰ�ڵ�Ľڵ�
		AOIMap<unsigned int, AOISet<NODE_ID>> mapAddWatched;
		for (AOIMap<unsigned int, AOISet<NODE_ID> >::iterator it1 = mapWatchedNodes.begin();
			it1 != mapWatchedNodes.end(); ++it1)
		{
			for (AOISet<NODE_ID>::iterator it2 = it1->second.begin();
				it2 != it1->second.end(); ++it2)
			{
				if (this->m_mapWatched[it1->first].end() != this->m_mapWatched[it1->first].find(*it2))
				{
					continue;
				}
				if (GetAOIVisibilityType(it1->first, this->m_dwAOIType) != AOIVisibilityType::AOIVisibilityType_Visible)
				{
					continue;
				}
				mapAddWatched[it1->first].insert(*it2);
			}
		}

		//Ҫɾ�����뿪�˶Է�����Ұ
		AOIMap<unsigned int, AOISet<NODE_ID> > mapDelWatched;
		for (std::map<unsigned int, std::set<NODE_ID> >::iterator it1 = this->m_mapWatched.begin();
			it1 != this->m_mapWatched.end(); ++it1)
		{
			for (std::set<NODE_ID>::iterator it2 = it1->second.begin();
				it2 != it1->second.end(); ++it2)
			{
				if (mapWatchedNodes[it1->first].end() == mapWatchedNodes[it1->first].find(*it2))
				{
					continue;
				}
				if (GetAOIVisibilityType(it1->first, this->m_dwAOIType) != AOIVisibilityType::AOIVisibilityType_Visible)
				{
					continue;
				}
				mapDelWatched[it1->first].insert(*it2);
			}
		}

		//������Ұ
		for (AOIMap<unsigned int, AOISet<NODE_ID> >::iterator it = mapAddMutualWatching.begin()
			; it != mapAddMutualWatching.end(); ++it)
		{
			mapAddWatching[it->first] = it->second;
			mapAddWatched[it->first] = it->second;
		}
		for (AOIMap<unsigned int, AOISet<NODE_ID> >::iterator it = mapDelMutualWatching.begin()
			; it != mapDelMutualWatching.end(); ++it)
		{
			mapDelWatching[it->first] = it->second;
			mapDelWatched[it->first] = it->second;
		}

		//���������Ұ
		for (AOIMap<unsigned int, AOISet<NODE_ID> >::iterator it1 = mapDelWatching.begin()
			; it1 != mapDelWatching.end(); ++it1)
		{
			for (AOISet<NODE_ID>::iterator it2 = it1->second.begin();
				it2 != it1->second.end(); ++it2)
			{
				assert(this->m_mapWatching[it1->first].end() != this->m_mapWatching[it1->first].find(*it2));
				this->m_mapWatching[it1->first].erase(*it2);

				AOINode* pNode = AOINodeMgr::Instance().GetNode(*it2);
				assert(pNode);
				assert(pNode->m_mapWatched[this->m_dwAOIType].end() != pNode->m_mapWatched[this->m_dwAOIType].find(this->m_lNodeId));
				pNode->m_mapWatched[this->m_dwAOIType].erase(this->m_lNodeId);
			}
		}

		for (AOIMap<unsigned int, AOISet<NODE_ID>>::iterator it1 = mapAddWatching.begin()
			; it1 != mapAddWatching.end(); ++it1)
		{
			for (AOISet<NODE_ID>::iterator it2 = it1->second.begin();
				it2 != it1->second.end(); ++it2)
			{
				assert(this->m_mapWatching[it1->first].end() == this->m_mapWatching[it1->first].find(*it2));
				this->m_mapWatching[it1->first].insert(*it2);

				AOINode* pNode = AOINodeMgr::Instance().GetNode(*it2);
				assert(pNode);
				assert(pNode->m_mapWatched[this->m_dwAOIType].end() == pNode->m_mapWatched[this->m_dwAOIType].find(this->m_lNodeId));
				pNode->m_mapWatched[this->m_dwAOIType].insert(this->m_lNodeId);
			}
		}

		for (AOIMap<unsigned int, AOISet<NODE_ID> >::iterator it1 = mapAddWatched.begin()
			; it1 != mapAddWatched.end(); ++it1)
		{
			for (AOISet<NODE_ID>::iterator it2 = it1->second.begin();
				it2 != it1->second.end(); ++it2)
			{
				assert(this->m_mapWatched[it1->first].end() == this->m_mapWatched[it1->first].find(*it2));
				this->m_mapWatched[it1->first].insert(*it2);

				AOINode* pNode = AOINodeMgr::Instance().GetNode(*it2);
				assert(pNode);
				assert(pNode->m_mapWatching[this->m_dwAOIType].end() == pNode->m_mapWatching[this->m_dwAOIType].find(this->m_lNodeId));
				pNode->m_mapWatching[this->m_dwAOIType].insert(this->m_lNodeId);
			}
		}
		for (AOIMap<unsigned int, AOISet<NODE_ID> >::iterator it1 = mapDelWatched.begin()
			; it1 != mapDelWatched.end(); ++it1)
		{
			for (AOISet<NODE_ID>::iterator it2 = it1->second.begin();
				it2 != it1->second.end(); ++it2)
			{
				assert(this->m_mapWatched[it1->first].end() != this->m_mapWatched[it1->first].find(*it2));
				this->m_mapWatched[it1->first].erase(*it2);

				AOINode* pNode = AOINodeMgr::Instance().GetNode(*it2);
				assert(pNode);
				assert(pNode->m_mapWatching[this->m_dwAOIType].end() != pNode->m_mapWatching[this->m_dwAOIType].find(this->m_lNodeId));
				pNode->m_mapWatching[this->m_dwAOIType].erase(this->m_lNodeId);
			}
		}
		
		//�����ӽڵ�
		AOIMap<unsigned int, AOISet<NODE_ID> > mapAddWatchingChild;
		for (AOIMap<unsigned int, AOISet<NODE_ID> >::iterator it1 = mapAddWatching.begin()
			; it1 != mapAddWatching.end(); ++it1)
		{
			for (AOISet<NODE_ID>::iterator it2 = it1->second.begin();
				it2 != it1->second.end(); ++it2)
			{
				if (AOINode* pNode = AOINodeMgr::Instance().GetNode(*it2))
				{
					for (std::set<NODE_ID>::iterator it3 = pNode->m_setChildrenNode.begin();
						it3 != pNode->m_setChildrenNode.end(); ++it3)
					{
						mapAddWatchingChild[it1->first].insert(*it3);
					}
				}
			}
		}
		for (AOIMap<unsigned int, AOISet<NODE_ID> >::iterator it1 = mapAddWatchingChild.begin()
			; it1 != mapAddWatchingChild.end(); ++it1)
		{
			for (AOISet<NODE_ID>::iterator it2 = it1->second.begin();
				it2 != it1->second.end(); ++it2)
			{
				mapAddWatching[it1->first].insert(*it2);
			}
		}

		AOIMap<unsigned int, AOISet<NODE_ID> > mapDelWatchingChild;
		for (AOIMap<unsigned int, AOISet<NODE_ID> >::iterator it1 = mapDelWatching.begin()
			; it1 != mapDelWatching.end(); ++it1)
		{
			for (AOISet<NODE_ID>::iterator it2 = it1->second.begin();
				it2 != it1->second.end(); ++it2)
			{
				if (AOINode* pNode = AOINodeMgr::Instance().GetNode(*it2))
				{
					for (std::set<NODE_ID>::iterator it3 = pNode->m_setChildrenNode.begin();
						it3 != pNode->m_setChildrenNode.end(); ++it3)
					{
						mapDelWatchingChild[it1->first].insert(*it3);
					}
				}
			}
		}
		for (AOIMap<unsigned int, AOISet<NODE_ID> >::iterator it1 = mapDelWatchingChild.begin()
			; it1 != mapDelWatchingChild.end(); ++it1)
		{
			for (AOISet<NODE_ID>::iterator it2 = it1->second.begin();
				it2 != it1->second.end(); ++it2)
			{
				mapDelWatching[it1->first].insert(*it2);
			}
		}

		AOIMap<unsigned int, AOISet<NODE_ID> > mapAddWatchedChild;
		for (AOIMap<unsigned int, AOISet<NODE_ID> >::iterator it1 = mapAddWatched.begin()
			; it1 != mapAddWatched.end(); ++it1)
		{
			for (AOISet<NODE_ID>::iterator it2 = it1->second.begin();
				it2 != it1->second.end(); ++it2)
			{
				if (AOINode* pNode = AOINodeMgr::Instance().GetNode(*it2))
				{
					for (std::set<NODE_ID>::iterator it3 = pNode->m_setChildrenNode.begin();
						it3 != pNode->m_setChildrenNode.end(); ++it3)
					{
						mapAddWatchedChild[it1->first].insert(*it3);
					}
				}
			}
		}
		for (AOIMap<unsigned int, AOISet<NODE_ID> >::iterator it1 = mapAddWatchedChild.begin()
			; it1 != mapAddWatchedChild.end(); ++it1)
		{
			for (AOISet<NODE_ID>::iterator it2 = it1->second.begin();
				it2 != it1->second.end(); ++it2)
			{
				mapAddWatched[it1->first].insert(*it2);
			}
		}

		AOIMap<unsigned int, AOISet<NODE_ID> > mapDelWatchedChild;
		for (AOIMap<unsigned int, AOISet<NODE_ID> >::iterator it1 = mapDelWatched.begin()
			; it1 != mapDelWatched.end(); ++it1)
		{
			for (AOISet<NODE_ID>::iterator it2 = it1->second.begin();
				it2 != it1->second.end(); ++it2)
			{
				if (AOINode* pNode = AOINodeMgr::Instance().GetNode(*it2))
				{
					for (std::set<NODE_ID>::iterator it3 = pNode->m_setChildrenNode.begin();
						it3 != pNode->m_setChildrenNode.end(); ++it3)
					{
						mapDelWatchedChild[it1->first].insert(*it3);
					}
				}
			}
		}
		for (AOIMap<unsigned int, AOISet<NODE_ID> >::iterator it1 = mapDelWatchedChild.begin()
			; it1 != mapDelWatchedChild.end(); ++it1)
		{
			for (AOISet<NODE_ID>::iterator it2 = it1->second.begin();
				it2 != it1->second.end(); ++it2)
			{
				mapDelWatched[it1->first].insert(*it2);
			}
		}

		// //�˽ڵ��յ��ĳ��ְ�
		// mapAddWatching;
		// //�˽ڵ��յ�����ʧ��
		// mapDelWatching;
		
		// //�����ڵ��յ��ĳ��ְ�
		// mapAddWatched;
		// //�����ڵ��յ�����ʧ��
		// mapDelWatched;
		AOISystem::Instance().GetAoiOperator()(m_lNodeId, mapAddWatching, mapDelWatching, mapAddWatched, mapDelWatched);
		for (std::set<NODE_ID>::iterator it = this->m_setChildrenNode.begin();
			it != this->m_setChildrenNode.end(); ++it)
		{
			AOISystem::Instance().GetAoiOperator()(*it, mapAddWatching, mapDelWatching, mapAddWatched, mapDelWatched);
		}
		
	}

	void AOINode::AddChild(NODE_ID lNodeId)
	{
		AOIMap<unsigned int, AOISet<NODE_ID> > mapAddWatching;
		AOIMap<unsigned int, AOISet<NODE_ID> > mapDelWatching;
		AOIMap<unsigned int, AOISet<NODE_ID> > mapAddWatched;
		AOIMap<unsigned int, AOISet<NODE_ID> > mapDelWatched;
		for (std::map<unsigned int, std::set< NODE_ID> >::iterator it1 = this->m_mapWatching.begin()
			; it1 != this->m_mapWatching.end(); ++it1)
		{
			for (std::set< NODE_ID>::iterator it2 = it1->second.begin();
				it2 != it1->second.end(); ++it2)
			{
				mapAddWatching[it1->first].insert(*it2);
			}
		}
		for (std::map<unsigned int, std::set< NODE_ID> >::iterator it1 = this->m_mapWatched.begin()
			; it1 != this->m_mapWatched.end(); ++it1)
		{
			for (std::set< NODE_ID>::iterator it2 = it1->second.begin();
				it2 != it1->second.end(); ++it2)
			{
				mapAddWatching[it1->first].insert(*it2);
			}
		}
		AOISystem::Instance().GetAoiOperator()(lNodeId, mapAddWatching, mapDelWatched, mapAddWatched, mapDelWatched);
		this->m_setChildrenNode.insert(lNodeId);
	}

	void AOINode::RemoveChild(NODE_ID lNodeId)
	{
		AOIMap<unsigned int, AOISet<NODE_ID> > mapAddWatching;
		AOIMap<unsigned int, AOISet<NODE_ID> > mapDelWatching;
		AOIMap<unsigned int, AOISet<NODE_ID> > mapAddWatched;
		AOIMap<unsigned int, AOISet<NODE_ID> > mapDelWatched;
		for (std::map<unsigned int, std::set< NODE_ID> >::iterator it1 = this->m_mapWatching.begin()
			; it1 != this->m_mapWatching.end(); ++it1)
		{
			for (std::set< NODE_ID>::iterator it2 = it1->second.begin();
				it2 != it1->second.end(); ++it2)
			{
				mapDelWatching[it1->first].insert(*it2);
			}
		}
		for (std::map<unsigned int, std::set< NODE_ID> >::iterator it1 = this->m_mapWatched.begin()
			; it1 != this->m_mapWatched.end(); ++it1)
		{
			for (std::set< NODE_ID>::iterator it2 = it1->second.begin();
				it2 != it1->second.end(); ++it2)
			{
				mapDelWatched[it1->first].insert(*it2);
			}
		}
		AOISystem::Instance().GetAoiOperator()(lNodeId, mapAddWatching, mapDelWatched, mapAddWatched, mapDelWatched);
		this->m_setChildrenNode.erase(lNodeId);
	}

	void AOINode::Debug(std::ostream& refOstream)
	{
		refOstream << "AOINode:" << this->m_lNodeId << ",AOIType:" << this->m_dwAOIType << ",MapId:" << this->m_dwMapId;
		refOstream << ",Children:{";
		for (std::set<NODE_ID>::iterator it = this->m_setChildrenNode.begin();
			it != this->m_setChildrenNode.end(); ++it)
		{
			refOstream << *it << ",";
		}
		refOstream << "},";
		
		this->m_oCoordinate.Debug(refOstream);
		this->m_oPosition.Debug(refOstream);
		refOstream << ",Watching:{";
		for (std::map<unsigned int, std::set< NODE_ID> >::iterator it1 = this->m_mapWatching.begin()
			; it1 != this->m_mapWatching.end(); ++it1)
		{
			refOstream << "type_" << it1->first << ":{";
			for (std::set< NODE_ID>::iterator it2 = it1->second.begin();
				it2 != it1->second.end(); ++it2)
			{
				refOstream << *it2 << ",";
			}
			refOstream << "},";
		}
		refOstream << "}";
		refOstream << ",Watched:{";
		for (std::map<unsigned int, std::set< NODE_ID> >::iterator it1 = this->m_mapWatched.begin()
			; it1 != this->m_mapWatched.end(); ++it1)
		{
			refOstream << "type_" << it1->first << ":{";
			for (std::set< NODE_ID>::iterator it2 = it1->second.begin();
				it2 != it1->second.end(); ++it2)
			{
				refOstream << *it2 << ",";
			}
			refOstream << "},";
		}
		refOstream << "}";
	}
} // namespace FXAOI
