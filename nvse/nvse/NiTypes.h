#pragma once
#include <span>

#include "Utilities.h"

#define ASSERT_SIZE(name, size) static_assert(sizeof(name) == size, "Size mismatch for " #name)
#define NI_DYNAMIC_CAST(type, ptr) ptr ? ThisStdCall<type*>(0x653290, ptr, type::ms_RTTI) : nullptr

#define NIRTTI_ADDRESS(address) \
	static inline const NiRTTI* const ms_RTTI = (NiRTTI*)address;

#define NiImplementRootRTTI(rootclassname) \
	NiRTTI rootclassname::ms_RTTI(#rootclassname, 0);

#define NiImplementRTTI(classname, baseclassname) \
	static inline const NiRTTI ms_RTTI = NiRTTI(#classname, baseclassname##::ms_RTTI);

#define NiImplementGetRTTI(classname) \
	inline const NiRTTI* GetRTTIEx() {return &classname##::ms_RTTI;}

#define NiNewRTTI(classname, baseclassname) \
	NiImplementRTTI(classname, baseclassname) \
	NiImplementGetRTTI(classname)

#define CREATE_OBJECT(CLASS, ADDRESS) static CLASS* CreateObject() { return CdeclCall<CLASS*>(ADDRESS); };

#if RUNTIME

const UInt32 _NiTMap_Lookup = 0x00853130;

#endif

inline void NIASSERT(bool condition)
{
	if (!condition)
	{
		DebugBreak();
	}
}

// 8
struct NiRTTI
{
	const char* m_pcName;
	const NiRTTI* m_pkBaseRTTI;

	NiRTTI() { m_pcName = nullptr; m_pkBaseRTTI = nullptr; }
	NiRTTI(const char* name, const NiRTTI* const base) { m_pcName = name; m_pkBaseRTTI = base; }
	NiRTTI(const char* name, const NiRTTI& base) { m_pcName = name; m_pkBaseRTTI = &base; }

	bool IsKindOf(const NiRTTI& apRTTI) const {
		for (const NiRTTI* i = this; i; i = i->m_pkBaseRTTI) {
			if (i == &apRTTI)
				return true;
		}
		return false;
	}

	bool IsKindOf(const NiRTTI* apRTTI) const {
		for (const NiRTTI* i = this; i; i = i->m_pkBaseRTTI) {
			if (i == apRTTI)
				return true;
		}
		return false;
	}

	template <typename T_RTTI>
	bool IsKindOf() const {
		return IsKindOf(T_RTTI::ms_RTTI);
	}

	bool IsExactKindOf(const NiRTTI* const apRTTI) const {
		return this == apRTTI;
	}

	bool IsExactKindOf(const NiRTTI& apRTTI) const {
		return this == &apRTTI;
	}

	template <typename T_RTTI>
	bool IsExactKindOf() const {
		return IsExactKindOf(T_RTTI::ms_RTTI);
	}

};

struct NiPoint3
{
	float x, y, z;

	float Dot(const NiPoint3& pt) const
	{
		return x * pt.x + y * pt.y + z * pt.z;
	}

	float Length() const
	{
		return sqrt(x * x + y * y + z * z);
	}

	float SqrLength() const
	{
		return x * x + y * y + z * z;
	}

	NiPoint3 operator-(const NiPoint3& pt) const
	{
		return NiPoint3{ x - pt.x, y - pt.y, z - pt.z };
	}

	NiPoint3 operator+(const NiPoint3& pt) const
	{
		return NiPoint3{ x + pt.x, y + pt.y, z + pt.z };
	}

	const static NiPoint3 ZERO;
	const static NiPoint3 INVALID_POINT;

	bool operator== (const NiPoint3& pt) const
	{
	    return (x == pt.x && y == pt.y && z == pt.z);
	}

	bool IsAlmostEqual(const NiPoint3& pt, float fEpsilon = 0.0001f) const
	{
		return (fabs(x - pt.x) < fEpsilon && fabs(y - pt.y) < fEpsilon && fabs(z - pt.z) < fEpsilon);
	}

	float operator* (const NiPoint3& pt) const
	{
	    return x*pt.x+y*pt.y+z*pt.z;
	}

	NiPoint3 operator* (float fScalar) const
	{
	    return NiPoint3(fScalar*x,fScalar*y,fScalar*z);
	}

	NiPoint3 operator/ (float fScalar) const
	{
	    float fInvScalar = 1.0f/fScalar;
	    return NiPoint3(fInvScalar*x,fInvScalar*y,fInvScalar*z);
	}

	NiPoint3 operator- () const
	{
	    return NiPoint3(-x,-y,-z);
	}

	NiPoint3& operator+= (const NiPoint3& pt)
	{
	    x += pt.x;
	    y += pt.y;
	    z += pt.z;
	    return *this;
	}

	NiPoint3& operator-= (const NiPoint3& pt)
	{
	    x -= pt.x;
	    y -= pt.y;
	    z -= pt.z;
	    return *this;
	}

	NiPoint3& operator*= (float fScalar)
	{
	    x *= fScalar;
	    y *= fScalar;
	    z *= fScalar;
	    return *this;
	}

	NiPoint3& operator/= (float fScalar)
	{
	    float fInvScalar = 1.0f/fScalar;
	    x *= fInvScalar;
	    y *= fInvScalar;
	    z *= fInvScalar;
	    return *this;
	}
};

// 10 - always aligned?
struct NiVector4
{
	float	x, y, z, w;
};

// 24
struct NiMatrix33
{
	float	m_pEntry[3][3];

	float GetEntry( unsigned int uiRow, unsigned int uiCol ) const
	{
		return m_pEntry[uiRow][uiCol];
	}

	void SetCol( unsigned int uiCol, float f0, float f1, float f2 )
	{
		NIASSERT( uiCol <= 2 );
		m_pEntry[0][uiCol] = f0;
		m_pEntry[1][uiCol] = f1;
		m_pEntry[2][uiCol] = f2;
	}

	void GetCol( unsigned int uiCol, float* pCol ) const
	{
		NIASSERT( uiCol <= 2 );
		pCol[0] = m_pEntry[0][uiCol];
		pCol[1] = m_pEntry[1][uiCol];
		pCol[2] = m_pEntry[2][uiCol];
	}
};

using NiMatrix3 = NiMatrix33;

inline float NiSqrt (float fValue)
{
	return float(sqrt(fValue));
}

// 10 - always aligned?
struct NiQuaternion
{
	float m_fW;
	float m_fX;
	float m_fY;
	float m_fZ;

	NiQuaternion(): m_fW(-FLT_MAX), m_fX(-FLT_MAX), m_fY(-FLT_MAX), m_fZ(-FLT_MAX)
	{ }
	NiQuaternion(float w, float x, float y, float z) : m_fW(w), m_fX(x), m_fY(y), m_fZ(z) { }

	bool IsValid() const
	{
		return m_fW != -FLT_MAX;
	}

	const static NiQuaternion IDENTITY;
	const static NiQuaternion INVALID_QUATERNION;

	static float Dot(const NiQuaternion& p, const NiQuaternion& q)
	{
		return p.m_fW * q.m_fW + p.m_fX * q.m_fX + 
			   p.m_fY * q.m_fY + p.m_fZ * q.m_fZ;
	}

	double MagnitudeSquared() const
	{
		return m_fW * m_fW + m_fX * m_fX + m_fY * m_fY + m_fZ * m_fZ;
	}

	void FastNormalize();
	
	static NiQuaternion Slerp(float t, const NiQuaternion& p,
	                          const NiQuaternion& q);

	void Normalize()
	{
		float fLength = m_fW * m_fW + m_fX * m_fX + m_fY * m_fY + m_fZ * m_fZ;
		float fInvLength = 1.0f / NiSqrt(fLength);
		*this = *this * fInvLength;
	}

	void SetValues(float w, float x, float y, float z)
	{
		m_fW = w;
		m_fX = x;
		m_fY = y;
		m_fZ = z;
	}

	void GetValues(float& w, float& x, float& y, float& z) const
	{
		w = m_fW;
		x = m_fX;
		y = m_fY;
		z = m_fZ;
	}
	
	void SetW(float w)
	{
		m_fW = w;
	}
	
	void SetX(float x)
	{
		m_fX = x;
	}
	
	void SetY(float y)
	{
		m_fY = y;
	}
	
	void SetZ(float z)
	{
		m_fZ = z;
	}
	
	float GetW() const
	{
		return m_fW;
	}
	
	float GetX() const
	{
		return m_fX;
	}
	
	float GetY() const
	{
		return m_fY;
	}
	
	float GetZ() const
	{
		return m_fZ;
	}

	NiQuaternion Inverse() const
	{
		float fNorm = m_fW * m_fW + m_fX * m_fX + m_fY * m_fY + m_fZ * m_fZ;
		if (fNorm > 0.0f)
		{
			float fInvNorm = 1.0f / fNorm;
			return NiQuaternion(m_fW * fInvNorm, -m_fX * fInvNorm, -m_fY * fInvNorm, -m_fZ * fInvNorm);
		}
		return IDENTITY;
	}
	
	NiQuaternion operator+ (const NiQuaternion& q) const
	{
	    return NiQuaternion(m_fW + q.m_fW, m_fX + q.m_fX,
	        m_fY + q.m_fY, m_fZ + q.m_fZ);
	}
	
	NiQuaternion operator- (const NiQuaternion& q) const
	{
	    return NiQuaternion(m_fW - q.m_fW, m_fX - q.m_fX,
	        m_fY - q.m_fY, m_fZ - q.m_fZ);
	}
	
	NiQuaternion operator- () const
	{
	    return NiQuaternion(-m_fW, -m_fX, -m_fY, -m_fZ);
	}
	
	NiQuaternion operator* (const NiQuaternion& q) const
	{
	    return NiQuaternion
	    (
	        m_fW * q.m_fW - m_fX * q.m_fX - m_fY * q.m_fY - m_fZ * q.m_fZ,
	        m_fW * q.m_fX + m_fX * q.m_fW + m_fY * q.m_fZ - m_fZ * q.m_fY,
	        m_fW * q.m_fY + m_fY * q.m_fW + m_fZ * q.m_fX - m_fX * q.m_fZ,
	        m_fW * q.m_fZ + m_fZ * q.m_fW + m_fX * q.m_fY - m_fY * q.m_fX
	    );
	}
	
	NiQuaternion operator* (float c) const
	{
		return NiQuaternion(c * m_fW, c * m_fX, c * m_fY, c * m_fZ);
	}

	NiQuaternion& operator*= (float c)
	{
		m_fW *= c;
		m_fX *= c;
		m_fY *= c;
		m_fZ *= c;
		return *this;
	}
	
	void FromRotation(const NiMatrix33& rot)
	{
		// Algorithm in Ken Shoemake's article in 1987 SIGGraPH course notes
		// article "Quaternion Calculus and Fast Animation".

		float fTrace = rot.GetEntry(0,0) + rot.GetEntry(1,1) + rot.GetEntry(2,2);
		float fRoot;

		if ( fTrace > 0.0f )
		{
			// |w| > 1/2, may as well choose w > 1/2
			fRoot = NiSqrt(fTrace+1.0f);  // 2w
			m_fW = 0.5f*fRoot;
			fRoot = 0.5f/fRoot;  // 1/(4w)

			m_fX = (rot.GetEntry( 2,1 ) - rot.GetEntry( 1,2 )) * fRoot;
			m_fY = (rot.GetEntry( 0,2 ) - rot.GetEntry( 2,0 )) * fRoot;
			m_fZ = (rot.GetEntry( 1,0 ) - rot.GetEntry( 0,1 )) * fRoot;
		}
		else
		{
			// |w| <= 1/2
			static int next[3] = { 1, 2, 0 };
			int i = 0;
			if ( rot.GetEntry( 1,1 ) > rot.GetEntry( 0,0 ) )
				i = 1;
			if ( rot.GetEntry( 2,2 ) > rot.GetEntry( i,i ) ) 
				i = 2;
			int j = next[i];
			int k = next[j];

			fRoot = NiSqrt(rot.GetEntry( i,i ) - 
				rot.GetEntry( j,j ) - rot.GetEntry( k,k ) + 1.0f);
			float* quat[3] = { &m_fX, &m_fY, &m_fZ };
			*quat[i] = 0.5f*fRoot;
			fRoot = 0.5f/fRoot;
			m_fW = (rot.GetEntry( k,j ) - rot.GetEntry( j,k )) * fRoot;
			*quat[j] = (rot.GetEntry( j,i ) + rot.GetEntry( i,j )) * fRoot;
			*quat[k] = (rot.GetEntry( k,i ) + rot.GetEntry( i,k )) * fRoot;
		}
	}

	void ToRotation(NiMatrix3& rot) const
	{
		// operations (*,+,-) = 24

		float tx  = 2.0f*m_fX;
		float ty  = 2.0f*m_fY;
		float tz  = 2.0f*m_fZ;
		float twx = tx*m_fW;
		float twy = ty*m_fW;
		float twz = tz*m_fW;
		float txx = tx*m_fX;
		float txy = ty*m_fX;
		float txz = tz*m_fX;
		float tyy = ty*m_fY;
		float tyz = tz*m_fY;
		float tzz = tz*m_fZ;

		rot.SetCol(0, 1.0f-(tyy+tzz), txy+twz, txz-twy);
		rot.SetCol(1, txy-twz, 1.0f-(txx+tzz), tyz+twx);
		rot.SetCol(2, txz+twy, tyz-twx, 1.0f-(txx+tyy));
	}
};


// 34
struct NiTransform
{
	NiMatrix33	m_Rotate;		// 00
	NiPoint3	m_Translate;	// 24
	float		m_fScale;		// 30
};

// 10
struct NiSphere
{
	float	x, y, z, radius;
};

// 1C
struct NiFrustum
{
	float	l;			// 00
	float	r;			// 04
	float	t;			// 08
	float	b;			// 0C
	float	n;			// 10
	float	f;			// 14
	UInt8	o;			// 18
	UInt8	pad19[3];	// 19
};

// 10
struct NiViewport
{
	float	l;
	float	r;
	float	t;
	float	b;
};

// C
struct NiColor
{
	float	r;
	float	g;
	float	b;
};

// 10
struct NiColorAlpha
{
	float	r;
	float	g;
	float	b;
	float	a;
};

// 10
struct NiPlane
{
	NiPoint3	nrm;
	float		offset;
};

// 10
// NiTArrays are slightly weird: they can be sparse
// this implies that they can only be used with types that can be NULL?
// not sure on the above, but some code only works if this is true
// this can obviously lead to fragmentation, but the accessors don't seem to care
// weird stuff
template <typename T_Data>
struct NiTArray
{
	virtual ~NiTArray();

	T_Data*     m_pBase;			// 04
	UInt16		m_usMaxSize;		// 08 - init'd to size of preallocation
	UInt16		m_usSize;	// 0A - index of the first free entry in the block of free entries at the end of the array (or numObjs if full)
	UInt16		m_usESize;		// 0C - init'd to 0
	UInt16		m_usGrowBy;		// 0E - init'd to size of preallocation

	T_Data operator[](UInt32 idx)
	{
		if (idx < m_usSize)
			return m_pBase[idx];
		return NULL;
	}

	UInt16 SizeWithEmpty() const { return m_usSize; }

	UInt16 EffectiveSize() const { return m_usESize; }

	uint32_t Remove(const T_Data& element)
	{
		if (element != T_Data(0)) {
			for (uint16_t i = 0; i < m_usSize; i++) {
				if (m_pBase[i] == element) {
					m_pBase[i] = T_Data(0);

					m_usESize--;
					if (i == m_usSize - 1)
						m_usSize--;

					return i;
				}
			}
		}

		return static_cast<uint32_t>(~0);
	}

	T_Data Get(UInt32 idx) { return m_pBase[idx]; }

	UInt16 Length() { return m_usSize; }
	void AddAtIndex(UInt32 index, T_Data* item);	// no bounds checking
	void SetCapacity(UInt16 newCapacity);	// grow and copy data if needed

	class Iterator
	{
		friend NiTArray;

		T_Data* pData;
		UInt32		count;

	public:
		operator bool() const { return count != 0; }
		void operator++()
		{
			pData++;
			count--;
		}

		T_Data& operator*() const { return *pData; }
		T_Data& operator->() const { return *pData; }
		T_Data& Get() const { return *pData; }
		
		Iterator(T_Data* ptr, UInt32 cnt) : pData(ptr), count(cnt) {}
		Iterator(NiTArray& source) : pData(source.m_pBase), count(source.m_usSize) {}
		bool operator!=(const Iterator& other) const { return count != other.count; }
	};

	Iterator Begin() { return Iterator(*this); }
	Iterator End() { return Iterator(m_pBase + m_usSize, 0); }
	Iterator begin() { return Begin(); }
	Iterator end() { return End(); }
	
};

template <typename T>
using NiTPrimitiveArray = NiTArray<T>;


#if RUNTIME

template <typename T> void NiTArray<T>::AddAtIndex(UInt32 index, T* item)
{
	ThisStdCall(0x00869640, this, index, item);
}

template <typename T> void NiTArray<T>::SetCapacity(UInt16 newCapacity)
{
	ThisStdCall(0x008696E0, this, newCapacity);
}

#endif

// 18
// an NiTArray that can go above 0xFFFF, probably with all the same weirdness
// this implies that they make fragmentable arrays with 0x10000 elements, wtf
template <typename T>
class NiTLargeArray
{
public:
	NiTLargeArray();
	virtual ~NiTLargeArray();

	T		* data;			// 04
	UInt32	m_uiMaxSize;		// 08 - init'd to size of preallocation
	UInt32	m_uiSize;	// 0C - index of the first free entry in the block of free entries at the end of the array (or numObjs if full)
	UInt32	m_uiESize;		// 10 - init'd to 0
	UInt32	m_uiGrowBy;		// 14 - init'd to size of preallocation

	T operator[](UInt32 idx) {
		if (idx < m_uiSize)
			return data[idx];
		return NULL;
	}

	T Get(UInt32 idx) { return (*this)[idx]; }

	UInt32 Length() { return m_uiSize; }

	std::span<T> GetItems() { return std::span<T>(data, m_uiSize); }
};

using NiFormArray = NiTLargeArray<TESForm*>;

// 8
template <typename T>
struct NiTSet
{
	T		* data;		// 00
	UInt32	capacity;	// 04
	UInt32	length;		// 06

	struct Iterator
	{
		friend NiTSet;

		UInt32 index;
		NiTSet* set;

		T& operator*() const { return set->data[index]; }

		Iterator& operator++()
		{
			index++;
			return *this;
		}

		bool operator!=(const Iterator& other)
		{
			return index != other.index;
		}
	};

	Iterator begin()
	{
		return Iterator{ 0, this };
	}

	Iterator end()
	{
		return Iterator{ length, this };
	}

	std::span<T> ToSpan() const
	{
		return std::span(data, length);
	}
};

template <typename T_Key, typename T_Data>
struct MapNode
{
	MapNode* m_pkNext;
	T_Key	m_key;
	T_Data m_val;
};

// 10
// this is a NiTPointerMap <UInt32, T_Data>
// todo: generalize key
template <typename T_Key, typename T_Data>
class NiTPointerMap_t
{
public:
	typedef MapNode<T_Key, T_Data> Entry;
	UInt32	m_uiHashSize;	// 4
	Entry	** m_ppkHashTable;	// 8
	UInt32	m_uiCount;		// C
	
	virtual ~NiTPointerMap_t();
	
	// note: traverses in non-numerical order
	class Iterator
	{
		friend NiTPointerMap_t;

	public:
		Iterator(NiTPointerMap_t* table, Entry * entry = nullptr, UInt32 bucket = 0)
			:m_table(table), m_entry(entry), m_bucket(bucket) { FindValid(); }
		~Iterator() { }

		T_Data *	Get(void);
		UInt32		GetKey(void);
		bool		Next(void);
		bool		Done(void);

		Iterator operator++()
		{
			Next();
			return *this;
		}

		Entry* operator*() const
		{
			return m_entry;
		}

		bool operator!=(const Iterator& other)
		{
			return m_entry != other.m_entry;
		}

	private:
		void		FindValid(void);

		NiTPointerMap_t* m_table;
		Entry		* m_entry;
		UInt32		m_bucket;
	};

	virtual UInt32	KeyToHashIndex(const T_Key key);
	virtual bool	IsKeysEqual(const T_Key key1, const T_Key key2);
	virtual void	SetValue(Entry* node, T_Key key, T_Data data);	// assign to entry
	virtual void	ClearValue(Entry* apItem);
	virtual void	NewItem();	// locked operations
	virtual void	DeleteItem(Entry* entry);	// locked operations

	T_Data	Lookup(T_Key key);
	bool		Insert(Entry* nuEntry);

	Entry* GetEntry(T_Key key)
	{
		auto hashIndex = KeyToHashIndex(key);
		auto* item = m_ppkHashTable[hashIndex];
		while (item)
		{
			if (IsKeysEqual(item->m_key, key))
			{
				return item;
			}
			item = item->m_pkNext;
		}
		return nullptr;
	}

	bool RemoveAt(T_Key key)
	{
		// look up hash table location for key
		unsigned int uiIndex = KeyToHashIndex(key);
		auto* pkItem = m_ppkHashTable[uiIndex];

		// search list at hash table location for key
		if (pkItem)
		{
			if (IsKeysEqual(key, pkItem->m_key))
			{
				// item at front of list, remove it
				m_ppkHashTable[uiIndex] = pkItem->m_pkNext;
				ClearValue(pkItem);
            
				DeleteItem(pkItem);

				m_uiCount--;
				return true;
			}
			else
			{
				// search rest of list for item
				auto* pkPrev = pkItem;
				auto* pkCurr = pkPrev->m_pkNext;
				while (pkCurr && !IsKeysEqual(key, pkCurr->m_key))
				{
					pkPrev = pkCurr;
					pkCurr = pkCurr->m_pkNext;
				}
				if (pkCurr)
				{
					// found the item, remove it
					pkPrev->m_pkNext = pkCurr->m_pkNext;
					ClearValue(pkCurr);
                
					DeleteItem(pkCurr);

					m_uiCount--;
					return true;
				}
			}
		}

		return false;
	}

	void Clear()
	{
		ThisStdCall(0x438AF0, this);
	}

	[[nodiscard]] Iterator begin()
	{
		return Iterator(this);
	}

	[[nodiscard]] Iterator end()
	{
		return Iterator(this, static_cast<Entry*>(nullptr), m_uiHashSize + 1);
	}


//	void	** _vtbl;		// 0

};

template <typename T_Key, typename T_Data>
T_Data NiTPointerMap_t <T_Key, T_Data>::Lookup(T_Key key)
{
	auto hashIndex = KeyToHashIndex(key);
	auto* item = m_ppkHashTable[hashIndex];
	while (item)
	{
		if (IsKeysEqual(item->m_key, key))
		{
			return item->m_val;
		}
		item = item->m_pkNext;
	}
	return nullptr;
}

template <typename T_Key, typename T_Data>
bool NiTPointerMap_t <T_Key, T_Data>::Insert(Entry* nuEntry)
{
	// game code does not appear to care about ordering of entries in buckets
	UInt32 bucket = KeyToHashIndex(nuEntry->m_key);
	Entry* prev = nullptr;
	for (Entry* cur = m_ppkHashTable[bucket]; cur; cur = cur->m_pkNext) {
		if (IsKeysEqual(cur->m_key, nuEntry->m_key)) {
			return false;
		}
		if (!cur->m_pkNext) {
			prev = cur;
			break;
		}
	}

	if (prev) {
		prev->m_pkNext = nuEntry;
	}
	else {
		m_ppkHashTable[bucket] = nuEntry;
	}

	m_uiHashSize++;
	return true;
}

template <typename T_Key, typename T_Data>
T_Data * NiTPointerMap_t <T_Key, T_Data>::Iterator::Get(void)
{
	if(m_entry)
		return m_entry->m_val;

	return NULL;
}

template <typename T_Key, typename T_Data>
UInt32 NiTPointerMap_t <T_Key, T_Data>::Iterator::GetKey(void)
{
	if(m_entry)
		return m_entry->m_key;

	return 0;
}

template <typename T_Key, typename T_Data>
bool NiTPointerMap_t <T_Key, T_Data>::Iterator::Next(void)
{
	if(m_entry)
		m_entry = m_entry->m_pkNext;

	while(!m_entry && (m_bucket < (m_table->m_uiHashSize - 1)))
	{
		m_bucket++;

		m_entry = m_table->m_ppkHashTable[m_bucket];
	}

	return m_entry != NULL;
}

template <typename T_Key, typename T_Data>
bool NiTPointerMap_t <T_Key, T_Data>::Iterator::Done(void)
{
	return m_entry == NULL;
}

template <typename T_Key, typename T_Data>
void NiTPointerMap_t <T_Key, T_Data>::Iterator::FindValid(void)
{
	// validate bucket
	if(m_bucket >= m_table->m_uiHashSize) return;

	// get bucket
	m_entry = m_table->m_ppkHashTable[m_bucket];

	// find non-empty bucket
	while(!m_entry && (m_bucket < (m_table->m_uiHashSize - 1)))
	{
		m_bucket++;

		m_entry = m_table->m_ppkHashTable[m_bucket];
	}
}

template <typename T_Data>
using NiTPointerMap = NiTPointerMap_t <UInt32, T_Data>;

// 10
// todo: NiTPointerMap should derive from this
// cleaning that up now could cause problems, so it will wait
template <typename T_Key, typename T_Data>
class NiTMapBase
{
public:
	NiTMapBase();
	virtual~NiTMapBase();

	struct Entry
	{
		Entry	* next;	// 000
		T_Key	key;	// 004
		T_Data	data;	// 008
	};

	virtual UInt32						Hash(T_Key key);								// 001
	virtual void						Equal(T_Key key1, T_Key key2);					// 002
	virtual void						SetValue(Entry* entry, T_Key key, T_Data data);	// 003
	virtual	void						NullSub(Entry* entry);							// 004
	virtual	void						NewItem();									// 005
	virtual	void						DeleteItem(Entry* entry);										// 006

	Entry* Begin()
	{
		return buckets[0];
	}

	//void	** _vtbl;	// 0
	UInt32	numBuckets;	// 4
	Entry	** buckets;	// 8
	UInt32	numItems;	// C

	DEFINE_MEMBER_FN_LONG(NiTMapBase, Lookup, bool, _NiTMap_Lookup, T_Key key, T_Data * dataOut);
};

// 14
template <typename T_Data>
class NiTStringPointerMap : public NiTPointerMap_t <const char*, T_Data>
{
public:
	virtual ~NiTStringPointerMap();

	UInt32	unk010;
};

// not sure how much of this is in NiTListBase and how much is in NiTPointerListBase
// 10
template <typename T>
class NiTListBase
{
public:
	NiTListBase();
	~NiTListBase();

	struct Node
	{
		Node	* next;
		Node	* prev;
		T		* data;
	};

	virtual void	Destructor(void);
	virtual Node *	AllocateNode(void);
	virtual void	FreeNode(Node * node);

//	void	** _vtbl;	// 000
	Node	* start;	// 004
	Node	* end;		// 008
	UInt32	numItems;	// 00C
};

// 10
template <typename T>
class NiTPointerListBase : public NiTListBase <T>
{
public:
	NiTPointerListBase();
	~NiTPointerListBase();
};

// 10
template <typename T>
class NiTPointerList : public NiTPointerListBase <T>
{
public:
	NiTPointerList();
	~NiTPointerList();
};

// 4
template <typename T>
class NiPointer
{
public:
	NiPointer(T *init) : data(init)
	{
		if (data)
			data->IncrementRefCount();
	}

	NiPointer(const NiPointer& other) : data(other.data)
	{
		if (data)
			data->IncrementRefCount();
	}

	~NiPointer()
	{
		if (data)
			data->DecrementRefCount();
	}

	NiPointer& operator=(const NiPointer& other)
	{
		if (data)
			data->DecrementRefCount();
		data = other.data;
		if (data)
			data->IncrementRefCount();
		return *this;
	}

	NiPointer& operator=(T* other)
	{
		if (data)
			data->DecrementRefCount();
		data = other;
		if (data)
			data->IncrementRefCount();
		return *this;
	}

	T* data;
	T& operator *() { return *data; }
	operator T*() { return data; }
	T* operator->() { return data; }
	
	T& operator *() const { return *data; }
	operator T*() const { return data; }
	T* operator->() const { return data; }
	
};

template <typename T>
struct std::hash<NiPointer<T>>
{
	std::size_t operator()(const NiPointer<T>& ptr) const
	{
		return std::hash<T*>()(ptr.data);
	}
};

#define NiSmartPointer(className) \
	class className; \
	typedef NiPointer<className> className##Ptr;

class NiCriticalSection {
public:
	CRITICAL_SECTION	m_kCriticalSection;
	UInt32				m_ulThreadOwner;
	UInt32				m_uiLockCount;

	void Lock() {
		EnterCriticalSection(&m_kCriticalSection); 
	}

	void Unlock() {
		LeaveCriticalSection(&m_kCriticalSection);
	}
};

ASSERT_SIZE(NiCriticalSection, 0x20);

// 14
template <typename T>
class BSTPersistentList
{
public:
	BSTPersistentList();
	~BSTPersistentList();

	virtual void	Destroy(bool destroy);

//	void	** _vtbl;	// 00
	UInt32	unk04;		// 04
	UInt32	unk08;		// 08
	UInt32	unk0C;		// 0C
	UInt32	unk10;		// 10
};
