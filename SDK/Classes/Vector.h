#pragma once

#include <cmath>
#include <limits>

#include "BaseTypes.h"
#include "Vector2D.h"
#include "Random.h"
#include "Platform.h"

#define CHECK_VALID( _v)	0

class Vector {
public:
	float x, y, z;

	Vector(void);
	Vector(vec_t X, vec_t  Y, vec_t  Z);
	void Init(vec_t ix = 0.0f, vec_t iy = 0.0f, vec_t iz = 0.0f);

	bool IsValid() const;
	bool IsReasonable(vec_t range = 1000000) const;		///< Check for reasonably-sized values (if used as a game world position)
	void Invalidate();

	// array access...
	vec_t operator[](int i) const;
	vec_t& operator[](int i);

	// Base address...
	vec_t* Base();
	vec_t const* Base() const;

	// Cast to Vector2D...
	Vector2D& AsVector2D();
	const Vector2D& AsVector2D() const;

	// Initialization methods
	void Random(vec_t minVal, vec_t maxVal);
	inline void Zero(); ///< zero out a vector

	// equality
	bool operator==(const Vector& v) const;
	bool operator!=(const Vector& v) const;

	// arithmetic operations
	FORCEINLINE Vector& operator+=(const Vector& v);
	FORCEINLINE Vector& operator-=(const Vector& v);
	FORCEINLINE Vector& operator*=(const Vector& v);
	FORCEINLINE Vector& operator*=(float s);
	FORCEINLINE Vector& operator/=(const Vector& v);
	FORCEINLINE Vector& operator/=(float s);
	FORCEINLINE Vector& operator+=(float fl); ///< broadcast add
	FORCEINLINE Vector& operator-=(float fl); ///< broadcast sub		

	// negate the vector components
	void	Negate();

	// Get the vector's magnitude.
	inline vec_t	Length() const;

	// Get the vector's magnitude squared.
	FORCEINLINE vec_t LengthSqr(void) const
	{
		CHECK_VALID(*this);
		return (x * x + y * y + z * z);
	}

	// Get one over the vector's length
	// via fast hardware approximation
	//inline vec_t LengthRecipFast(void) const
	//{
	//	return FastRSqrtFast(LengthSqr());
	//}

	// return true if this vector is (0,0,0) within tolerance
	bool IsZero(float tolerance = 0.01f) const
	{
		return (x > -tolerance && x < tolerance &&
			y > -tolerance && y < tolerance &&
			z > -tolerance && z < tolerance);
	}


	// return true if this vector is exactly (0,0,0) -- only fast if vector is coming from memory, not registers
	//inline bool IsZeroFast() const RESTRICT
	//{
	//	COMPILE_TIME_ASSERT(sizeof(vec_t) == sizeof(int));
	//	return (*reinterpret_cast<const int*>(&x) == 0 &&
	//		*reinterpret_cast<const int*>(&y) == 0 &&
	//		*reinterpret_cast<const int*>(&z) == 0);
	//}

	vec_t	NormalizeInPlace();								///< Normalize all components
	vec_t	NormalizeInPlaceSafe(const Vector& vFallback);///< Normalize all components
	Vector	Normalized() const;								///< Return normalized vector
	Vector	NormalizedSafe(const Vector& vFallback)const;		///< Return normalized vector, falling back to vFallback if the length of this is 0
	bool	IsLengthGreaterThan(float val) const;
	bool	IsLengthLessThan(float val) const;

	// check if a vector is within the box defined by two other vectors
	FORCEINLINE bool WithinAABox(Vector const& boxmin, Vector const& boxmax);

	// Get the distance from this vector to the other one.
	vec_t	DistTo(const Vector& vOther) const;

	// Get the distance from this vector to the other one squared.
	// NJS: note, VC wasn't inlining it correctly in several deeply nested inlines due to being an 'out of line' inline.  
	// may be able to tidy this up after switching to VC7
	FORCEINLINE vec_t DistToSqr(const Vector& vOther) const
	{
		Vector delta;

		delta.x = x - vOther.x;
		delta.y = y - vOther.y;
		delta.z = z - vOther.z;

		return delta.LengthSqr();
	}

	// Copy
	void	CopyToArray(float* rgfl) const;

	// Multiply, add, and assign to this (ie: *this = a + b * scalar). This
	// is about 12% faster than the actual vector equation (because it's done per-component
	// rather than per-vector).
	void	MulAdd(const Vector& a, const Vector& b, float scalar);

	// Dot product.
	vec_t	Dot(const Vector& vOther) const;

	// assignment
	Vector& operator=(const Vector& vOther);

	// returns 0, 1, 2 corresponding to the component with the largest absolute value
	inline int LargestComponent() const;
	inline vec_t LargestComponentValue() const;
	inline int SmallestComponent() const;
	inline vec_t SmallestComponentValue() const;

	// 2d
	vec_t	Length2D(void) const;
	vec_t	Length2DSqr(void) const;

	/// get the component of this vector parallel to some other given vector
	inline Vector  ProjectOnto(const Vector& onto);

	//operator VectorByValue& () { return *((VectorByValue*)(this)); }
	//operator const VectorByValue& () const { return *((const VectorByValue*)(this)); }

	// copy constructors
	//	Vector(const Vector &vOther);

	// arithmetic operations
	Vector	operator-(void) const;

	Vector	operator+(const Vector& v) const;
	Vector	operator-(const Vector& v) const;
	Vector	operator*(const Vector& v) const;
	Vector	operator/(const Vector& v) const;
	Vector	operator*(float fl) const;
	Vector	operator/(float fl) const;

	// Cross product between two vectors.
	Vector	Cross(const Vector& vOther) const;

	// Returns a vector with the min or max in X, Y, and Z.
	Vector	Min(const Vector& vOther) const;
	Vector	Max(const Vector& vOther) const;





	//Custom
	float NormalizeMovement() {
		Vector res = *this;
		float l = res.LengthSqr();
		if (l != 0.0f)
		{
			res /= l;
		}
		else
		{
			res.x = res.y = res.z = 0.0f;
		}
		return l;
	}
};

inline Vector::Vector(void)
{

}

inline Vector::Vector(vec_t X, vec_t Y, vec_t Z)
{
	x = X; y = Y; z = Z;
	CHECK_VALID(*this);
}

inline void Vector::Init(vec_t ix, vec_t iy, vec_t iz)
{
	x = ix; y = iy; z = iz;
	CHECK_VALID(*this);
}

inline void Vector::Random(vec_t minVal, vec_t maxVal)
{
	x = RandomFloat(minVal, maxVal);
	y = RandomFloat(minVal, maxVal);
	z = RandomFloat(minVal, maxVal);
	CHECK_VALID(*this);
}

// This should really be a single opcode on the PowerPC (move r0 onto the vec reg)
inline void Vector::Zero()
{
	x = y = z = 0.0f;
}

inline void VectorClear(Vector& a)
{
	a.x = a.y = a.z = 0.0f;
}

inline Vector& Vector::operator=(const Vector& vOther)
{
	CHECK_VALID(vOther);
	x = vOther.x; y = vOther.y; z = vOther.z;
	return *this;
}

//-----------------------------------------------------------------------------
// Array access
//-----------------------------------------------------------------------------
inline vec_t& Vector::operator[](int i)
{
	//Assert((i >= 0) && (i < 3));
	return ((vec_t*)this)[i];
}

inline vec_t Vector::operator[](int i) const
{
	//Assert((i >= 0) && (i < 3));
	return ((vec_t*)this)[i];
}

inline vec_t* Vector::Base()
{
	return (vec_t*)this;
}

inline vec_t const* Vector::Base() const
{
	return (vec_t const*)this;
}

//-----------------------------------------------------------------------------
// Cast to Vector2D...
//-----------------------------------------------------------------------------

inline Vector2D& Vector::AsVector2D()
{
	return *(Vector2D*)this;
}

inline const Vector2D& Vector::AsVector2D() const
{
	return *(const Vector2D*)this;
}

//-----------------------------------------------------------------------------
// IsValid?
//-----------------------------------------------------------------------------

inline bool Vector::IsValid() const
{
	return IsFinite(x) && IsFinite(y) && IsFinite(z);
}

//-----------------------------------------------------------------------------
// IsReasonable?
//-----------------------------------------------------------------------------
inline bool Vector::IsReasonable(vec_t range) const
{
	return (Length() < range);
}

//-----------------------------------------------------------------------------
// Invalidate
//-----------------------------------------------------------------------------

inline void Vector::Invalidate()
{
	//#ifdef _DEBUG
	//#ifdef VECTOR_PARANOIA
	x = y = z = VEC_T_NAN;
	//#endif
	//#endif
}

//-----------------------------------------------------------------------------
// comparison
//-----------------------------------------------------------------------------

inline bool Vector::operator==(const Vector& src) const
{
	CHECK_VALID(src);
	CHECK_VALID(*this);
	return (src.x == x) && (src.y == y) && (src.z == z);
}

inline bool Vector::operator!=(const Vector& src) const
{
	CHECK_VALID(src);
	CHECK_VALID(*this);
	return (src.x != x) || (src.y != y) || (src.z != z);
}

//-----------------------------------------------------------------------------
// Copy
//-----------------------------------------------------------------------------

FORCEINLINE void VectorCopy(const Vector& src, Vector& dst)
{
	CHECK_VALID(src);
	dst.x = src.x;
	dst.y = src.y;
	dst.z = src.z;
}

inline void	Vector::CopyToArray(float* rgfl) const
{
	//Assert(rgfl);
	CHECK_VALID(*this);
	rgfl[0] = x, rgfl[1] = y, rgfl[2] = z;
}

//-----------------------------------------------------------------------------
// standard math operations
//-----------------------------------------------------------------------------
// #pragma message("TODO: these should be SSE")

inline void Vector::Negate()
{
	CHECK_VALID(*this);
	x = -x; y = -y; z = -z;
}

//-----------------------------------------------------------------------------
// dot, cross
//-----------------------------------------------------------------------------
FORCEINLINE vec_t DotProduct(const Vector& a, const Vector& b)
{
	CHECK_VALID(a);
	CHECK_VALID(b);
	return(a.x * b.x + a.y * b.y + a.z * b.z);
}

inline void CrossProduct(const Vector& a, const Vector& b, Vector& result)
{
	CHECK_VALID(a);
	CHECK_VALID(b);
	//Assert(&a != &result);
	//Assert(&b != &result);
	result.x = a.y * b.z - a.z * b.y;
	result.y = a.z * b.x - a.x * b.z;
	result.z = a.x * b.y - a.y * b.x;
}

// for backwards compatability
inline vec_t Vector::Dot(const Vector& vOther) const
{
	CHECK_VALID(vOther);
	return DotProduct(*this, vOther);
}

//-----------------------------------------------------------------------------
// length
//-----------------------------------------------------------------------------

inline vec_t VectorLength(const Vector& v)
{
	CHECK_VALID(v);
	return (vec_t)std::sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

inline vec_t Vector::Length(void) const
{
	CHECK_VALID(*this);
	return VectorLength(*this);
}


FORCEINLINE  Vector& Vector::operator+=(const Vector& v)
{
	CHECK_VALID(*this);
	CHECK_VALID(v);
	x += v.x; y += v.y; z += v.z;
	return *this;
}

FORCEINLINE  Vector& Vector::operator-=(const Vector& v)
{
	CHECK_VALID(*this);
	CHECK_VALID(v);
	x -= v.x; y -= v.y; z -= v.z;
	return *this;
}

FORCEINLINE  Vector& Vector::operator*=(float fl)
{
	x *= fl;
	y *= fl;
	z *= fl;
	CHECK_VALID(*this);
	return *this;
}

FORCEINLINE  Vector& Vector::operator*=(const Vector& v)
{
	CHECK_VALID(v);
	x *= v.x;
	y *= v.y;
	z *= v.z;
	CHECK_VALID(*this);
	return *this;
}

// this ought to be an opcode.
FORCEINLINE Vector& Vector::operator+=(float fl)
{
	x += fl;
	y += fl;
	z += fl;
	CHECK_VALID(*this);
	return *this;
}

FORCEINLINE Vector& Vector::operator-=(float fl)
{
	x -= fl;
	y -= fl;
	z -= fl;
	CHECK_VALID(*this);
	return *this;
}



FORCEINLINE  Vector& Vector::operator/=(float fl)
{
	//Assert(fl != 0.0f);
	float oofl = 1.0f / fl;
	x *= oofl;
	y *= oofl;
	z *= oofl;
	CHECK_VALID(*this);
	return *this;
}

FORCEINLINE  Vector& Vector::operator/=(const Vector& v)
{
	CHECK_VALID(v);
	//Assert(v.x != 0.0f && v.y != 0.0f && v.z != 0.0f);
	x /= v.x;
	y /= v.y;
	z /= v.z;
	CHECK_VALID(*this);
	return *this;
}


// get the component of this vector parallel to some other given vector
inline Vector Vector::ProjectOnto(const Vector& onto)
{
	return onto * (this->Dot(onto) / (onto.LengthSqr()));
}

inline vec_t Vector::Length2D(void) const
{
	return (vec_t)std::sqrtf(x * x + y * y);
}

inline vec_t Vector::Length2DSqr(void) const
{
	return (x * x + y * y);
}

inline void VectorAdd(const Vector& a, const Vector& b, Vector& result)
{
	CHECK_VALID(a);
	CHECK_VALID(b);
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
}

inline void VectorMA(const Vector& start, float scale, const Vector& direction, Vector& dest)
{
	CHECK_VALID(start);
	CHECK_VALID(direction);
	dest.x = start.x + scale * direction.x;
	dest.y = start.y + scale * direction.y;
	dest.z = start.z + scale * direction.z;
}

FORCEINLINE void VectorSubtract(const Vector& a, const Vector& b, Vector& c)
{
	CHECK_VALID(a);
	CHECK_VALID(b);
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
}

FORCEINLINE void VectorMultiply(const Vector& a, vec_t b, Vector& c)
{
	CHECK_VALID(a);
	//Assert(IsFinite(b));
	c.x = a.x * b;
	c.y = a.y * b;
	c.z = a.z * b;
}

FORCEINLINE void VectorMultiply(const Vector& a, const Vector& b, Vector& c)
{
	CHECK_VALID(a);
	CHECK_VALID(b);
	c.x = a.x * b.x;
	c.y = a.y * b.y;
	c.z = a.z * b.z;
}

// for backwards compatability
inline void VectorScale(const Vector& in, vec_t scale, Vector& result)
{
	VectorMultiply(in, scale, result);
}


FORCEINLINE void VectorDivide(const Vector& a, vec_t b, Vector& c)
{
	CHECK_VALID(a);
	//Assert(b != 0.0f);
	vec_t oob = 1.0f / b;
	c.x = a.x * oob;
	c.y = a.y * oob;
	c.z = a.z * oob;
}

FORCEINLINE void VectorDivide(const Vector& a, const Vector& b, Vector& c)
{
	CHECK_VALID(a);
	CHECK_VALID(b);
	//Assert((b.x != 0.0f) && (b.y != 0.0f) && (b.z != 0.0f));
	c.x = a.x / b.x;
	c.y = a.y / b.y;
	c.z = a.z / b.z;
}

// FIXME: Remove
// For backwards compatability
inline void	Vector::MulAdd(const Vector& a, const Vector& b, float scalar)
{
	CHECK_VALID(a);
	CHECK_VALID(b);
	x = a.x + b.x * scalar;
	y = a.y + b.y * scalar;
	z = a.z + b.z * scalar;
}

inline void VectorLerp(const Vector& src1, const Vector& src2, vec_t t, Vector& dest)
{
	CHECK_VALID(src1);
	CHECK_VALID(src2);
	dest.x = src1.x + (src2.x - src1.x) * t;
	dest.y = src1.y + (src2.y - src1.y) * t;
	dest.z = src1.z + (src2.z - src1.z) * t;
}

inline Vector VectorLerp(const Vector& src1, const Vector& src2, vec_t t)
{
	Vector result;
	VectorLerp(src1, src2, t, result);
	return result;
}

//-----------------------------------------------------------------------------
// arithmetic operations
//-----------------------------------------------------------------------------

inline Vector Vector::operator-(void) const
{
	return Vector(-x, -y, -z);
}

inline Vector Vector::operator+(const Vector& v) const
{
	Vector res;
	VectorAdd(*this, v, res);
	return res;
}

inline Vector Vector::operator-(const Vector& v) const
{
	Vector res;
	VectorSubtract(*this, v, res);
	return res;
}

inline Vector Vector::operator*(float fl) const
{
	Vector res;
	VectorMultiply(*this, fl, res);
	return res;
}

inline Vector Vector::operator*(const Vector& v) const
{
	Vector res;
	VectorMultiply(*this, v, res);
	return res;
}

inline Vector Vector::operator/(float fl) const
{
	Vector res;
	VectorDivide(*this, fl, res);
	return res;
}

inline Vector Vector::operator/(const Vector& v) const
{
	Vector res;
	VectorDivide(*this, v, res);
	return res;
}

inline Vector operator*(float fl, const Vector& v)
{
	return v * fl;
}

inline vec_t Vector::NormalizeInPlace()
{
	const float length = this->Length();
	const float radius = 1.0f / (length + std::numeric_limits< float >::epsilon());

	this->x *= radius;
	this->y *= radius;
	this->z *= radius;

	return length;
}

class ALIGN16 VectorAligned : public Vector
{
public:
	inline VectorAligned(void) {};
	inline VectorAligned(vec_t X, vec_t Y, vec_t Z)
	{
		Init(X, Y, Z);
	}

#ifdef VECTOR_NO_SLOW_OPERATIONS

private:
	// No copy constructors allowed if we're in optimal mode
	VectorAligned(const VectorAligned& vOther);
	VectorAligned(const Vector& vOther);

#else
public:
	explicit VectorAligned(const Vector& vOther)
	{
		Init(vOther.x, vOther.y, vOther.z);
	}

	VectorAligned& operator=(const Vector& vOther)
	{
		Init(vOther.x, vOther.y, vOther.z);
		return *this;
	}

	VectorAligned& operator=(const VectorAligned& vOther)
	{
		// we know we're aligned, so use simd
		// we can't use the convenient abstract interface coz it gets declared later
#ifdef _X360
		XMStoreVector4A(Base(), XMLoadVector4A(vOther.Base()));
#elif _WIN32
		_mm_store_ps(Base(), _mm_load_ps(vOther.Base()));
#else
		Init(vOther.x, vOther.y, vOther.z);
#endif
		return *this;
	}


#endif
	float w;	// this space is used anyway
//
//#if !defined(NO_MALLOC_OVERRIDE)
//	void* operator new[](size_t nSize)
//	{
//		return MemAlloc_AllocAligned(nSize, 16);
//	}
//
//	void* operator new[](size_t nSize, const char* pFileName, int nLine)
//	{
//		return MemAlloc_AllocAlignedFileLine(nSize, 16, pFileName, nLine);
//	}
//
//	void* operator new[](size_t nSize, int /*nBlockUse*/, const char* pFileName, int nLine)
//	{
//		return MemAlloc_AllocAlignedFileLine(nSize, 16, pFileName, nLine);
//	}
//
//	void operator delete[](void* p)
//	{
//		MemAlloc_FreeAligned(p);
//	}
//
//	void operator delete[](void* p, const char* pFileName, int nLine)
//	{
//		MemAlloc_FreeAligned(p, pFileName, nLine);
//	}
//
//	void operator delete[](void* p, int /*nBlockUse*/, const char* pFileName, int nLine)
//	{
//		MemAlloc_FreeAligned(p, pFileName, nLine);
//	}
//
//	// please don't allocate a single quaternion...
//	void* operator new   (size_t nSize)
//	{
//		return MemAlloc_AllocAligned(nSize, 16);
//	}
//	void* operator new   (size_t nSize, const char* pFileName, int nLine)
//	{
//		return MemAlloc_AllocAlignedFileLine(nSize, 16, pFileName, nLine);
//	}
//	void* operator new   (size_t nSize, int /*nBlockUse*/, const char* pFileName, int nLine)
//	{
//		return MemAlloc_AllocAlignedFileLine(nSize, 16, pFileName, nLine);
//	}
//	void operator delete (void* p)
//	{
//		MemAlloc_FreeAligned(p);
//	}
//
//	void operator delete (void* p, const char* pFileName, int nLine)
//	{
//		MemAlloc_FreeAligned(p, pFileName, nLine);
//	}
//
//	void operator delete (void* p, int /*nBlockUse*/, const char* pFileName, int nLine)
//	{
//		MemAlloc_FreeAligned(p, pFileName, nLine);
//	}
//#endif
} ALIGN16_POST;

//typedef __m128 fltx4;
//typedef const fltx4& FLTX4;
//
//FORCEINLINE fltx4 AddSIMD(const fltx4& a, const fltx4& b)				// a+b
//{
//	return _mm_add_ps(a, b);
//}
//
//FORCEINLINE fltx4 SubSIMD(const fltx4& a, const fltx4& b)				// a-b
//{
//	return _mm_sub_ps(a, b);
//};
//
//FORCEINLINE fltx4 MulSIMD(const fltx4& a, const fltx4& b)				// a*b
//{
//	return _mm_mul_ps(a, b);
//};
//
//FORCEINLINE fltx4 DivSIMD(const fltx4& a, const fltx4& b)				// a/b
//{
//	return _mm_div_ps(a, b);
//};
//
//FORCEINLINE fltx4 MaddSIMD(const fltx4& a, const fltx4& b, const fltx4& c)				// a*b + c
//{
//	return AddSIMD(MulSIMD(a, b), c);
//}
//
//FORCEINLINE fltx4 ReplicateX4(float flValue)
//{
//	__m128 value = _mm_set_ss(flValue);
//	return _mm_shuffle_ps(value, value, 0);
//}
//
//FORCEINLINE fltx4 ReplicateX4(const float* flValue)
//{
//	__m128 value = _mm_set_ss(*flValue);
//	return _mm_shuffle_ps(value, value, 0);
//}
//// Componentwise multiply
//FORCEINLINE fltx4 operator*(FLTX4 a, FLTX4 b)
//{
//	return MulSIMD(a, b);
//}
//// Componentwise subtract
//FORCEINLINE fltx4 operator-(FLTX4 a, FLTX4 b)
//{
//	return SubSIMD(a, b);
//}
//extern const fltx4 Four_Zeros;
//// Return one in the fastest way -- on the x360, faster even than loading.
//FORCEINLINE fltx4 LoadZeroSIMD(void)
//{
//	return Four_Zeros;
//}
//
//FORCEINLINE fltx4 NegSIMD(const fltx4& a) // negate: -a
//{
//	return SubSIMD(LoadZeroSIMD(), a);
//}
//// unary negate
//FORCEINLINE fltx4 operator-(FLTX4 a)
//{
//	return NegSIMD(a);
//}
//
//class ALIGN16 FourVectors
//{
//public:
//	fltx4 x, y, z;
//	FourVectors(void)
//	{
//	}
//
//	FourVectors(FourVectors const& src)
//	{
//		x = src.x;
//		y = src.y;
//		z = src.z;
//	}
//
//	explicit FORCEINLINE FourVectors(float a)
//	{
//		fltx4 aReplicated = ReplicateX4(a);
//		x = y = z = aReplicated;
//	}
//
//	FORCEINLINE void Init(void)
//	{
//		x = Four_Zeros;
//		y = Four_Zeros;
//		z = Four_Zeros;
//	}
//
//	FORCEINLINE void Init(float flX, float flY, float flZ)
//	{
//		x = ReplicateX4(flX);
//		y = ReplicateX4(flY);
//		z = ReplicateX4(flZ);
//	}
//
//	FORCEINLINE FourVectors(float flX, float flY, float flZ)
//	{
//		Init(flX, flY, flZ);
//	}
//
//	FORCEINLINE void Init(fltx4 const& fl4X, fltx4 const& fl4Y, fltx4 const& fl4Z)
//	{
//		x = fl4X;
//		y = fl4Y;
//		z = fl4Z;
//	}
//
//	FORCEINLINE FourVectors(fltx4 const& fl4X, fltx4 const& fl4Y, fltx4 const& fl4Z)
//	{
//		Init(fl4X, fl4Y, fl4Z);
//	}
//
//	FORCEINLINE fltx4 const& operator[](int idx) const
//	{
//		return *((&x) + idx);
//	}
//
//	FORCEINLINE fltx4& operator[](int idx)
//	{
//		return *((&x) + idx);
//	}
//
//	FORCEINLINE void operator+=(FourVectors const& b)			//< add 4 vectors to another 4 vectors
//	{
//		x = AddSIMD(x, b.x);
//		y = AddSIMD(y, b.y);
//		z = AddSIMD(z, b.z);
//	}
//
//	FORCEINLINE void operator-=(FourVectors const& b)			//< subtract 4 vectors from another 4
//	{
//		x = SubSIMD(x, b.x);
//		y = SubSIMD(y, b.y);
//		z = SubSIMD(z, b.z);
//	}
//
//	FORCEINLINE void operator*=(FourVectors const& b)			//< scale all four vectors per component scale
//	{
//		x = MulSIMD(x, b.x);
//		y = MulSIMD(y, b.y);
//		z = MulSIMD(z, b.z);
//	}
//
//	FORCEINLINE void operator*=(const fltx4& scale)			//< scale 
//	{
//		x = MulSIMD(x, scale);
//		y = MulSIMD(y, scale);
//		z = MulSIMD(z, scale);
//	}
//
//	FORCEINLINE void operator*=(float scale)					//< uniformly scale all 4 vectors
//	{
//		fltx4 scalepacked = ReplicateX4(scale);
//		*this *= scalepacked;
//	}
//
//	FORCEINLINE fltx4 operator*(FourVectors const& b) const		//< 4 dot products
//	{
//		fltx4 dot = MulSIMD(x, b.x);
//		dot = MaddSIMD(y, b.y, dot);
//		dot = MaddSIMD(z, b.z, dot);
//		return dot;
//	}
//
//	FORCEINLINE fltx4 operator*(Vector const& b) const			//< dot product all 4 vectors with 1 vector
//	{
//		fltx4 dot = MulSIMD(x, ReplicateX4(b.x));
//		dot = MaddSIMD(y, ReplicateX4(b.y), dot);
//		dot = MaddSIMD(z, ReplicateX4(b.z), dot);
//		return dot;
//	}
//
//	FORCEINLINE FourVectors operator*(float b) const					//< scale
//	{
//		fltx4 scalepacked = ReplicateX4(b);
//		FourVectors res;
//		res.x = MulSIMD(x, scalepacked);
//		res.y = MulSIMD(y, scalepacked);
//		res.z = MulSIMD(z, scalepacked);
//		return res;
//	}
//
//	FORCEINLINE FourVectors operator*(FLTX4 fl4Scale) const					//< scale
//	{
//		FourVectors res;
//		res.x = MulSIMD(x, fl4Scale);
//		res.y = MulSIMD(y, fl4Scale);
//		res.z = MulSIMD(z, fl4Scale);
//		return res;
//	}
//
//	FORCEINLINE void operator=(FourVectors const& src)
//	{
//		x = src.x;
//		y = src.y;
//		z = src.z;
//	}
//
//};
//
//inline FourVectors operator-(const FourVectors& a, const FourVectors& b)
//{
//	FourVectors ret;
//	ret.x = SubSIMD(a.x, b.x);
//	ret.y = SubSIMD(a.y, b.y);
//	ret.z = SubSIMD(a.z, b.z);
//	return ret;
//}
//
//#define _MM_SHUFFLE(fp3,fp2,fp1,fp0) (((fp3) << 6) | ((fp2) << 4) | \
//                                     ((fp1) << 2) | ((fp0)))
//// remember, the SSE numbers its words 3 2 1 0
//// The way we want to specify shuffles is backwards from the default
//// MM_SHUFFLE_REV is in array index order (default is reversed)
//#define MM_SHUFFLE_REV(a,b,c,d) _MM_SHUFFLE(d,c,b,a)
//
//// a b c d -> b c d a
//FORCEINLINE fltx4 RotateLeft(const fltx4& a)
//{
//	return _mm_shuffle_ps(a, a, MM_SHUFFLE_REV(1, 2, 3, 0));
//}
//
//FORCEINLINE FourVectors RotateLeft(const FourVectors& src)
//{
//	FourVectors ret;
//	ret.x = RotateLeft(src.x);
//	ret.y = RotateLeft(src.y);
//	ret.z = RotateLeft(src.z);
//	return ret;
//}
//
//inline Vector CrossProduct(const Vector& a, const Vector& b)
//{
//	return Vector(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
//}
