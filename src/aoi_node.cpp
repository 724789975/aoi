#include "aoi_node.h"
#include "aoi_map_instance.h"
#include "aoi_node_mgr.h"
#include "../include/aoi_define.h"
#include "../include/aoi_system.h"

#include "arr_map.h"
#include "arr_set.h"
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
		, m_oPosition{ 0
#if AOI_USE_Y_AXIS
			, 0
#endif
            , 0 }
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
		, m_oPosition{ 0
#if AOI_USE_Y_AXIS
			, 0
#endif
            , 0 }
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
		class Reserve1
		{
		public:
			const Reserve1& operator()(ArrMap<unsigned int, ArrSet<NODE_ID> >& refNodes, unsigned int dwSize) const
			{
				refNodes.reserve(dwSize);
				return *this;
			}
			const Reserve1& operator()(std::map<unsigned int, std::set<NODE_ID> >& refNodes, unsigned int dwSize) const
			{
				return *this;
			}
		};
		class Reserve2
		{
		public:
			const Reserve2& operator()(ArrSet<NODE_ID>& refNodes, unsigned int dwSize) const
			{
				refNodes.reserve(dwSize);
				return *this;
			}
			const Reserve2& operator()(std::set<NODE_ID>& refNodes, unsigned int dwSize) const
			{
				return *this;
			}
		};
		AOISet<AOI_UNIT_SUB_SCRIPT> setTemp;
		setTemp.swap(this->m_setTempWatchingMap);

		AOIMap<unsigned int, AOISet<NODE_ID> > mapWatchingNodes;
		//获取观察范围内地块中node
		for (AOISet<AOI_UNIT_SUB_SCRIPT>::iterator it = setTemp.begin()
			; it != setTemp.end(); ++it)
		{
			MapInstance* pInstance = pMapRoot->GetInstance(*it);
			assert(pInstance);
			pInstance->GetNodeInPos(*it, mapWatchingNodes);
		}
		AOI_UNIT_SUB_SCRIPT lPos;
		bool bRet = AOIUnits::Instance().GetMapPos(this->m_oCoordinate, lPos);
		assert(bRet);
		MapInstance* pInstance = pMapRoot->GetInstance(this->m_oCoordinate);
		assert(pInstance);

		//在地图中
		if (setTemp.size())
		{
			//处理别的节点能被本节点看到
			pInstance->GetWatchedInPos(lPos, mapWatchingNodes);
		}
		AOIMap<unsigned int, AOISet<NODE_ID> >::iterator it = mapWatchingNodes.find(this->m_dwAOIType);
		if (mapWatchingNodes.end() != it)
		{
			it->second.erase(this->m_lNodeId);
		}
		
		//新增进入视野
		AOIMap<unsigned int, AOISet<NODE_ID> > mapAddWatching;
		AOIMap<unsigned int, AOISet<NODE_ID> > mapAddMutualWatching;	//互相看见的 被观察的就不计算了
		Reserve1()(mapAddWatching, mapWatchingNodes.size());
		for (AOIMap<unsigned int, AOISet<NODE_ID> >::iterator it1 = mapWatchingNodes.begin()
			; it1 != mapWatchingNodes.end(); ++it1)
		{
			AOISet<NODE_ID>& refAddWatching = mapAddWatching[it1->first];
			AOISet<NODE_ID>& refAddMutualWatching = mapAddMutualWatching[it1->first];
			AOISet<NODE_ID>& refWatching = this->m_mapWatching[it1->first];
			if (!it1->second.size())
			{
				continue;
			}
			if (AOISystem::Instance().GetAOIVisibilityType(this->m_dwAOIType, it1->first) <= AOIVisibilityType::AOIVisibilityType_Invisible)
			{
				continue;
			}
			
			if (AOISystem::Instance().GetAOIVisibilityType(this->m_dwAOIType, it1->first) == AOIVisibilityType::AOIVisibilityType_Mutual_Visibility)
			{
				//优先级队列
				Reserve2()(refAddWatching, it1->second.size());
				Reserve2()(refAddMutualWatching, it1->second.size());
				AOIMap<double, NODE_ID> mapFirstNode;
				for (AOISet<NODE_ID>::iterator it2 = it1->second.begin()
					; it2 != it1->second.end(); ++it2)
				{
					if (refWatching.end() != refWatching.find(*it2))
					{
						continue;
					}
					//被观察者视野可及范围内才能加入
					if (!pInstance->CanWatching(*it2, lPos))
					{
						continue;
					}
					
					if (AOINode* pNode = AOINodeMgr::Instance().GetNode(*it2))
					{
						if (AOI_MAX_NODE_IN_VIEW > pNode->m_mapWatching[it1->first].size())
						{
							refAddMutualWatching.insert(*it2);
							mapFirstNode[Distance(pNode->m_oPosition, this->m_oPosition)] = *it2;
						}
					}
				}

				if (refWatching.size() + refAddMutualWatching.size() > AOI_MAX_NODE_IN_VIEW)
				{
					int dwNum = int(refWatching.size() + refAddMutualWatching.size() - AOI_MAX_NODE_IN_VIEW);
					for (AOIMap<double, NODE_ID>::iterator it3 = mapFirstNode.begin()
						; it3 != mapFirstNode.end() && 0 < dwNum; ++it3, --dwNum)
					{
						refAddMutualWatching.erase(it3->second);
					}
				}
			}
			else
			{
				for (AOISet<NODE_ID>::iterator it2 = it1->second.begin()
					; it2 != it1->second.end(); ++it2)
				{
					if (refWatching.end() == refWatching.find(*it2))
					{
						refAddWatching.insert(*it2);
					}
				}
			}
		}

		//离开视野
		AOIMap<unsigned int, AOISet<NODE_ID> > mapDelWatching;
		AOIMap<unsigned int, AOISet<NODE_ID> > mapDelMutualWatching;
		for (AOIMap<unsigned int, AOISet<NODE_ID> >::iterator it1 = this->m_mapWatching.begin()
			; it1 != this->m_mapWatching.end(); ++it1)
		{
			if (!it1->second.size())
			{
				continue;
			}

			AOISet<NODE_ID>& refDelWatching = mapDelWatching[it1->first];
			AOISet<NODE_ID>& refDelMutualWatching = mapDelMutualWatching[it1->first];
			AOISet<NODE_ID>& refAddMutualWatching = mapAddMutualWatching[it1->first];
			AOISet<NODE_ID>& refWatchingNodes = mapWatchingNodes[it1->first];
			Reserve2()(refDelWatching, it1->second.size())
				(refDelMutualWatching, it1->second.size());
			for (AOISet<NODE_ID>::iterator it2 = it1->second.begin()
				; it2 != it1->second.end(); ++it2)
			{
				if (refWatchingNodes.end() == refWatchingNodes.find(*it2))
				{
					if (AOISystem::Instance().GetAOIVisibilityType(this->m_dwAOIType, it1->first) == AOIVisibilityType::AOIVisibilityType_Mutual_Visibility)
					{
						assert(refAddMutualWatching.end() == refAddMutualWatching.find(*it2));
						refDelMutualWatching.insert(*it2);
					}
					else
					{
						refDelWatching.insert(*it2);
					}
				}
			}
		}

		//处理被其他节点观察列表
		AOIMap<unsigned int, AOISet<NODE_ID> > mapWatchedNodes;
		//如果这个长度为0 那么是离开地图
		if (setTemp.size())
		{
			pInstance->GetWatchingInPos(lPos, mapWatchedNodes);
			if (mapWatchedNodes.end() != mapWatchedNodes.find(this->m_dwAOIType))
			{
				mapWatchedNodes[this->m_dwAOIType].erase(this->m_lNodeId);
			}
		}
		
		//新增的可以看见当前节点的节点
		AOIMap<unsigned int, AOISet<NODE_ID> > mapAddWatched;
		for (AOIMap<unsigned int, AOISet<NODE_ID> >::iterator it1 = mapWatchedNodes.begin();
			it1 != mapWatchedNodes.end(); ++it1)
		{
			AOISet<NODE_ID>& refAddWatched = mapAddWatched[it1->first];
			AOISet<NODE_ID>& refWatched = this->m_mapWatched[it1->first];
			Reserve2()(refAddWatched, it1->second.size());
			for (AOISet<NODE_ID>::iterator it2 = it1->second.begin();
				it2 != it1->second.end(); ++it2)
			{
				if (refWatched.end() != refWatched.find(*it2))
				{
					continue;
				}
				if (AOISystem::Instance().GetAOIVisibilityType(it1->first, this->m_dwAOIType) != AOIVisibilityType::AOIVisibilityType_Visible)
				{
					continue;
				}
				refAddWatched.insert(*it2);
			}
		}

		//要删除的离开了对方的视野
		AOIMap<unsigned int, AOISet<NODE_ID> > mapDelWatched;
		for (AOIMap<unsigned int, AOISet<NODE_ID> >::iterator it1 = this->m_mapWatched.begin();
			it1 != this->m_mapWatched.end(); ++it1)
		{
			AOISet<NODE_ID>& refDelWatched = mapDelWatched[it1->first];
			AOISet<NODE_ID>& refWatchedNodes = mapWatchedNodes[it1->first];
			Reserve2()(refDelWatched, it1->second.size());
			for (AOISet<NODE_ID>::iterator it2 = it1->second.begin();
				it2 != it1->second.end(); ++it2)
			{
				if (refWatchedNodes.end() != refWatchedNodes.find(*it2))
				{
					continue;
				}
				if (AOISystem::Instance().GetAOIVisibilityType(it1->first, this->m_dwAOIType) != AOIVisibilityType::AOIVisibilityType_Visible)
				{
					continue;
				}
				refDelWatched.insert(*it2);
			}
		}

		//计算视野
		for (AOIMap<unsigned int, AOISet<NODE_ID> >::iterator it = mapAddMutualWatching.begin()
			; it != mapAddMutualWatching.end(); ++it)
		{
			if (it->second.size())
			{
				mapAddWatching[it->first] = it->second;
				mapAddWatched[it->first] = it->second;
			}
		}
		for (AOIMap<unsigned int, AOISet<NODE_ID> >::iterator it = mapDelMutualWatching.begin()
			; it != mapDelMutualWatching.end(); ++it)
		{
			if (it->second.size())
			{
				mapDelWatching[it->first] = it->second;
				mapDelWatched[it->first] = it->second;
			}
		}

		//处理关联视野
		for (AOIMap<unsigned int, AOISet<NODE_ID> >::iterator it1 = mapDelWatching.begin()
			; it1 != mapDelWatching.end(); ++it1)
		{
			AOISet<NODE_ID>& refWatching = this->m_mapWatching[it1->first];
			for (AOISet<NODE_ID>::iterator it2 = it1->second.begin();
				it2 != it1->second.end(); ++it2)
			{
				assert(refWatching.end() != refWatching.find(*it2));
				refWatching.erase(*it2);

				AOINode* pNode = AOINodeMgr::Instance().GetNode(*it2);
				assert(pNode);
				assert(pNode->m_mapWatched[this->m_dwAOIType].end() != pNode->m_mapWatched[this->m_dwAOIType].find(this->m_lNodeId));
				pNode->m_mapWatched[this->m_dwAOIType].erase(this->m_lNodeId);
			}
		}

		for (AOIMap<unsigned int, AOISet<NODE_ID>>::iterator it1 = mapAddWatching.begin()
			; it1 != mapAddWatching.end(); ++it1)
		{
			AOISet<NODE_ID>& refWatching = this->m_mapWatching[it1->first];
			for (AOISet<NODE_ID>::iterator it2 = it1->second.begin();
				it2 != it1->second.end(); ++it2)
			{
				assert(refWatching.end() == refWatching.find(*it2));
				refWatching.insert(*it2);

				AOINode* pNode = AOINodeMgr::Instance().GetNode(*it2);
				assert(pNode);
				assert(pNode->m_mapWatched[this->m_dwAOIType].end() == pNode->m_mapWatched[this->m_dwAOIType].find(this->m_lNodeId));
				pNode->m_mapWatched[this->m_dwAOIType].insert(this->m_lNodeId);
			}
		}

		for (AOIMap<unsigned int, AOISet<NODE_ID> >::iterator it1 = mapAddWatched.begin()
			; it1 != mapAddWatched.end(); ++it1)
		{
			AOISet<NODE_ID>& refWatched = this->m_mapWatched[it1->first];
			for (AOISet<NODE_ID>::iterator it2 = it1->second.begin();
				it2 != it1->second.end(); ++it2)
			{
				assert(refWatched.end() == refWatched.find(*it2));
				refWatched.insert(*it2);

				AOINode* pNode = AOINodeMgr::Instance().GetNode(*it2);
				assert(pNode);
				assert(pNode->m_mapWatching[this->m_dwAOIType].end() == pNode->m_mapWatching[this->m_dwAOIType].find(this->m_lNodeId));
				pNode->m_mapWatching[this->m_dwAOIType].insert(this->m_lNodeId);
			}
		}
		for (AOIMap<unsigned int, AOISet<NODE_ID> >::iterator it1 = mapDelWatched.begin()
			; it1 != mapDelWatched.end(); ++it1)
		{
			AOISet<NODE_ID>& refWatched = this->m_mapWatched[it1->first];
			for (AOISet<NODE_ID>::iterator it2 = it1->second.begin();
				it2 != it1->second.end(); ++it2)
			{
				assert(refWatched.end() != refWatched.find(*it2));
				refWatched.erase(*it2);

				AOINode* pNode = AOINodeMgr::Instance().GetNode(*it2);
				assert(pNode);
				assert(pNode->m_mapWatching[this->m_dwAOIType].end() != pNode->m_mapWatching[this->m_dwAOIType].find(this->m_lNodeId));
				pNode->m_mapWatching[this->m_dwAOIType].erase(this->m_lNodeId);
			}
		}
		
		//处理子节点
		AOIMap<unsigned int, AOISet<NODE_ID> > mapAddWatchingChild;
		for (AOIMap<unsigned int, AOISet<NODE_ID> >::iterator it1 = mapAddWatching.begin()
			; it1 != mapAddWatching.end(); ++it1)
		{
			AOISet<NODE_ID>& refAddWatchingChild = mapAddWatchingChild[it1->first];
			Reserve2()(refAddWatchingChild, it1->second.size());
			for (AOISet<NODE_ID>::iterator it2 = it1->second.begin();
				it2 != it1->second.end(); ++it2)
			{
				if (AOINode* pNode = AOINodeMgr::Instance().GetNode(*it2))
				{
					for (AOISet<NODE_ID>::iterator it3 = pNode->m_setChildrenNode.begin();
						it3 != pNode->m_setChildrenNode.end(); ++it3)
					{
						refAddWatchingChild.insert(*it3);
					}
				}
			}
		}
		for (AOIMap<unsigned int, AOISet<NODE_ID> >::iterator it1 = mapAddWatchingChild.begin()
			; it1 != mapAddWatchingChild.end(); ++it1)
		{
			AOISet<NODE_ID>& refAddWatching = mapAddWatching[it1->first];
			for (AOISet<NODE_ID>::iterator it2 = it1->second.begin();
				it2 != it1->second.end(); ++it2)
			{
				refAddWatching.insert(*it2);
			}
		}

		AOIMap<unsigned int, AOISet<NODE_ID> > mapDelWatchingChild;
		for (AOIMap<unsigned int, AOISet<NODE_ID> >::iterator it1 = mapDelWatching.begin()
			; it1 != mapDelWatching.end(); ++it1)
		{
			AOISet<NODE_ID>& refDelWatchingChild = mapDelWatchingChild[it1->first];
			Reserve2()(refDelWatchingChild, it1->second.size());
			for (AOISet<NODE_ID>::iterator it2 = it1->second.begin();
				it2 != it1->second.end(); ++it2)
			{
				if (AOINode* pNode = AOINodeMgr::Instance().GetNode(*it2))
				{
					for (AOISet<NODE_ID>::iterator it3 = pNode->m_setChildrenNode.begin();
						it3 != pNode->m_setChildrenNode.end(); ++it3)
					{
						refDelWatchingChild.insert(*it3);
					}
				}
			}
		}
		for (AOIMap<unsigned int, AOISet<NODE_ID> >::iterator it1 = mapDelWatchingChild.begin()
			; it1 != mapDelWatchingChild.end(); ++it1)
		{
			AOISet<NODE_ID>& refDelWatching = mapDelWatching[it1->first];
			for (AOISet<NODE_ID>::iterator it2 = it1->second.begin();
				it2 != it1->second.end(); ++it2)
			{
				refDelWatching.insert(*it2);
			}
		}

		AOIMap<unsigned int, AOISet<NODE_ID> > mapAddWatchedChild;
		for (AOIMap<unsigned int, AOISet<NODE_ID> >::iterator it1 = mapAddWatched.begin()
			; it1 != mapAddWatched.end(); ++it1)
		{
			AOISet<NODE_ID>& refAddWatchedChild = mapAddWatchedChild[it1->first];
			Reserve2()(refAddWatchedChild, it1->second.size());
			for (AOISet<NODE_ID>::iterator it2 = it1->second.begin();
				it2 != it1->second.end(); ++it2)
			{
				if (AOINode* pNode = AOINodeMgr::Instance().GetNode(*it2))
				{
					for (AOISet<NODE_ID>::iterator it3 = pNode->m_setChildrenNode.begin();
						it3 != pNode->m_setChildrenNode.end(); ++it3)
					{
						refAddWatchedChild.insert(*it3);
					}
				}
			}
		}
		for (AOIMap<unsigned int, AOISet<NODE_ID> >::iterator it1 = mapAddWatchedChild.begin()
			; it1 != mapAddWatchedChild.end(); ++it1)
		{
			AOISet<NODE_ID>& refAddWatched = mapAddWatched[it1->first];
			for (AOISet<NODE_ID>::iterator it2 = it1->second.begin();
				it2 != it1->second.end(); ++it2)
			{
				refAddWatched.insert(*it2);
			}
		}

		AOIMap<unsigned int, AOISet<NODE_ID> > mapDelWatchedChild;
		for (AOIMap<unsigned int, AOISet<NODE_ID> >::iterator it1 = mapDelWatched.begin()
			; it1 != mapDelWatched.end(); ++it1)
		{
			AOISet<NODE_ID>& refDelWatchedChild = mapDelWatchedChild[it1->first];
			Reserve2()(refDelWatchedChild, it1->second.size());
			for (AOISet<NODE_ID>::iterator it2 = it1->second.begin();
				it2 != it1->second.end(); ++it2)
			{
				if (AOINode* pNode = AOINodeMgr::Instance().GetNode(*it2))
				{
					for (AOISet<NODE_ID>::iterator it3 = pNode->m_setChildrenNode.begin();
						it3 != pNode->m_setChildrenNode.end(); ++it3)
					{
						refDelWatchedChild.insert(*it3);
					}
				}
			}
		}
		for (AOIMap<unsigned int, AOISet<NODE_ID> >::iterator it1 = mapDelWatchedChild.begin()
			; it1 != mapDelWatchedChild.end(); ++it1)
		{
			AOISet<NODE_ID>& refDelWatched = mapDelWatched[it1->first];
			for (AOISet<NODE_ID>::iterator it2 = it1->second.begin();
				it2 != it1->second.end(); ++it2)
			{
				refDelWatched.insert(*it2);
			}
		}

		// //此节点收到的出现包
		// mapAddWatching;
		// //此节点收到的消失包
		// mapDelWatching;
		
		// //其他节点收到的出现包
		// mapAddWatched;
		// //其他节点收到的消失包
		// mapDelWatched;

		std::vector<AoiOperatorInfo> vecAddWatching;
		std::vector<AoiOperatorInfo> vecDelWatching;
		std::vector<AoiOperatorInfo> vecAddWatched;
		std::vector<AoiOperatorInfo> vecDelWatched;

		class Map2VecOp
		{
		public:
			const Map2VecOp& operator()(std::vector<AoiOperatorInfo>& vecDest, ArrMap<unsigned int, ArrSet<NODE_ID> >& mapSrc) const
			{
				vecDest.reserve(mapSrc.size());
				for (ArrMap<unsigned int, ArrSet<NODE_ID> >::iterator it1 = mapSrc.begin()
					; it1 != mapSrc.end(); ++it1)
				{
					if (!it1->second.size())
					{
						continue;
					}
					vecDest.push_back(AoiOperatorInfo());
					AoiOperatorInfo& refInfo = vecDest.back();
					refInfo.m_dwType = it1->first;
					refInfo.m_vecNodes.reserve(it1->second.size());
					memcpy(refInfo.m_vecNodes.data(), it1->second.begin(), it1->second.size());
				}
				return *this;
			};

			const Map2VecOp& operator()(std::vector<AoiOperatorInfo>& vecDest, std::map<unsigned int, std::set<NODE_ID> >& mapSrc) const
			{
				vecDest.reserve(mapSrc.size());
				for (std::map<unsigned int, std::set<NODE_ID> >::iterator it1 = mapSrc.begin()
					; it1 != mapSrc.end(); ++it1)
				{
					if (!it1->second.size())
					{
						continue;
					}
					vecDest.push_back(AoiOperatorInfo());
					AoiOperatorInfo& refInfo = vecDest.back();
					refInfo.m_dwType = it1->first;
					refInfo.m_vecNodes.reserve(it1->second.size());
					for (std::set<NODE_ID>::iterator it2 = it1->second.begin();
						it2 != it1->second.end(); ++it2)
					{
						refInfo.m_vecNodes.push_back(*it2);
					}
				}
				return *this;
			};
		};
		Map2VecOp()(vecAddWatching, mapAddWatching)
			(vecDelWatching, mapDelWatching)
			(vecAddWatched, mapAddWatched)
			(vecDelWatched, mapDelWatched);

		AOISystem::Instance().GetAoiOperator()(m_lNodeId, vecAddWatching, vecDelWatching, vecAddWatched, vecDelWatched);
		for (AOISet<NODE_ID>::iterator it = this->m_setChildrenNode.begin();
			it != this->m_setChildrenNode.end(); ++it)
		{
			AOISystem::Instance().GetAoiOperator()(*it, vecAddWatching, vecDelWatching, vecAddWatched, vecDelWatched);
		}
		
	}

	void AOINode::AddChild(NODE_ID lNodeId)
	{
		class Map2VecOp
		{
		public:
			const Map2VecOp& operator()(std::vector<AoiOperatorInfo>& vecDest, ArrMap<unsigned int, ArrSet<NODE_ID> >& mapSrc) const
			{
				for (ArrMap<unsigned int, ArrSet<NODE_ID> >::iterator it1 = mapSrc.begin()
					; it1 != mapSrc.end(); ++it1)
				{
					vecDest.push_back(AoiOperatorInfo());
					AoiOperatorInfo& refInfo = vecDest.back();
					refInfo.m_dwType = it1->first;
					refInfo.m_vecNodes.reserve(it1->second.size());
					memcpy(refInfo.m_vecNodes.data(), it1->second.begin(), it1->second.size());
				}
				return *this;
			};

			const Map2VecOp& operator()(std::vector<AoiOperatorInfo>& vecDest, std::map<unsigned int, std::set<NODE_ID> >& mapSrc) const
			{
				for (std::map<unsigned int, std::set<NODE_ID> >::iterator it1 = mapSrc.begin()
					; it1 != mapSrc.end(); ++it1)
				{
					vecDest.push_back(AoiOperatorInfo());
					AoiOperatorInfo& refInfo = vecDest.back();
					refInfo.m_dwType = it1->first;
					refInfo.m_vecNodes.reserve(it1->second.size());
					for (std::set<NODE_ID>::iterator it2 = it1->second.begin();
						it2 != it1->second.end(); ++it2)
					{
						refInfo.m_vecNodes.push_back(*it2);
					}
				}
				return *this;
			};
		};

		std::vector<AoiOperatorInfo> vecAddWatching;
		std::vector<AoiOperatorInfo> vecDelWatching;
		std::vector<AoiOperatorInfo> vecAddWatched;
		std::vector<AoiOperatorInfo> vecDelWatched;

		Map2VecOp()(vecAddWatching, this->m_mapWatching)
			(vecAddWatched, this->m_mapWatched);
		AOISystem::Instance().GetAoiOperator()(lNodeId, vecAddWatching, vecDelWatching, vecAddWatched, vecDelWatched);
		this->m_setChildrenNode.insert(lNodeId);
	}

	void AOINode::RemoveChild(NODE_ID lNodeId)
	{
		class Map2VecOp
		{
		public:
			const Map2VecOp& operator()(std::vector<AoiOperatorInfo>& vecDest, ArrMap<unsigned int, ArrSet<NODE_ID> >& mapSrc) const
			{
				for (ArrMap<unsigned int, ArrSet<NODE_ID> >::iterator it1 = mapSrc.begin()
					; it1 != mapSrc.end(); ++it1)
				{
					vecDest.push_back(AoiOperatorInfo());
					AoiOperatorInfo& refInfo = vecDest.back();
					refInfo.m_dwType = it1->first;
					refInfo.m_vecNodes.reserve(it1->second.size());
					memcpy(refInfo.m_vecNodes.data(), it1->second.begin(), it1->second.size());
				}
				return *this;
			};

			const Map2VecOp& operator()(std::vector<AoiOperatorInfo>& vecDest, std::map<unsigned int, std::set<NODE_ID> >& mapSrc) const
			{
				for (std::map<unsigned int, std::set<NODE_ID> >::iterator it1 = mapSrc.begin()
					; it1 != mapSrc.end(); ++it1)
				{
					vecDest.push_back(AoiOperatorInfo());
					AoiOperatorInfo& refInfo = vecDest.back();
					refInfo.m_dwType = it1->first;
					refInfo.m_vecNodes.reserve(it1->second.size());
					for (std::set<NODE_ID>::iterator it2 = it1->second.begin();
						it2 != it1->second.end(); ++it2)
					{
						refInfo.m_vecNodes.push_back(*it2);
					}
				}
				return *this;
			};
		};

		std::vector<AoiOperatorInfo> vecAddWatching;
		std::vector<AoiOperatorInfo> vecDelWatching;
		std::vector<AoiOperatorInfo> vecAddWatched;
		std::vector<AoiOperatorInfo> vecDelWatched;

		Map2VecOp()(vecDelWatching, this->m_mapWatching)
			(vecDelWatched, this->m_mapWatched);

		AOISystem::Instance().GetAoiOperator()(lNodeId, vecAddWatching, vecDelWatching, vecAddWatched, vecDelWatched);
		this->m_setChildrenNode.erase(lNodeId);
	}

	void AOINode::AfterLeaveMap()
	{
		this->m_dwMapId = -1;
		
		for (AOIMap<unsigned int, AOISet< NODE_ID> >::iterator it1 = this->m_mapWatched.begin()
			; it1 != this->m_mapWatched.end(); ++it1)
		{
			assert(0 == it1->second.size());
		}
		this->m_mapWatched.clear();
		for (AOIMap<unsigned int, AOISet< NODE_ID> >::iterator it1 = this->m_mapWatching.begin()
			; it1 != this->m_mapWatching.end(); ++it1)
		{
			assert(0 == it1->second.size());
		}
		this->m_mapWatching.clear();
		assert(0 == m_setTempWatchingMap.size());
		this->m_setTempWatchingMap.clear();
	}

	void AOINode::Debug(std::ostream& refOstream)
	{
		refOstream << "AOINode:" << this->m_lNodeId << ",AOIType:" << this->m_dwAOIType << ",MapId:" << this->m_dwMapId;
		refOstream << ",Children:{";
		for (AOISet<NODE_ID>::iterator it = this->m_setChildrenNode.begin();
			it != this->m_setChildrenNode.end(); ++it)
		{
			refOstream << *it << ",";
		}
		refOstream << "},";
		
		this->m_oCoordinate.Debug(refOstream);
		this->m_oPosition.Debug(refOstream);
		refOstream << ",Watching:{";
		for (AOIMap<unsigned int, AOISet< NODE_ID> >::iterator it1 = this->m_mapWatching.begin()
			; it1 != this->m_mapWatching.end(); ++it1)
		{
			refOstream << "type_" << it1->first << ":{";
			for (AOISet< NODE_ID>::iterator it2 = it1->second.begin();
				it2 != it1->second.end(); ++it2)
			{
				refOstream << *it2 << ",";
			}
			refOstream << "},";
		}
		refOstream << "}";
		refOstream << ",Watched:{";
		for (AOIMap<unsigned int, AOISet< NODE_ID> >::iterator it1 = this->m_mapWatched.begin()
			; it1 != this->m_mapWatched.end(); ++it1)
		{
			refOstream << "type_" << it1->first << ":{";
			for (AOISet< NODE_ID>::iterator it2 = it1->second.begin();
				it2 != it1->second.end(); ++it2)
			{
				refOstream << *it2 << ",";
			}
			refOstream << "},";
		}
		refOstream << "}";
	}
} // namespace FXAOI
