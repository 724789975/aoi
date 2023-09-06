#ifndef __AOI_MAP_INSTANCE_H__
#define __AOI_MAP_INSTANCE_H__

#include "aoi_define.h"
#include "aoi_node.h"
#include "aoi_unit.h"

#include <unordered_map>
#include <unordered_set>

namespace FXAOI
{
	class MapInstance
	{
	public:
		MapInstance(){}
		MapInstance(unsigned int dwMapId, AOI_UNIT_SUB_SCRIPT lSubScript, unsigned int dwDivideNum, MapInstance* pRoot);

		MapInstance* GetInstance(const AOICoordinate& refCoordinate);
		MapInstance* GetInstance(AOI_UNIT_SUB_SCRIPT lPos);

		void Divide();
		void Merge();

		void Enter(NODE_ID lNodeId, const AOICoordinate& refCoordinate
			, unsigned int dwWatchedRadius, unsigned int dwWatchingRadius);
		void Leave(NODE_ID lNodeId, const AOICoordinate& refCoordinate
			, unsigned int dwWatchedRadius, unsigned int dwWatchingRadius);

	protected:
	private:
		unsigned int m_dwMapId;
		/**
		 * @brief 
		 * 地块根节点
		 */
		MapInstance* m_pRoot;
		/**
		 * @brief 
		 * 地块坐标
		 */
		AOI_UNIT_SUB_SCRIPT m_lSubScript;
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
		 * map<格子坐标,set<节点id>>
		 * 列表中的节点可被格子中的节点看见
		 */
		std::unordered_map<AOI_UNIT_SUB_SCRIPT, std::unordered_set<NODE_ID> > m_mapWatched;
		/**
		 * @brief 
		 * map<格子坐标,set<节点id>>
		 * 列表中的节点可看见格子中的节点
		 */
		std::unordered_map<AOI_UNIT_SUB_SCRIPT, std::unordered_set<NODE_ID> > m_mapWatching;
		/**
		 * @brief 
		 * <节点id, 地块id>
		 */
		std::unordered_map<NODE_ID, AOI_UNIT_SUB_SCRIPT> m_mapNodeChunk;
	};
} // namespace FXAOI

#endif	//!__AOI_MAP_INSTANCE_H__