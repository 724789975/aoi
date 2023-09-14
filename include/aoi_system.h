#ifndef __AOI_SYSTEM_H__
#define __AOI_SYSTEM_H__

#include "aoi_define.h"

namespace FXAOI
{
	struct NodePosition;
	class AOISystem
	{
	public:
		static AOISystem& Instance() {return s_oInstace;}

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

		void AddNode(NODE_ID lNodeId, unsigned int dwAOIType, unsigned int dwWatchedRadius, unsigned int dwWatchingRadius);

		void RemoveNode(NODE_ID lNodeId);

		void EnterMap(NODE_ID lNodeId, unsigned int dwMapId, const NodePosition& refPosition);

		void LeaveMap(NODE_ID lNodeId, const NodePosition& refPosition);

		void Move(NODE_ID lNodeId, const NodePosition& refPosition);
	protected:
	private:
		static AOISystem s_oInstace;

	};
} // namespace FXAOI


#endif