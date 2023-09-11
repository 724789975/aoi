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

		void Enter(NODE_ID lNodeId, const AOICoordinate& refCoordinate
			, unsigned int dwWatchedRadius, unsigned int dwWatchingRadius);
		void Leave(NODE_ID lNodeId, const AOICoordinate& refCoordinate
			, unsigned int dwWatchedRadius, unsigned int dwWatchingRadius);

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
		std::unordered_map<unsigned int, MapInstance> m_mapDividedMap;
		/**
		 * @brief 
		 * map<��������,set<�ڵ�id>>
		 * �б��еĽڵ�ɱ������еĽڵ㿴��
		 */
		std::unordered_map<AOI_UNIT_SUB_SCRIPT, std::unordered_set<NODE_ID> > m_mapWatched;
		/**
		 * @brief 
		 * map<��������,set<�ڵ�id>>
		 * �б��еĽڵ�ɿ��������еĽڵ�
		 */
		std::unordered_map<AOI_UNIT_SUB_SCRIPT, std::unordered_set<NODE_ID> > m_mapWatching;
		/**
		 * @brief 
		 * <�ڵ�id, �ؿ�id>
		 */
		std::unordered_map<NODE_ID, AOI_UNIT_SUB_SCRIPT> m_mapNodeChunk;
	};
} // namespace FXAOI

#endif	//!__AOI_MAP_INSTANCE_H__