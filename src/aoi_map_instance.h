#ifndef __AOI_MAP_INSTANCE_H__
#define __AOI_MAP_INSTANCE_H__

#include "../include/aoi_define.h"
#include "aoi_node.h"
#include "aoi_unit.h"

#include <map>
#include <set>

namespace FXAOI
{
	class MapInstance
	{
	public:
		MapInstance();
		MapInstance(unsigned int dwMapId, AOI_UNIT_SUB_SCRIPT lSubScript, unsigned int dwDivideNum, MapInstance* pRoot);

		/**
		 * @brief Get the Instance object
		 * �������ֻ�ܴ�root�ؿ鿪ʼ����
		 * @param refCoordinate ����
		 * @return MapInstance* �ؿ�ʵ��
		 */
		MapInstance* GetInstance(const AOICoordinate& refCoordinate);
		/**
		 * @brief Get the Instance object
		 * �������ֻ�ܴ�root�ؿ鿪ʼ����
		 * @param lPos �ؿ��±�
		 * @return MapInstance* �ؿ�ʵ��
		 */
		MapInstance* GetInstance(AOI_UNIT_SUB_SCRIPT lPos);

		/**
		 * @brief 
		 * ����ؿ�
		 * @param lNodeId �ڵ�id
		 * @param refCoordinate ���������
		 * @param dwWatchedRadius ���۲�뾶
		 * @param dwWatchingRadius �۲�뾶
		 */
		void Enter(NODE_ID lNodeId, const AOICoordinate& refCoordinate
			, unsigned int dwWatchedRadius, unsigned int dwWatchingRadius);

		/**
		 * @brief 
		 * �뿪�ؿ�
		 * �����Ҫ�Ӹ��ڵ�ݹ���� ��Ϊ�������ٵ�ʱ�� ��Ҫ�ݹ��merge
		 * @param lNodeId �ڵ�id
		 * @param refCoordinate �뿪������
		 * @param dwWatchedRadius ���۲�뾶
		 * @param dwWatchingRadius �۲�뾶
		 */
		void Leave(NODE_ID lNodeId, const AOICoordinate& refCoordinate
			, unsigned int dwWatchedRadius, unsigned int dwWatchingRadius);

		void Move(NODE_ID lNodeId, const AOICoordinate& refFromCoordinate, const AOICoordinate& refToCoordinate
			, unsigned int dwWatchedRadius, unsigned int dwWatchingRadius);

		void GetNodeInPos(AOI_UNIT_SUB_SCRIPT lPos, std::map<unsigned int, std::set< NODE_ID> >& refWatchingNode);
		/**
		 * @brief Get the Watching In Pos object
		 * ��ȡ���ڹ۲�ĳ�����ӵĽڵ�
		 * @param lPos 
		 * @param refWatchingNode 
		 */
		void GetWatchingInPos(AOI_UNIT_SUB_SCRIPT lPos, std::map<unsigned int, std::set< NODE_ID> >& refWatchingNode);
		/**
		 * @brief 
		 * Ŀ����Ƿ��ڽڵ�ɼ���Χ��
		 * @param lNodeId 
		 * @param lPos 
		 * @return true 
		 * @return false 
		 */
		bool CanWatching(NODE_ID lNodeId, AOI_UNIT_SUB_SCRIPT lPos);

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
		 * �ؿ���ڵ�
		 */
		MapInstance* m_pRoot;
		/**
		 * @brief 
		 * �ؿ�����
		 */
		AOI_UNIT_SUB_SCRIPT m_lSubScript;
		/**
		 * @brief 
		 * ����ؿ�ķ��Ѵ��� ������8��
		 */
		unsigned int m_dwDivideNum;
		/**
		 * @brief 
		 * �Ƿ��ѷ���
		 */
		bool m_bDivided;
		/**
		 * @brief 
		 * �ѷ��ѵؿ�
		 */
		std::map<unsigned int, MapInstance> m_mapDividedMap;
		/**
		 * @brief 
		 * map<��������,set<�ڵ�id>>
		 * �б��еĽڵ�ɱ������еĽڵ㿴��
		 */
		std::map<AOI_UNIT_SUB_SCRIPT, std::set<NODE_ID> > m_mapWatched;
		/**
		 * @brief 
		 * map<��������,set<�ڵ�id>>
		 * �б��еĽڵ�ɿ��������еĽڵ�
		 */
		std::map<AOI_UNIT_SUB_SCRIPT, std::set<NODE_ID> > m_mapWatching;
		/**
		 * @brief 
		 * <�ڵ�id, �ؿ�id>
		 */
		std::map<NODE_ID, AOI_UNIT_SUB_SCRIPT> m_mapNodeChunk;
	};
} // namespace FXAOI

#endif	//!__AOI_MAP_INSTANCE_H__