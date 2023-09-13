#ifndef __AOI_NODE_H__
#define __AOI_NODE_H__

#include "aoi_define.h"
#include "aoi_unit.h"

#include <unordered_set>
#include <unordered_map>

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

	struct NodePosition
	{
		double x;
#if AOI_USE_Y_AXIS
		double y;
#endif
		double z;
	};

	double Distance(const NodePosition& l, const NodePosition& r);

	class AOINode
	{
	public:
		AOINode(NODE_ID lNodeId, unsigned int dwAOIType, unsigned int dwWatchedRadius, unsigned int dwWatchingRadius);
		AOINode();
		~AOINode() {}

		void SetCoordinate(const AOICoordinate& refCoordinate){m_oCoordinate = refCoordinate;}
		void SetPosition(const NodePosition& refPosition){m_oPosition = refPosition;}
		unsigned int GetAOIType(){return this->m_dwAOIType;}
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

		std::unordered_set<AOI_UNIT_SUB_SCRIPT> m_setTempWatchingMap;

		/**
		 * @brief 
		 * 子节点 共享跟此节点相同的 可见 被观察列表 视野与本节点相同
		 */
		std::unordered_set<NODE_ID> m_setChildrenNode;
	};
} // namespace FXAOI

#endif	//!__AOI_NODE_H__
