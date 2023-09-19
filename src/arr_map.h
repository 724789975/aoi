#ifndef __ArrMap_H__
#define __ArrMap_H__

#include "arr_tree_less.h"
#include <exception>

namespace FXAOI
{
	//K只能为基础类型 且 不能为string, 可以为char[] V必须指定默认构造函数
	//K要支持[]=操作
	template<typename K, typename V, class KeyLess = Less<K> >
	class ArrMap
	{
		class KVPair
		{
		public:
			K first;
			V second;
		};

		template<typename T>
		struct VHasClear {
			template<typename U, void (U::*)()> struct HELPS;
			template<typename U> static char Check(HELPS<U, &U::clear>*);
			template<typename U> static int Check(...);
			const static bool Has = sizeof(Check<T>(0)) == sizeof(char);
		};

		struct TrueType {
			enum { Result = true };
		};

		struct FalseType {
			enum { Result = false };
		};

		template <bool val>
		struct BooleanType : public FalseType
		{};

		template <>
		struct BooleanType<true> : public TrueType
		{};

		template<typename T>
		void OnClear(T* p, TrueType t)
		{
			for (unsigned int i = 0; i <p->m_dwSize; ++i)
			{
				p->m_pKeyStores[i].second.clear();
			}
		}
		template<typename T>
		void OnClear(T* p, FalseType f)
		{}

	public:
		typedef class
		{
		public:
			const K first;
			V second;
		}*iterator;

		//typedef KeyStore* Iterator;

		ArrMap() : m_dwSize(0)
		{
			m_pKeyStores = 0;
			m_dwCapcity = 0;
		}
		ArrMap(const ArrMap<K, V, KeyLess>& ref)
			: m_dwSize(ref.m_dwSize)
			, m_dwCapcity(ref.m_dwCapcity)
		{
			m_pKeyStores = new KVPair[m_dwCapcity];
			for (unsigned int i = 0; i < m_dwSize; ++i)
			{
				m_pKeyStores[i] = ref.m_pKeyStores[i];
			}
			//memcpy(m_pKeyStores, ref.m_pKeyStores, m_dwSize * sizeof(KVPair));
		}
		const ArrMap<K, V, KeyLess>& operator = (const ArrMap<K, V, KeyLess>& ref)
		{
			m_dwSize = ref.m_dwSize;
			m_dwCapcity = ref.m_dwCapcity;
			if (m_dwCapcity)
			{
				m_pKeyStores = new KVPair[m_dwCapcity];
				for (unsigned int i = 0; i < m_dwSize; ++i)
				{
					m_pKeyStores[i] = ref.m_pKeyStores[i];
				}
				//memcpy(m_pKeyStores, ref.m_pKeyStores, m_dwSize * sizeof(KVPair));
			}
			return *this;
		}
		~ArrMap()
		{
			if (m_pKeyStores)
			{
				delete[] m_pKeyStores;
				m_pKeyStores = 0;
			}
		}

		void clear()
		{
			OnClear(this, BooleanType<VHasClear<V>::Has>());
			m_dwSize = 0;
			delete[] m_pKeyStores;
			m_dwCapcity = 0;
			m_pKeyStores = 0;
		}

		V* Alloc(const K& k)
		{
			KVPair* pPair = _Alloc(k);
			if (!pPair)
			{
				return nullptr;
			}
			return &(pPair->second);
		}

		iterator insert(const K& k, const V& v)
		{
			KVPair* pPair = _Alloc(k);
			if (!pPair)
			{
				return end();
			}
			//memmove(&pPair->second, &v, sizeof(V));
			pPair->second = v;
			return (iterator)pPair;
		}

		iterator erase(iterator k)
		{
			return erase(*k);
		}

		iterator erase(const K& k)
		{
			unsigned int dwLeftIndex = 0;
			unsigned int dwRightIndex = 0;

			unsigned int dwIndex = Search(k);
			if (dwIndex == 0XFFFFFFFF)
			{
				return end();
			}

			if (dwIndex == m_dwSize - 1)
			{
				--m_dwSize;
				memset(&m_pKeyStores[dwIndex], 0, sizeof(KVPair));
				return (iterator)(m_pKeyStores + dwIndex);
			}
			memmove(&m_pKeyStores[dwIndex], &m_pKeyStores[dwIndex + 1], (m_dwSize - 1 - dwIndex) * sizeof(KVPair));
			memset(&m_pKeyStores[m_dwSize - 1], 0, sizeof(KVPair));
			--m_dwSize;
			return (iterator)(m_pKeyStores + dwIndex);
		}

		iterator find(const K& k)
		{
			unsigned int dwIndex = Search(k);
			if (dwIndex == 0XFFFFFFFF)
			{
				return end();
			}

			return (iterator)(&m_pKeyStores[dwIndex]);
		}

		iterator begin() const
		{
			return (iterator)m_pKeyStores;
		}

		iterator end()
		{
			return (iterator)m_pKeyStores + m_dwSize;
		}

		unsigned int size() const { return m_dwSize; }

		void swap(ArrMap<K, V, KeyLess>& ref)
		{
			unsigned int dwSize = ref.m_dwSize;
			ref.m_dwSize = m_dwSize;
			m_dwSize = dwSize;
			unsigned int dwCapcity = ref.m_dwCapcity;
			ref.m_dwCapcity = m_dwCapcity;
			m_dwCapcity = dwCapcity;
			KVPair* pKeyStores = ref.m_pKeyStores;
			ref.m_pKeyStores = m_pKeyStores;
			m_pKeyStores = pKeyStores;
		}

		V& operator [](const K& refK)
		{
			unsigned int dwIndex = Search(refK);
			if (dwIndex == 0XFFFFFFFF)
			{
				KVPair* pPair = _Alloc(refK);
				new(&(pPair->second)) V();
				return pPair->second;
			}

			return m_pKeyStores[dwIndex].second;
		}
	protected:
	private:
		KVPair* _Alloc(const K& k)
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
				KVPair* pTemp = new KVPair[m_dwCapcity];
				for (unsigned int i = 0; i < m_dwSize; ++i)
				{
					pTemp[i] = m_pKeyStores[i];
				}
				//memcpy(pTemp, m_pKeyStores, m_dwSize * sizeof(KVPair));
				if (m_pKeyStores)
				{
					delete[] m_pKeyStores;
				}
				m_pKeyStores = pTemp;
			}

			if (m_dwSize == 0)
			{
				m_pKeyStores[0].first = k;
				++m_dwSize;
				return &m_pKeyStores[0];
			}

			unsigned int dwLeftIndex = 0;
			unsigned int dwRightIndex = 0;

			if (Search(k, dwLeftIndex, dwRightIndex) != 0XFFFFFFFF)
			{
				return nullptr;
			}
			//放到最前面
			if (m_oLess(k, m_pKeyStores[dwLeftIndex].first))
			{
				memmove(&m_pKeyStores[dwLeftIndex + 1], &m_pKeyStores[dwLeftIndex], (m_dwSize - dwLeftIndex) * sizeof(KVPair));
				m_pKeyStores[dwLeftIndex].first = k;
				++m_dwSize;
				return &m_pKeyStores[dwLeftIndex];
			}
			//放到最后面
			if (m_oLess(m_pKeyStores[dwRightIndex].first, k))
			{
				m_pKeyStores[m_dwSize].first = k;
				++m_dwSize;
				return &m_pKeyStores[m_dwSize - 1];
			}

			memmove(&m_pKeyStores[dwRightIndex + 1], &m_pKeyStores[dwRightIndex], (m_dwSize - dwRightIndex) * sizeof(KVPair));

			m_pKeyStores[dwRightIndex].first = k;
			++m_dwSize;

			new (&m_pKeyStores[dwRightIndex]) V;
			return &m_pKeyStores[dwRightIndex];
		}

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

			unsigned int dwMidIndex = (dwLeftIndex + dwRightIndex) / 2;

			while (dwLeftIndex < dwRightIndex && !m_oEqual(m_pKeyStores[dwMidIndex].first, k))
			{
				if (m_oLess(m_pKeyStores[dwMidIndex].first, k))
				{
					dwLeftIndex = dwMidIndex + 1;
				}
				else if (m_oLess(k, m_pKeyStores[dwMidIndex].first))
				{
					dwRightIndex = dwMidIndex;
				}
				dwMidIndex = (dwLeftIndex + dwRightIndex) / 2;
			}
			if (m_oEqual(m_pKeyStores[dwMidIndex].first, k))
			{
				return dwMidIndex;
			}
			return 0XFFFFFFFF;
		}

	private:
		unsigned int m_dwSize;
		unsigned int m_dwCapcity;
		KVPair* m_pKeyStores;

		KeyLess m_oLess;
		Equal<K, KeyLess> m_oEqual;
	};

};

#endif // !__ArrMap_H__
