#ifndef __AOI_SYSTEM_H__
#define __AOI_SYSTEM_H__

#include "aoi_define.h"

#include <map>
#include <set>
#include <ostream>

namespace FXAOI
{
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

	/**
	 * @brief 
	 * 
	 * @param lNodeId ���������Ľڵ�
	 * @param mapAddWatching �˽ڵ��յ��ĳ��ְ�
	 * @param mapDelWatching �˽ڵ��յ�����ʧ��
	 * @param mapAddWatched �����ڵ��յ����ְ�
	 * @param mapDelWatched �����ڵ��յ���ʧ��
	 */
	typedef void AoiOperator (NODE_ID lNodeId
		, std::map<unsigned int, std::set<NODE_ID> >& mapAddWatching
		, std::map<unsigned int, std::set<NODE_ID> >& mapDelWatching
		, std::map<unsigned int, std::set<NODE_ID> >& mapAddWatched
		, std::map<unsigned int, std::set<NODE_ID> >& mapDelWatched
	);

	void AoiOperatorDefault (NODE_ID lNodeId
		, std::map<unsigned int, std::set<NODE_ID> >& mapAddWatching
		, std::map<unsigned int, std::set<NODE_ID> >& mapDelWatching
		, std::map<unsigned int, std::set<NODE_ID> >& mapAddWatched
		, std::map<unsigned int, std::set<NODE_ID> >& mapDelWatched
	);

	class AOISystem
	{
	public:
		AOISystem(AoiOperator* pOperator = AoiOperatorDefault):m_pAoiOperator(pOperator){}
		static AOISystem& Instance() {return s_oInstance;}
		/**
		 * @brief 
		 * ��ӵ�ͼ����
		 * @param dwMapId ��ͼid
		 * @param dwViweRadius ��Ұ�뾶
		 * @param dwLength ��ͼ����
		 * @param dwWidth ��ͼ���
		 * @param dwHight ��ͼ�߶�
		 * @param dOffsetX ����ԭ��ƫ��(������ƫ�Ƶ�0,0,0)
		 * @param dOffsetY ����ԭ��ƫ��
		 * @param dOffsetZ ����ԭ��ƫ��
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
		 * ��ӽڵ�
		 * @param lNodeId 
		 * @param dwAOIType aoi����
		 * @param dwWatchedRadius ���۲�뾶
		 * @param dwWatchingRadius �۲�뾶
		 */
		void AddNode(NODE_ID lNodeId, unsigned int dwAOIType, unsigned int dwWatchedRadius, unsigned int dwWatchingRadius);
		/**
		 * @brief 
		 * �Ƴ��ڵ�
		 * @param lNodeId 
		 */
		void RemoveNode(NODE_ID lNodeId);
		/**
		 * @brief 
		 * �����ͼ
		 * @param lNodeId �ڵ�id
		 * @param dwMapId ��ͼid
		 * @param refPosition ����λ��(��������)
		 */
		void EnterMap(NODE_ID lNodeId, unsigned int dwMapId, const NodePosition& refPosition);
		/**
		 * @brief 
		 * �뿪��ͼ
		 * @param lNodeId �ڵ�id
		 */
		void LeaveMap(NODE_ID lNodeId);
		/**
		 * @brief 
		 * �뿪��ͼ
		 * @param lNodeId �ڵ�id
		 * @param refPosition ��ǰ����λ��(��������)
		 */
		void Move(NODE_ID lNodeId, const NodePosition& refPosition);
		/**
		 * @brief 
		 * ����ӽڵ�
		 * @param lNodeId 
		 * @param lChildId 
		 */
		void AddChild(NODE_ID lNodeId, NODE_ID lChildId);
		/**
		 * @brief 
		 * ɾ���ӽڵ�
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
		 * ��ӡnode��Ϣ
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