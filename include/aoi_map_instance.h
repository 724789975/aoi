#ifndef __AOI_MAP_INSTANCE_H__
#define __AOI_MAP_INSTANCE_H__

#include "aoi_define.h"
#include "aoi_node.h"

#include <unordered_map>
#include <unordered_set>

namespace AOI
{
	class MapInstance
	{
	public:
		MapInstance(unsigned int dwMapId, unsigned int dwDivideNum)
			: m_dwMapId(dwMapId)
			, m_dwDivideNum(dwDivideNum)
			, m_bDivided(false)
		{}

		void Divide(){}
		void Merge(){}

		void Enter(AOINode& refNode){}
		void Leave(AOINode& refNode){}

	protected:
	private:
		unsigned int m_dwMapId;
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
		 * �б��еĽڵ�ɱ������еĽڵ㿴��
		 */
		std::unordered_set<NODE_ID> m_setWatched;
		/**
		 * @brief 
		 * �б��еĽڵ�ɿ��������еĽڵ�
		 */
		std::unordered_set<NODE_ID> m_setWatching;
		/**
		 * @brief 
		 * <�ڵ�id, �ؿ�id>
		 */
		std::unordered_map<NODE_ID, AOI_UNIT_SUB_SCRIPT> m_mapNodeChunk;
	};
} // namespace AOI

#endif	//!__AOI_MAP_INSTANCE_H__