#ifndef __AOI_MAP_INSTANCE_H__
#define __AOI_MAP_INSTANCE_H__

#include "aoi_define.h"
#include "aoi_node.h"

#include <unordered_map>
#include <unordered_set>

namespace AOI
{
	class MapInstance
	{
	public:
		MapInstance(unsigned int dwMapId, unsigned int dwDivideNum)
			: m_dwMapId(dwMapId)
			, m_dwDivideNum(dwDivideNum)
			, m_bDivided(false)
		{}

		void Divide(){}
		void Merge(){}

		void Enter(AOINode& refNode){}
		void Leave(AOINode& refNode){}

	protected:
	private:
		unsigned int m_dwMapId;
		/**
		 * @brief 
		 * 这个地块的分裂次数 最多分裂8次
		 */
		unsigned int m_dwDivideNum;
		/**
		 * @brief 
		 * 是否已分裂
		 */
		bool m_bDivided;
		/**
		 * @brief 
		 * 已分裂地块
		 */
		std::unordered_map<unsigned int, MapInstance> m_mapDividedMap;
		/**
		 * @brief 
		 * 列表中的节点可被格子中的节点看见
		 */
		std::unordered_set<NODE_ID> m_setWatched;
		/**
		 * @brief 
		 * 列表中的节点可看见格子中的节点
		 */
		std::unordered_set<NODE_ID> m_setWatching;
		/**
		 * @brief 
		 * <节点id, 地块id>
		 */
		std::unordered_map<NODE_ID, AOI_UNIT_SUB_SCRIPT> m_mapNodeChunk;
	};
} // namespace AOI

#endif	//!__AOI_MAP_INSTANCE_H__