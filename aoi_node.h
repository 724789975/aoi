#ifndef __AOI_NODE_H__
#define __AOI_NODE_H__

#include "include/aoi_define.h"
#include "aoi_unit.h"

#include <unordered_set>
#include <unordered_map>

namespace FXAOI
{
	class MapInstance;
	
	double Distance(const NodePosition& l, const NodePosition& r);

	class AOINode
	{
	public:
		AOINode(NODE_ID lNodeId, unsigned int dwAOIType, unsigned int dwWatchedRadius, unsigned int dwWatchingRadius);
		AOINode();
		~AOINode() {}

		void SetCoordinate(const AOICoordinate& refCoordinate){m_oCoordinate = refCoordinate;}
		const AOICoordinate& GetCoordinate(){return m_oCoordinate;}
		void SetPosition(const NodePosition& refPosition);
		unsigned int GetAOIType(){return this->m_dwAOIType;}
		void SetMapId(unsigned int dwMapId){this->m_dwMapId = dwMapId;}
		unsigned int GetMapId(){return this->m_dwMapId;}
		unsigned int GetWatchedRadius(){return this->m_dwWatchedRadius;}
		unsigned int GetWatchingRadius(){return this->m_dwWatchingRadius;}
		/**
		 * @brief 
		 * 用于保存计算视野用的正在观察的格子
		 */
		void AddWatching(AOI_UNIT_SUB_SCRIPT lPos);
		/**
		 * @brief 
		 * 计算视野
		 */
		void CalcView(MapInstance* pMapRoot);
	private:
		/**
		 * @brief 
		 * 节点id
		 */
		NODE_ID m_lNodeId;
		/**
		 * @brief 
		 * AOI类型
		 */
		unsigned int m_dwAOIType;
		/**
		 * @brief 
		 * 地图id
		 */
		unsigned int m_dwMapId;
		/**
		 * @brief 
		 * 地块坐标
		 */
		AOICoordinate m_oCoordinate;
		/**
		 * @brief 
		 * 在世界中的位置
		 */
		NodePosition m_oPosition;
		/**
		 * @brief 
		 * 被观察半径
		 */
		unsigned int m_dwWatchedRadius;
		/**
		 * @brief 
		 * 观察半径
		 */
		unsigned int m_dwWatchingRadius;
		/**
		 * @brief 
		 * 被节点看见
		 * <视野类型, 视野列表>
		 */
		std::unordered_map<unsigned int, std::unordered_set< NODE_ID> > m_mapWatched;
		/**
		 * @brief 
		 * 正在观察的节点
		 * <视野类型, 视野列表>
		 */
		std::unordered_map<unsigned int, std::unordered_set< NODE_ID> > m_mapWatching;
		/**
		 * @brief 
		 * 记录玩家移动或进入地图时能够看到的地块
		 */
		std::unordered_set<AOI_UNIT_SUB_SCRIPT> m_setTempWatchingMap;

		/**
		 * @brief 
		 * 子节点 共享跟此节点相同的 可见 被观察列表 视野与本节点相同
		 */
		std::unordered_set<NODE_ID> m_setChildrenNode;
	};
} // namespace FXAOI

#endif	//!__AOI_NODE_H__
