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
		 * �ؿ�����
		 */
		AOICoordinate m_oCoordinate;
		/**
		 * @brief 
		 * �������е�λ��
		 */
		NodePosition m_oPosition;
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
		 * <��Ұ����, ��Ұ�б�>
		 */
		std::unordered_map<unsigned int, std::unordered_set< NODE_ID> > m_mapWatched;
		/**
		 * @brief 
		 * ���ڹ۲�Ľڵ�
		 * <��Ұ����, ��Ұ�б�>
		 */
		std::unordered_map<unsigned int, std::unordered_set< NODE_ID> > m_mapWatching;

		std::unordered_set<AOI_UNIT_SUB_SCRIPT> m_setTempWatchingMap;

		/**
		 * @brief 
		 * �ӽڵ� ������˽ڵ���ͬ�� �ɼ� ���۲��б� ��Ұ�뱾�ڵ���ͬ
		 */
		std::unordered_set<NODE_ID> m_setChildrenNode;
	};
} // namespace FXAOI

#endif	//!__AOI_NODE_H__
