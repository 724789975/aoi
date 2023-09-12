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
		 * ���ɼ�
		 */
		AOIVisibilityType_Invisible,
		/**
		 * @brief 
		 * �ɼ���
		 */
		AOIVisibilityType_Visible,
		/**
		 * @brief 
		 * ���뻥��ɼ�
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
		 * ���ڱ��������Ұ�õ����ڹ۲�ĸ���
		 */
		void AddWatching(AOI_UNIT_SUB_SCRIPT lPos);
		/**
		 * @brief 
		 * ������Ұ
		 */
		void CalcView(MapInstance* pMapRoot);
	private:
		/**
		 * @brief 
		 * �ڵ�id
		 */
		NODE_ID m_lNodeId;
		/**
		 * @brief 
		 * AOI����
		 */
		unsigned int m_dwAOIType;
		/**
		 * @brief 
		 * ����
		 */
		AOICoordinate m_oCoordinate;
		/**
		 * @brief 
		 * ���۲�뾶
		 */
		unsigned int m_dwWatchedRadius;
		/**
		 * @brief 
		 * �۲�뾶
		 */
		unsigned int m_dwWatchingRadius;
		/**
		 * @brief 
		 * ���ڵ㿴��
		 */
		std::unordered_set<NODE_ID> m_setWatched;
		/**
		 * @brief 
		 * ���ڹ۲�Ľڵ�
		 */
		std::unordered_set<NODE_ID> m_setWatching;

		std::unordered_set<AOI_UNIT_SUB_SCRIPT> m_setTempWatchingMap;

		/**
		 * @brief 
		 * �ӽڵ� ������˽ڵ���ͬ�� �ɼ� ���۲��б� ��Ұ�뱾�ڵ���ͬ
		 */
		std::unordered_set<NODE_ID> m_setChildrenNode;
	};
} // namespace FXAOI

#endif	//!__AOI_NODE_H__