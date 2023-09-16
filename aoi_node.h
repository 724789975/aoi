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

		/**
		 * @brief Set the Coordinate object
		 * 
		 * @param refCoordinate 
		 */
		void SetCoordinate(const AOICoordinate& refCoordinate){m_oCoordinate = refCoordinate;}
		/**
		 * @brief Get the Coordinate object
		 * 
		 * @return const AOICoordinate& 
		 */
		const AOICoordinate& GetCoordinate(){return m_oCoordinate;}
		/**
		 * @brief Set the Position object
		 * 
		 * @param refPosition 
		 */
		void SetPosition(const NodePosition& refPosition);
		/**
		 * @brief 
		 * ����aoi����
		 * @return unsigned int 
		 */
		unsigned int GetAOIType(){return this->m_dwAOIType;}
		/**
		 * @brief Set the Map Id object
		 * 
		 * @param dwMapId 
		 */
		void SetMapId(unsigned int dwMapId){this->m_dwMapId = dwMapId;}
		/**
		 * @brief Get the Map Id object
		 * 
		 * @return unsigned int 
		 */
		unsigned int GetMapId(){return this->m_dwMapId;}
		/**
		 * @brief Get the Watched Radius object
		 * 
		 * @return unsigned int 
		 */
		unsigned int GetWatchedRadius(){return this->m_dwWatchedRadius;}
		/**
		 * @brief Get the Watching Radius object
		 * 
		 * @return unsigned int 
		 */
		unsigned int GetWatchingRadius(){return this->m_dwWatchingRadius;}
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

		void Debug(std::ostream& refOstream);
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
		 * ��ͼid
		 */
		unsigned int m_dwMapId;
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
		/**
		 * @brief 
		 * ��¼����ƶ�������ͼʱ�ܹ������ĵؿ�
		 */
		std::unordered_set<AOI_UNIT_SUB_SCRIPT> m_setTempWatchingMap;

		/**
		 * @brief 
		 * �ӽڵ� ������˽ڵ���ͬ�� �ɼ� ���۲��б� ��Ұ�뱾�ڵ���ͬ
		 */
		std::unordered_set<NODE_ID> m_setChildrenNode;
	};
} // namespace FXAOI

#endif	//!__AOI_NODE_H__
