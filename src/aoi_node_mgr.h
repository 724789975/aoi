#ifndef __AOI_NODE_MGR_H__
#define __AOI_NODE_MGR_H__

#include "aoi_define.h"
#include "aoi_node.h"

#include <unordered_map>

namespace FXAOI
{
	class AOINodeMgr
	{
	public:
		static AOINodeMgr& Instance() {return s_oInstance;}

		AOINode* AddNode(NODE_ID lNodeId, unsigned int dwAOIType, unsigned int dwWatchedRadius, unsigned int dwWatchingRadius);

		AOINode* GetNode(NODE_ID lNodeId);
		bool RemoveNode(NODE_ID lNodeId);
	protected:
	private:
		static AOINodeMgr s_oInstance;
		std::unordered_map<NODE_ID, AOINode> m_mapNodes;
	};
} // namespace FXAOI


#endif