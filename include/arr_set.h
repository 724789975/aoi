#ifndef __ArrSet_H__
#define __ArrSet_H__

#include "arr_tree_less.h"
#include <exception>
#include <string.h>

namespace FXAOI
{
	//K只能为基础类型 且 不能为string, 可以为char[] 
	//K要支持=操作
	template<typename K, class KeyLess = Less<K> >
	class ArrSet
	{
		template<typename T>
		struct KHasClear {
			template<typename U, void (U::*)()> struct HELPS;
			template<typename U> static char Check(HELPS<U, &U::clear>*);
			template<typename U> static int Check(...);
			const static bool Has = sizeof(Check<T>(0)) == sizeof(char);
		};

		template<typename T>
		struct VHasDestruct {
			template<typename U, void (U::*)()> struct HELPS;
			template<typename U> static char Check(HELPS<U, &U::~U>*);
			template<typename U> static int Check(...);
			const static bool Has = sizeof(Check<T>(0)) == sizeof(char);
		};

		template<typename T>
		void OnClear(T* p, TrueType t)
		{
			for (int i = 0; i < p->m_dwSize; ++i)
			{
				p->m_pKeys[i].clear();
			}
		}
		template<typename T>
		void OnClear(T* p, FalseType f)
		{}

		template<typename T>
		void OnDestruct(T* p, TrueType t)
		{
			p->~T();
		}
		template<typename T>
		void OnDestruct(T* p, FalseType f)
		{}

	public:
		typedef const K* iterator;

		ArrSet() : m_dwSize(0)
		{
			m_pKeys = 0;
			m_dwCapcity = 0;
		}
		ArrSet(const ArrSet<K, KeyLess>& ref)
			: m_dwSize(ref.m_dwSize)
			, m_dwCapcity(ref.m_dwCapcity)
		{
			m_pKeys = new K[m_dwCapcity];
			for (unsigned int i = 0; i < m_dwSize; ++i)
			{
				m_pKeys[i] = ref.m_pKeys[i];
			}
			//memcpy(m_pKeys, ref.m_pKeys, m_dwSize * sizeof(K));
		}
		const ArrSet<K, KeyLess>& operator = (const ArrSet<K, KeyLess>& ref)
		{
			clear();
			m_dwSize = ref.m_dwSize;
			m_dwCapcity = ref.m_dwCapcity;
			if (m_dwCapcity)
			{
				m_pKeys = new K[m_dwCapcity];
				for (unsigned int i = 0; i < m_dwSize; ++i)
				{
					m_pKeys[i] = ref.m_pKeys[i];
				}
				//memcpy(m_pKeys, ref.m_pKeys, m_dwSize * sizeof(K));
			}
			return *this;
		}
		~ArrSet()
		{
			if (m_pKeys)
			{
				delete[] m_pKeys;
				m_pKeys = 0;
			}
		}

		void clear()
		{
			OnClear(this, BooleanType<KHasClear<K>::Has>());
			m_dwSize = 0;
			delete[] m_pKeys;
			m_dwCapcity = 0;
			m_pKeys = 0;
		}

		iterator insert(const K& k)
		{
			if (m_dwSize >= m_dwCapcity)
			{
				if (128 > m_dwCapcity)
				{
					m_dwCapcity = 128;
				}
				else
				{
					m_dwCapcity <<= 1;
				}
				K* pTemp = new K[m_dwCapcity];
				for (unsigned int i = 0; i < m_dwSize; ++i)
				{
					pTemp[i] = m_pKeys[i];
				}
				//memcpy(pTemp, m_pKeys, m_dwSize * sizeof(K));
				if (m_pKeys)
				{
					delete[] m_pKeys;
				}
				m_pKeys = pTemp;
			}

			if (m_dwSize == 0)
			{
				m_pKeys[0] = k;
				++m_dwSize;
				return &m_pKeys[0];
			}

			unsigned int dwLeftIndex = 0;
			unsigned int dwRightIndex = 0;

			unsigned int dwIndexTemp = Search(k, dwLeftIndex, dwRightIndex);
			if (dwIndexTemp != 0XFFFFFFFF)
			{
				return &m_pKeys[dwIndexTemp];
			}
			//放到最前面
			if (m_oLess(k, m_pKeys[dwLeftIndex]))
			{
				memmove(m_pKeys + dwLeftIndex + 1, m_pKeys + dwLeftIndex, (m_dwSize - dwLeftIndex) * sizeof(K));
				m_pKeys[dwLeftIndex] = k;
				++m_dwSize;
				return m_pKeys + dwLeftIndex;
			}
			//放到最后面
			if (m_oLess(m_pKeys[dwRightIndex], k))
			{
				m_pKeys[m_dwSize] = k;
				++m_dwSize;
				return m_pKeys + m_dwSize - 1;
			}

			memmove(m_pKeys + dwRightIndex + 1, m_pKeys + dwRightIndex, (m_dwSize - dwRightIndex) * sizeof(K));

			m_pKeys[dwRightIndex] = k;
			++m_dwSize;

			new (m_pKeys + dwRightIndex)K;
			return m_pKeys + dwRightIndex;
		}

		bool empty()const
		{
			return this->m_dwSize == 0;
		}

		iterator erase(iterator k)
		{
			return erase(*k);
		}

		iterator erase(const K& k)
		{
			unsigned int dwIndex = Search(k);
			if (dwIndex == 0XFFFFFFFF)
			{
				return end();
			}

			if (dwIndex == m_dwSize - 1)
			{
				--m_dwSize;
				OnDestruct(&m_pKeys[dwIndex], BooleanType<VHasDestruct<K>::Has>());
				memset(m_pKeys + dwIndex, 0, sizeof(K));
				return m_pKeys + dwIndex;
			}
			OnDestruct(&m_pKeys[dwIndex], BooleanType<VHasDestruct<K>::Has>());
			memmove(m_pKeys + dwIndex, m_pKeys + dwIndex + 1, (m_dwSize - 1 - dwIndex) * sizeof(K));
			memset(m_pKeys + m_dwSize - 1, 0, sizeof(K));
			--m_dwSize;
			return m_pKeys + dwIndex;
		}

		iterator find(K k)
		{
			unsigned int dw_index = Search(k);
			if (dw_index == 0XFFFFFFFF)
			{
				return end();
			}

			return m_pKeys + dw_index;
		}

		iterator begin() const
		{
			return m_pKeys;
		}

		iterator end() const
		{
			return m_pKeys + m_dwSize;
		}

		unsigned int size() const { return m_dwSize; }


		void swap(ArrSet<K, KeyLess>& ref)
		{

			unsigned int dwSize = ref.m_dwSize;
			ref.m_dwSize = m_dwSize;
			m_dwSize = dwSize;
			unsigned int dwCapcity = ref.m_dwCapcity;
			ref.m_dwCapcity = m_dwCapcity;
			m_dwCapcity = dwCapcity;
			K* pKeys = ref.m_pKeys;
			ref.m_pKeys = m_pKeys;
			m_pKeys = pKeys;
		}

	protected:
	private:

		unsigned int Search(const K& k) const
		{
			if (m_dwSize == 0)
			{
				return 0XFFFFFFFF;
			}

			unsigned int dwLeftIndex = 0;
			unsigned int dwRightIndex = 0;

			return Search(k, dwLeftIndex, dwRightIndex);
		}

		unsigned int Search(const K& k, unsigned int& dwLeftIndex, unsigned int& dwRightIndex) const
		{
			if (m_dwSize == 0)
			{
				return 0XFFFFFFFF;
			}
			//二分法查找
			dwLeftIndex = 0;
			dwRightIndex = m_dwSize - 1;

			unsigned int dwMidIndex = (dwLeftIndex + dwRightIndex) >> 1;

			while (dwLeftIndex < dwRightIndex && !m_oEqual(m_pKeys[dwMidIndex], k))
			{
				if (m_oLess(m_pKeys[dwMidIndex], k))
				{
					dwLeftIndex = dwMidIndex + 1;
				}
				else if (m_oLess(k, m_pKeys[dwMidIndex]))
				{
					dwRightIndex = dwMidIndex;
				}
				dwMidIndex = (dwLeftIndex + dwRightIndex) >> 1;
			}
			if (m_oEqual(m_pKeys[dwMidIndex], k))
			{
				return dwMidIndex;
			}
			return 0XFFFFFFFF;
		}

	private:
		unsigned int m_dwSize;
		unsigned int m_dwCapcity;
		K* m_pKeys;

		KeyLess m_oLess;
		Equal<K, KeyLess> m_oEqual;
	};
};
#endif // !__ArrSet_H__
