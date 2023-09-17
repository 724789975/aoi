#ifndef __AOI_SYSTEM_H__
#define __AOI_SYSTEM_H__

#include "aoi_define.h"

#include <unordered_map>
#include <unordered_set>
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
		, std::unordered_map<unsigned int, std::unordered_set<NODE_ID> >& mapAddWatching
		, std::unordered_map<unsigned int, std::unordered_set<NODE_ID> >& mapDelWatching
		, std::unordered_map<unsigned int, std::unordered_set<NODE_ID> >& mapAddWatched
		, std::unordered_map<unsigned int, std::unordered_set<NODE_ID> >& mapDelWatched
	);

	void AoiOperatorDefault (NODE_ID lNodeId
		, std::unordered_map<unsigned int, std::unordered_set<NODE_ID> >& mapAddWatching
		, std::unordered_map<unsigned int, std::unordered_set<NODE_ID> >& mapDelWatching
		, std::unordered_map<unsigned int, std::unordered_set<NODE_ID> >& mapAddWatched
		, std::unordered_map<unsigned int, std::unordered_set<NODE_ID> >& mapDelWatched
	);

	class AOISystem
	{
	public:
		AOISystem(AoiOperator* pOperator = AoiOperatorDefault):m_pAoiOperator(pOperator){}
		static AOISystem& Instance() {return s_oInstance;}

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
		 * 
		 * @param lNodeId 
		 * @param dwAOIType 
		 * @param dwWatchedRadius 
		 * @param dwWatchingRadius 
		 */
		void AddNode(NODE_ID lNodeId, unsigned int dwAOIType, unsigned int dwWatchedRadius, unsigned int dwWatchingRadius);
		/**
		 * @brief 
		 * 
		 * @param lNodeId 
		 */
		void RemoveNode(NODE_ID lNodeId);
		/**
		 * @brief 
		 * 
		 * @param lNodeId 
		 * @param dwMapId 
		 * @param refPosition 
		 */
		void EnterMap(NODE_ID lNodeId, unsigned int dwMapId, const NodePosition& refPosition);
		/**
		 * @brief 
		 * 
		 * @param lNodeId 
		 * @param refPosition 
		 */
		void LeaveMap(NODE_ID lNodeId);
		/**
		 * @brief 
		 * 
		 * @param lNodeId 
		 * @param refPosition 
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
		std::unordered_map<NODE_ID, NODE_ID> m_mapNodeParent;
	};
} // namespace FXAOI


#endif