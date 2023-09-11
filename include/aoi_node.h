#ifndef __AOI_NODE_H__
#define __AOI_NODE_H__

#include "aoi_define.h"
#include "aoi_unit.h"

#include <unordered_set>

namespace FXAOI
{
	
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

	class AOINode
	{
	public:
		AOINode(NODE_ID lNodeId, unsigned int dwAOIType, unsigned int dwWatchedRadius, unsigned int dwWatchingRadius);
		~AOINode() {}

		void SetCoordinate(const AOICoordinate& refCoordinate){m_oCoordinate = refCoordinate;}
	private:
		NODE_ID m_lNodeId;
		unsigned int m_dwAOIType;
		AOICoordinate m_oCoordinate;
		unsigned int m_dwWatchedRadius;
		unsigned int m_dwWatchingRadius;

		/**
		 * @brief 
		 * 子节点 共享跟此节点相同的 可见 被观察列表 视野与本节点相同
		 */
		std::unordered_set<NODE_ID> m_setChildrenNode;
	};
} // namespace FXAOI

#endif	//!__AOI_NODE_H__