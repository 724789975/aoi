#ifndef __AOI_SYSTEM_H__
#define __AOI_SYSTEM_H__

#include "aoi_define.h"

//#include "arr_map.h"
//#include "arr_set.h"
#include <map>
#include <set>
#include <ostream>
#include <vector>

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
	

	struct AoiOperatorInfo
	{
		unsigned int m_dwType;
		std::vector<NODE_ID> m_vecNodes;
	};
	/**
	 * @brief 
	 * 
	 * @param lNodeId 发生操作的节点
	 * @param mapAddWatching 此节点收到的出现包
	 * @param mapDelWatching 此节点收到的消失包
	 * @param mapAddWatched 其他节点收到出现包
	 * @param mapDelWatched 其他节点收到消失包
	 */
	typedef void AoiOperator(NODE_ID lNodeId
		, std::vector<AoiOperatorInfo> vecAddWatching
		, std::vector<AoiOperatorInfo> vecDelWatching
		, std::vector<AoiOperatorInfo> vecAddWatched
		, std::vector<AoiOperatorInfo> vecDelWatched
	);

	void AoiOperatorDefault (NODE_ID lNodeId
		, std::vector<AoiOperatorInfo> vecAddWatching
		, std::vector<AoiOperatorInfo> vecDelWatching
		, std::vector<AoiOperatorInfo> vecAddWatched
		, std::vector<AoiOperatorInfo> vecDelWatched
	);

	class AOISystem
	{
	public:
		AOISystem(AoiOperator* pOperator = AoiOperatorDefault):m_pAoiOperator(pOperator){}
		static AOISystem& Instance() {return s_oInstance;}
		/**
		 * @brief 
		 * 添加地图配置
		 * @param dwMapId 地图id
		 * @param dwViweRadius 视野半径
		 * @param dwLength 地图长度
		 * @param dwWidth 地图宽度
		 * @param dwHight 地图高度
		 * @param dOffsetX 距离原点偏移(将中心偏移到0,0,0)
		 * @param dOffsetY 距离原点偏移
		 * @param dOffsetZ 距离原点偏移
		 */
		bool AddMap(unsigned int dwMapId, unsigned int dwViweRadius, unsigned int dwLength, unsigned int dwWidth
#if AOI_USE_Y_AXIS
			, unsigned int dwHight
#endif
 			, double dOffsetX
#if AOI_USE_Y_AXIS
			, double dOffsetY
#endif
			, double dOffsetZ
		);

		/**
		 * @brief
		 * 删除地图配置
		 * @param dwMapId 地图id
		 */
		void RemoveMap(unsigned int dwMapId);

		/**
		 * @brief 
		 * 设置dwAOIType1对dwAOIType2的可见性
		 * @param dwAOIType1 
		 * @param dwAOIType2 
		 * @param type 
		 */
		void SetAOIVisibilityType(unsigned int dwAOIType1, unsigned int dwAOIType2, AOIVisibilityType type);
		/**
		 * @brief 
		 * 获取可见性
		 * @param dwAOIType1 
		 * @param dwAOIType2 
		 * @return AOIVisibilityType 
		 */
		const AOIVisibilityType& GetAOIVisibilityType(unsigned int dwAOIType1, unsigned int dwAOIType2);
		/**
		 * @brief 
		 * 设置dwAOIType1可见的dwAOIType2的数量
		 * @param dwAOIType1 
		 * @param dwAOIType2 
		 * @param dwNum 
		 */
		void SetAOINodeLimit(unsigned int dwAOIType1, unsigned int dwAOIType2, unsigned int dwNum);
		/**
		 * @brief 
		 * 获取某类型视野上限 默认无上限
		 * @param dwAOIType1 
		 * @param dwAOIType2 
		 * @return unsigned int 
		 */
		const unsigned int& GetAOINodeLimit(unsigned int dwAOIType1, unsigned int dwAOIType2);
		/**
		 * @brief 
		 * 添加节点
		 * @param lNodeId 
		 * @param dwAOIType aoi类型
		 * @param dwWatchedRadius 被观察半径
		 * @param dwWatchingRadius 观察半径
		 */
		void AddNode(NODE_ID lNodeId, unsigned int dwAOIType, unsigned int dwWatchedRadius, unsigned int dwWatchingRadius);
		/**
		 * @brief 
		 * 移除节点
		 * @param lNodeId 
		 */
		void RemoveNode(NODE_ID lNodeId);
		/**
		 * @brief 
		 * 进入地图
		 * @param lNodeId 节点id
		 * @param dwMapId 地图id
		 * @param refPosition 坐标位置(世界坐标)
		 */
		void EnterMap(NODE_ID lNodeId, unsigned int dwMapId, const NodePosition& refPosition);
		/**
		 * @brief 
		 * 离开地图
		 * @param lNodeId 节点id
		 */
		void LeaveMap(NODE_ID lNodeId);
		/**
		 * @brief 
		 * 离开地图
		 * @param lNodeId 节点id
		 * @param refPosition 当前坐标位置(世界坐标)
		 */
		void Move(NODE_ID lNodeId, const NodePosition& refPosition);
		/**
		 * @brief 
		 * 添加子节点
		 * @param lNodeId 
		 * @param lChildId 
		 */
		void AddChild(NODE_ID lNodeId, NODE_ID lChildId);
		/**
		 * @brief 
		 * 删除子节点
		 * @param lNodeId 
		 * @param lChildId 
		 */
		void RemoveChild(NODE_ID lNodeId, NODE_ID lChildId);
		/**
		 * @brief Set the Aoi Operator object
		 * 
		 * @param pAoiOperator 
		 */
		void SetAoiOperator(AoiOperator* pAoiOperator){m_pAoiOperator = pAoiOperator;}
		/**
		 * @brief Get the Aoi Operator object
		 * 
		 * @return AoiOperator* 
		 */
		AoiOperator* GetAoiOperator(){return m_pAoiOperator;}
		/**
		 * @brief 
		 * 打印node信息
		 * @param lNodeId 
		 * @param refOstream 
		 */
		void DebugNode(NODE_ID lNodeId, std::ostream& refOstream);
	protected:
	private:
		static AOISystem s_oInstance;
		AoiOperator* m_pAoiOperator;
		std::map<NODE_ID, NODE_ID> m_mapNodeParent;
	};
} // namespace FXAOI


#endif