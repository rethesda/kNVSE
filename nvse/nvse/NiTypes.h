#pragma once
#include "Utilities.h"

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

template <typename T>
T* AllocateNiArray(UInt32 numElems)
{
	const auto msize = sizeof(T) * numElems + sizeof(UInt32);
	auto* arr = static_cast<UInt32*>(GameHeapAlloc(msize));
#if _DEBUG
	memset(arr, 0xCD, msize);
#endif
	*arr = numElems;
	return reinterpret_cast<T*>(arr + 1);
}

// 8
struct NiRTTI
{
	const char	* name;
	NiRTTI		* parent;
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

	bool operator== (const NiPoint3& pt) const
	{
	    return (x == pt.x && y == pt.y && z == pt.z);
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

	NiQuaternion()  { }
	NiQuaternion(float w, float x, float y, float z) : m_fW(w), m_fX(x), m_fY(y), m_fZ(z) { }

	const static NiQuaternion IDENTITY;

	static float Dot(const NiQuaternion& p, const NiQuaternion& q)
	{
		return p.m_fW * q.m_fW + p.m_fX * q.m_fX + 
			   p.m_fY * q.m_fY + p.m_fZ * q.m_fZ;
	}

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
	virtual void* Destroy(UInt32 doFree);

	T_Data*     data;			// 04
	UInt16		capacity;		// 08 - init'd to size of preallocation
	UInt16		firstFreeEntry;	// 0A - index of the first free entry in the block of free entries at the end of the array (or numObjs if full)
	UInt16		numObjs;		// 0C - init'd to 0
	UInt16		growSize;		// 0E - init'd to size of preallocation

	T_Data operator[](UInt32 idx)
	{
		if (idx < firstFreeEntry)
			return data[idx];
		return NULL;
	}

	T_Data Get(UInt32 idx) { return data[idx]; }

	UInt16 Length() { return firstFreeEntry; }
	void AddAtIndex(UInt32 index, T_Data* item);	// no bounds checking
	void SetCapacity(UInt16 newCapacity);	// grow and copy data if needed

	class Iterator
	{
		friend NiTArray;

		T_Data* pData;
		UInt32		count;

	public:
		explicit operator bool() const { return count != 0; }
		void operator++()
		{
			pData++;
			count--;
		}

		T_Data& operator*() const { return *pData; }
		T_Data& operator->() const { return *pData; }
		T_Data& Get() const { return *pData; }

		Iterator(NiTArray& source) : pData(source.data), count(source.firstFreeEntry) {}
	};

	Iterator Begin() { return Iterator(*this); }
};


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
	~NiTLargeArray();

	void	** _vtbl;		// 00
	T		* data;			// 04
	UInt32	capacity;		// 08 - init'd to size of preallocation
	UInt32	firstFreeEntry;	// 0C - index of the first free entry in the block of free entries at the end of the array (or numObjs if full)
	UInt32	numObjs;		// 10 - init'd to 0
	UInt32	growSize;		// 14 - init'd to size of preallocation

	T operator[](UInt32 idx) {
		if (idx < firstFreeEntry)
			return data[idx];
		return NULL;
	}

	T Get(UInt32 idx) { return (*this)[idx]; }

	UInt32 Length(void) { return firstFreeEntry; }
};

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
};

template <typename T_Key, typename T_Data>
struct MapNode
{
	MapNode* next;
	T_Key	key;
	T_Data* data;
};

// 10
// this is a NiTPointerMap <UInt32, T_Data>
// todo: generalize key
template <typename T_Key, typename T_Data>
class NiTPointerMap_t
{
public:
	virtual ~NiTPointerMap_t();

	typedef MapNode<T_Key, T_Data> Entry;

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

	virtual UInt32	CalculateBucket(UInt32 key);
	virtual bool	CompareKey(UInt32 lhs, UInt32 rhs);
	virtual void	Fn_03(UInt32 arg0, UInt32 arg1, UInt32 arg2);	// assign to entry
	virtual void	Fn_04(UInt32 arg);
	virtual void	Fn_05(void);	// locked operations
	virtual void	Fn_06(void);	// locked operations

	T_Data *	Lookup(T_Key key);
	bool		Insert(Entry* nuEntry);

	void Replace(T_Key key, T_Data* data)
	{
		for (Entry* traverse = m_buckets[key % m_numBuckets]; traverse; traverse = traverse->next)
			if (traverse->key == key)
			{
				traverse->data = data;
				break;
			}
	}

	[[nodiscard]] Iterator begin()
	{
		return Iterator(this);
	}

	[[nodiscard]] Iterator end()
	{
		return Iterator(this, static_cast<Entry*>(nullptr), m_numBuckets + 1);
	}


//	void	** _vtbl;		// 0
	UInt32	m_numBuckets;	// 4
	Entry	** m_buckets;	// 8
	UInt32	m_numItems;		// C
};

template <typename T_Key, typename T_Data>
T_Data* NiTPointerMap_t <T_Key, T_Data>::Lookup(T_Key key)
{
	const auto hashNiString = [](const char* str)
	{
		// 0x486DF0
		UInt32 hash = 0;
		while (*str)
		{
			hash = *str + 33 * hash;
			++str;
		}
		return hash;
	};
	UInt32 hashIndex;
	if constexpr (std::is_same_v<T_Key, const char*>)
	{
		hashIndex = hashNiString(key) % m_numBuckets;
	}
	else
	{
		hashIndex = key % m_numBuckets;
	}
	for(Entry * traverse = m_buckets[hashIndex]; traverse; traverse = traverse->next)
	{
		if constexpr (std::is_same_v<T_Key, const char*>)
		{
			if (!_stricmp(traverse->key, key))
			{
				return traverse->data;
			}
		}
		else if (traverse->key == key)
		{
			return traverse->data;
		}
	}
	return nullptr;
}

template <typename T_Key, typename T_Data>
bool NiTPointerMap_t <T_Key, T_Data>::Insert(Entry* nuEntry)
{
	// game code does not appear to care about ordering of entries in buckets
	UInt32 bucket = nuEntry->key % m_numBuckets;
	Entry* prev = NULL;
	for (Entry* cur = m_buckets[bucket]; cur; cur = cur->next) {
		if (cur->key == nuEntry->key) {
			return false;
		}
		else if (!cur->next) {
			prev = cur;
			break;
		}
	}

	if (prev) {
		prev->next = nuEntry;
	}
	else {
		m_buckets[bucket] = nuEntry;
	}

	m_numBuckets++;
	return true;
}

template <typename T_Key, typename T_Data>
T_Data * NiTPointerMap_t <T_Key, T_Data>::Iterator::Get(void)
{
	if(m_entry)
		return m_entry->data;

	return NULL;
}

template <typename T_Key, typename T_Data>
UInt32 NiTPointerMap_t <T_Key, T_Data>::Iterator::GetKey(void)
{
	if(m_entry)
		return m_entry->key;

	return 0;
}

template <typename T_Key, typename T_Data>
bool NiTPointerMap_t <T_Key, T_Data>::Iterator::Next(void)
{
	if(m_entry)
		m_entry = m_entry->next;

	while(!m_entry && (m_bucket < (m_table->m_numBuckets - 1)))
	{
		m_bucket++;

		m_entry = m_table->m_buckets[m_bucket];
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
	if(m_bucket >= m_table->m_numBuckets) return;

	// get bucket
	m_entry = m_table->m_buckets[m_bucket];

	// find non-empty bucket
	while(!m_entry && (m_bucket < (m_table->m_numBuckets - 1)))
	{
		m_bucket++;

		m_entry = m_table->m_buckets[m_bucket];
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
	~NiTMapBase();

	struct Entry
	{
		Entry	* next;	// 000
		T_Key	key;	// 004
		T_Data	data;	// 008
	};

	virtual NiTMapBase<T_Key, T_Data>*	Destructor(bool doFree);						// 000
	virtual UInt32						Hash(T_Key key);								// 001
	virtual void						Equal(T_Key key1, T_Key key2);					// 002
	virtual void						FillEntry(Entry entry, T_Key key, T_Data data);	// 003
	virtual	void						Unk_004(void * arg0);							// 004
	virtual	void						Unk_005(void);									// 005
	virtual	void						Unk_006();										// 006

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
	NiPointer(T *init) : data(init)		{	}

	T* data;
	T& operator *() { return *data; }
	operator T*() { return data; }
	T* operator->() { return data; }
	
	T& operator *() const { return *data; }
	operator T*() const { return data; }
	T* operator->() const { return data; }
};

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
