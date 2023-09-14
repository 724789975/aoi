#include "aoi_node_mgr.h"

namespace FXAOI
{
	AOINodeMgr AOINodeMgr::s_oInstace;

	AOINode* AOINodeMgr::AddNode(NODE_ID lNodeId, unsigned int dwAOIType, unsigned int dwWatchedRadius, unsigned int dwWatchingRadius)
	{
		std::unordered_map<NODE_ID, AOINode>::iterator it = this->m_mapNodes.find(lNodeId);
		if (this->m_mapNodes.end() != it)
		{
			return &it->second;
		}
		
		return &(this->m_mapNodes[lNodeId] = AOINode(lNodeId, dwAOIType, dwWatchedRadius, dwWatchingRadius));
	}

	AOINode* AOINodeMgr::GetNode(NODE_ID lNodeId)
	{
		std::unordered_map<NODE_ID, AOINode>::iterator it = this->m_mapNodes.find(lNodeId);
		if (this->m_mapNodes.end() != it)
		{
			return 0;
		}

		return &it->second;
	}

	bool AOINodeMgr::RemoveNode(NODE_ID lNodeId)
	{
		if (this->m_mapNodes.end() == this->m_mapNodes.find(lNodeId))
		{
			return false;
		}

		this->m_mapNodes.erase(lNodeId);
		return true;
	}
} // namespace FXAOI

