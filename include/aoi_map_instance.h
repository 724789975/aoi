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

		/**
		 * @brief Get the Instance object
		 * 这个函数只能从root地块开始调用
		 * @param refCoordinate 坐标
		 * @return MapInstance* 地块实例
		 */
		MapInstance* GetInstance(const AOICoordinate& refCoordinate);
		/**
		 * @brief Get the Instance object
		 * 这个函数只能从root地块开始调用
		 * @param lPos 地块下标
		 * @return MapInstance* 地块实例
		 */
		MapInstance* GetInstance(AOI_UNIT_SUB_SCRIPT lPos);

		/**
		 * @brief 
		 * 进入地块
		 * @param lNodeId 节点id
		 * @param refCoordinate 进入点坐标
		 * @param dwWatchedRadius 被观察半径
		 * @param dwWatchingRadius 观察半径
		 */
		void Enter(NODE_ID lNodeId, const AOICoordinate& refCoordinate
			, unsigned int dwWatchedRadius, unsigned int dwWatchingRadius);

		/**
		 * @brief 
		 * 离开地块
		 * @param lNodeId 节点id
		 * @param refCoordinate 离开的坐标
		 * @param dwWatchedRadius 被观察半径
		 * @param dwWatchingRadius 观察半径
		 */
		void Leave(NODE_ID lNodeId, const AOICoordinate& refCoordinate
			, unsigned int dwWatchedRadius, unsigned int dwWatchingRadius);

		void Move(NODE_ID lNodeId, const AOICoordinate& refFromCoordinate, const AOICoordinate& refToCoordinate
			, unsigned int dwWatchedRadius, unsigned int dwWatchingRadius);

		void GetNodeInPos(AOI_UNIT_SUB_SCRIPT lPos, std::unordered_set<NODE_ID>& setRet);

	protected:
	private:
		void Divide();
		void Merge();
		void AddWatched(NODE_ID lNodeId, AOI_UNIT_SUB_SCRIPT lPos);
		void AddWatching(NODE_ID lNodeId, AOI_UNIT_SUB_SCRIPT lPos);
		void RemoveWatched(NODE_ID lNodeId, AOI_UNIT_SUB_SCRIPT lPos);
		void RemoveWatching(NODE_ID lNodeId, AOI_UNIT_SUB_SCRIPT lPos);

		void AfterAddWatched(NODE_ID lNodeId, AOI_UNIT_SUB_SCRIPT lPos);
		void AfterAddWatching(NODE_ID lNodeId, AOI_UNIT_SUB_SCRIPT lPos);
		void AfterRemoveWatched(NODE_ID lNodeId, AOI_UNIT_SUB_SCRIPT lPos);
		void AfterRemoveWatching(NODE_ID lNodeId, AOI_UNIT_SUB_SCRIPT lPos);

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