#ifndef __AOI_NODE_H__
#define __AOI_NODE_H__

#include "aoi_define.h"
#include "aoi_unit.h"

#include <unordered_set>

namespace FXAOI
{
	class MapInstance;
	
	enum AOIVisibilityType
	{
		AOIVisibilityType_None,
		/**
		 * @brief 
		 * 不可见
		 */
		AOIVisibilityType_Invisible,
		/**
		 * @brief 
		 * 可见的
		 */
		AOIVisibilityType_Visible,
		/**
		 * @brief 
		 * 必须互相可见
		 */
		AOIVisibilityType_Mutual_Visibility,
	};

	void SetAOIVisibilityType(unsigned int dwAOIType1, unsigned int dwAOIType2, AOIVisibilityType type);
	AOIVisibilityType GetAOIVisibilityType(unsigned int dwAOIType1, unsigned int dwAOIType2);
	void SetAOINodeLimit(unsigned int dwAOIType1, unsigned int dwAOIType2, unsigned int dwNum);
	unsigned int GetAOINodeLimit(unsigned int dwAOIType1, unsigned int dwAOIType2);

	class AOINode
	{
	public:
		AOINode(NODE_ID lNodeId, unsigned int dwAOIType, unsigned int dwWatchedRadius, unsigned int dwWatchingRadius);
		AOINode();
		~AOINode() {}

		void SetCoordinate(const AOICoordinate& refCoordinate){m_oCoordinate = refCoordinate;}
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
		 * 坐标
		 */
		AOICoordinate m_oCoordinate;
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
		 */
		std::unordered_set<NODE_ID> m_setWatched;
		/**
		 * @brief 
		 * 正在观察的节点
		 */
		std::unordered_set<NODE_ID> m_setWatching;

		std::unordered_set<AOI_UNIT_SUB_SCRIPT> m_setTempWatchingMap;

		/**
		 * @brief 
		 * 子节点 共享跟此节点相同的 可见 被观察列表 视野与本节点相同
		 */
		std::unordered_set<NODE_ID> m_setChildrenNode;
	};
} // namespace FXAOI

#endif	//!__AOI_NODE_H__