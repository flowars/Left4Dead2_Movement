#pragma once
#include "BaseTypes.h"
#include "Vector2D.h"
#include "Random.h"
#include "Platform.h"
#include <cmath>

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


