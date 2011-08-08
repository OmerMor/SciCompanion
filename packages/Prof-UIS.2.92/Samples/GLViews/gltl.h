#if (!defined __GLTL_H)
#define __GLTL_H

// disable warning C4201
#pragma warning( push )
#pragma warning( disable : 4201 )

namespace gltl
{

template < class _num >
	class ntraits;
template < class _num, class _ntr >
	class vector3;
template < class _num, class _ntr >
	class vector4;
template < class _num, class _ntr >
	class matrix;

#include <math.h>

#if (defined _DEBUG)
	#if (defined ASSERT)
		#define __gltl_assert__(__x__)	ASSERT(__x__)
	#elif (defined ATLASSERT)
		#define __gltl_assert__(__x__)	ATLASSERT(__x__)
	#else
		#include <assert.h>
		#define __gltl_assert__(__x__)	assert(__x__)
	#endif
#else
		#define __gltl_assert__(__x__)	
#endif // _DEBUG

template < class _num >
	class ntraits
{
public:

	typedef const _num _numc;
	typedef _num & _numr;
	typedef const _num & _numrc;
	typedef _num * _nump;
	typedef const _num * _numpc;

	static _num adjust_angle_d(
		_numc _angle_degrees,
		long _l_adjust_prec = 1000L
		)
	{
		long _ang = long( _angle_degrees * ((_num)(_l_adjust_prec)) );

		long _modx360 = 360L * _l_adjust_prec;
//		long _modx180 = _modx360 >> 1; // _modx360/2
		_ang %= _modx360;
		if( _ang < 0L )
			_ang = _modx360 + _ang;
		__gltl_assert__( _ang >= 0L );
//		if( _ang > _modx180 )
//			_ang = _ang%_modx180 - _modx180;
		_num _ret_val = (_num)(_ang);
		_ret_val /= (_num)(_l_adjust_prec);
		return _ret_val;
	}

	static _numc get_pi()
	{
		return ((_num)(3.1415926535897932384626433832795));
	}

	static _numc get_safe_min()
	{
		return ((_num)(0.000001));
	}

	static _numc get_def_epsilon()
	{
		return ((_num)(0.0001));
	}

	static _num r2d( _numc _angle_radians )
	{
		return
			adjust_angle_d( 
					( _angle_radians * ((_num)180.0) )
					/ get_pi()
				);
	}

	static _num d2r( _numc _angle_degrees )
	{
		return
			( adjust_angle_d(_angle_degrees) * get_pi() )
				/ ((_num)180.0)
			;
	}

	static _num abs( _numc _val )
	{
		return (_val >= (_num)(0.0)) ? _val : (-_val);
	}

	static _num sin( _numc _val )
	{
		return ( (_num) ::sin(_val) );
	}
	static _num cos( _numc _val )
	{
		return ( (_num) ::cos(_val) );
	}
	static _num tan( _numc _val )
	{
		return ( (_num) ::tan(_val) );
	}

	static _num asin( _numc _val )
	{
		return ( (_num) ::asin(_val) );
	}
	static _num acos( _numc _val )
	{
		return ( (_num) ::acos(_val) );
	}
	static _num atan( _numc _val )
	{
		return ( (_num) ::atan(_val) );
	}

	static _num atan2( _numc _y, _numc _x )
	{
		return ( (_num) ::atan2( _y, _x ) );
	}

	static _num sqrt( _numc _val )
	{
		return ( (_num) ::sqrt(_val) );
	}

};

template < class _num, class _ntr >
	class vector3
{
public:
	
	typedef const _num _numc;
	typedef _num & _numr;
	typedef const _num & _numrc;
	typedef _num * _nump;
	typedef const _num * _numpc;

	typedef vector3<_num,_ntr> _v3t;
	typedef vector3<_num,_ntr> & _v3rt;
	typedef const vector3<_num,_ntr> & _v3rct;

	typedef vector4<_num,_ntr> _v4t;
	typedef vector4<_num,_ntr> & _v4rt;
	typedef const vector4<_num,_ntr> & _v4rct;

	typedef matrix<_num,_ntr> _mt;
	typedef matrix<_num,_ntr> & _mrt;
	typedef const matrix<_num,_ntr> & _mrct;
	
	union 
	{
		struct { _num x, y, z; };
		struct { _num a, b, c; };
		_num arr[3];
	};

	vector3(
		_numc _x = ((_num)0.0),
		_numc _y = ((_num)0.0),
		_numc _z = ((_num)0.0)
		)
		: x( _x )
		, y( _y )
		, z( _z )
	{
	}

	void load_vector(
		_num _x = ((_num)0.0),
		_num _y = ((_num)0.0),
		_num _z = ((_num)0.0)
		)
	{
		x = _x;
		y = _y;
		z = _z;
	}
	
	void translate(
		_num _x = ((_num)0.0),
		_num _y = ((_num)0.0),
		_num _z = ((_num)0.0)
		)
	{
		x += _x;
		y += _y;
		z += _z;
	}

	void scale(
		_num _x = ((_num)1.0),
		_num _y = ((_num)1.0),
		_num _z = ((_num)1.0)
		)
	{
		x *= _x;
		y *= _y;
		z *= _z;
	}

	_num get_length_sq() const
	{
		return ( (_num) (x*x + y*y + z*z) );
	}

	_num get_length() const
	{
		return _ntr::sqrt( get_length_sq() );
	}

	_mt get_as_scale() const;
	_mt get_as_translation() const;
	_mt get_as_rotation_xyz() const;
	_mt get_as_rotation_quaternion() const;

	_v3rt normalize() 
	{
		_num _len = get_length();
		if( ( _len < _ntr::get_safe_min() ) && ( _len > (- _ntr::get_safe_min()) ) )
			_len = 1;
		x /= _len;
		y /= _len;
		z /= _len;
		return (*this);
	}

	static inline void stat_multiplication(
		_mrct _mtx,
		_v3rct _v,
		_v3rt _ret_val
		);
	static inline _v3t stat_multiplication(
		_mrct _mtx,
		_v3rct _v
		);

	_numr operator () ( int _i )
	{
		__gltl_assert__( ( 0 <= _i ) && ( _i <= 3 ) );
		return *(((_nump)&x) + _i);
	}
	
	_numr operator [] ( int _i ) 
	{
		__gltl_assert__( ( 0 <= _i ) && ( _i <= 2 ) );
		return *(((_nump)&x) + _i);
	}
	_numrc operator [] ( int _i ) const
	{
		__gltl_assert__( ( 0 <= _i ) && ( _i <= 2 ) );
		return *(((_nump)&x) + _i);
	}

	bool operator == ( _v3rct _v ) const
	{
		return ( x==_v.x && y==_v.y && z==_v.z ) ? true : false;
	}
	bool operator != ( _v3rct _v ) const
	{
		return ( x==_v.x && y==_v.y && z==_v.z ) ? false : true;
	}

	_v3rt operator = ( _v3rct _v )
	{
		x = _v.x;
		y = _v.y;
		z = _v.z;
		return (*this);
	}
	_v3rt operator = ( _v4rct );

	_v3rt operator *= ( _numc _s ) 
	{
		x *= _s;
		y *= _s;
		z *= _s;
		return (*this);
	}
	_v3rt operator *= ( _mrct _mtx ) 
	{
		stat_multiplication( _mtx, *this, *this );
		return (*this);
	}
	_v3rt operator += ( _v3rct _v ) 
	{
		x += _v.x;
		y += _v.y;
		z += _v.z;
		return (*this);
	}
	_v3rt operator -= ( _v3rct _v ) 
	{
		x -= _v.x;
		y -= _v.y;
		z -= _v.z;
		return (*this);
	}

};

template < class _num, class _ntr >
	inline vector3<_num,_ntr> operator * (
		const vector3<_num,_ntr> & _v,
		const _num _s
		) 
{
vector3<_num,_ntr> _ret_val;
	_ret_val.x = _v.x * _s;
	_ret_val.y = _v.y * _s;
	_ret_val.z = _v.z * _s;
	return _ret_val;
}

template < class _num, class _ntr >
	inline vector3<_num,_ntr> operator * (
		const _num _s,
		const vector3<_num,_ntr> & _v
		) 
{
vector3<_num,_ntr> _ret_val;
	_ret_val.x = _v.x * _s;
	_ret_val.y = _v.y * _s;
	_ret_val.z = _v.z * _s;
	return _ret_val;
}

template < class _num, class _ntr >
	inline _num operator * (
		const vector3<_num,_ntr> & _v1,
		const vector3<_num,_ntr> & _v2
		) 
{
	return ( _v1.x*_v2.x + _v1.y*_v2.y + _v1.z*_v2.z );
}

template < class _num, class _ntr >
	inline vector3<_num,_ntr> operator ^ (
		const vector3<_num,_ntr> & _v1,
		const vector3<_num,_ntr> & _v2
		) 
{
vector3<_num,_ntr> _ret_val;
	_ret_val.x = _v1.y*_v2.z - _v1.z*_v2.y;
	_ret_val.y = _v1.z*_v2.x - _v1.x*_v2.z;
	_ret_val.z = _v1.x*_v2.y - _v1.y*_v2.x;
	return _ret_val;
}

template < class _num, class _ntr >
	inline vector3<_num,_ntr> operator + (
		const vector3<_num,_ntr> & _v1,
		const vector3<_num,_ntr> & _v2
		) 
{
vector3<_num,_ntr> _ret_val;
	_ret_val.x = _v1.x + _v2.x;
	_ret_val.y = _v1.y + _v2.y;
	_ret_val.z = _v1.z + _v2.z;
	return _ret_val;
}

template < class _num, class _ntr >
	inline vector3<_num,_ntr> operator - (
		const vector3<_num,_ntr> & _v1,
		const vector3<_num,_ntr> & _v2
		) 
{
vector3<_num,_ntr> _ret_val;
	_ret_val.x = _v1.x - _v2.x;
	_ret_val.y = _v1.y - _v2.y;
	_ret_val.z = _v1.z - _v2.z;
	return _ret_val;
}

template < class _num, class _ntr >
	inline vector3<_num,_ntr> operator - (
		const vector3<_num,_ntr> & _v
		) 
{
vector3<_num,_ntr> _ret_val;
	_ret_val.x = - _v.x;
	_ret_val.y = - _v.y;
	_ret_val.z = - _v.z;
	return _ret_val;
}

template < class _num, class _ntr >
	inline vector3<_num,_ntr> operator + (
		const vector3<_num,_ntr> & _v
		) 
{
	return _v;
}

template < class _num, class _ntr >
	inline vector3<_num,_ntr> operator ~ (
		const vector3<_num,_ntr> & _v
		) 
{
_num _len = _v.get_length();
vector3<_num,_ntr>	_ret_val;
	_ret_val.x = _v.x / _len;
	_ret_val.y = _v.y / _len;
	_ret_val.z = _v.z / _len;
	return _ret_val;
}

template < class _num, class _ntr >
	class vector4
{
public:

	typedef const _num _numc;
	typedef _num & _numr;
	typedef const _num & _numrc;
	typedef _num * _nump;
	typedef const _num * _numpc;

	typedef vector3<_num,_ntr> _v3t;
	typedef vector3<_num,_ntr> & _v3rt;
	typedef const vector3<_num,_ntr> & _v3rct;

	typedef vector4<_num,_ntr> _v4t;
	typedef vector4<_num,_ntr> & _v4rt;
	typedef const vector4<_num,_ntr> & _v4rct;

	typedef matrix<_num,_ntr> _mt;
	typedef matrix<_num,_ntr> & _mrt;
	typedef const matrix<_num,_ntr> & _mrct;

	union 
	{
		struct { _num x, y, z, w; };
		struct { _num a, b, c, d; };
		_num arr[4];
	};

	vector4(
		_numc _x = ((_num)0.0),
		_numc _y = ((_num)0.0),
		_numc _z = ((_num)0.0),
		_numc _w = ((_num)0.0)
		)
		: x( _x )
		, y( _y )
		, z( _z )
		, w( _w )
	{
	}

	void load_vector(
		_num _x = ((_num)0.0),
		_num _y = ((_num)0.0),
		_num _z = ((_num)0.0),
		_num _w = ((_num)0.0)
		)
	{
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}

	void translate(
		_num _x = ((_num)0.0),
		_num _y = ((_num)0.0),
		_num _z = ((_num)0.0),
		_num _w = ((_num)0.0)
		)
	{
		x += _x;
		y += _y;
		z += _z;
		w += _w;
	}

	void scale(
		_num _x = ((_num)1.0),
		_num _y = ((_num)1.0),
		_num _z = ((_num)1.0),
		_num _w = ((_num)1.0)
		)
	{
		x *= _x;
		y *= _y;
		z *= _z;
		w *= _w;
	}

	_num get_length_sq() const
	{
		return ( (_num) (x*x + y*y + z*z + w*w) );
	}

	_num get_length() const
	{
		return _ntr::sqrt( get_length_sq() );
	}

	_v4rt normalize() 
	{
		_num _len = get_length();
		if( ( _len < _ntr::get_safe_min() ) && ( _len > (- _ntr::get_safe_min()) ) )
			_len = 1;
		x /= _len;
		y /= _len;
		z /= _len;
		w /= _len;
		return (*this);
	}

	static inline _v4t stat_multiplication(
		_mrct _mtx,
		_v4rct _v
		);
	static inline void stat_multiplication(
		_mrct _mtx,
		_v4rct _v,
		_v4rt _ret_val
		);

	_numr operator () ( int _i )
	{
		__gltl_assert__( ( 0 <= _i ) && ( _i <= 3 ) );
		return *(((_nump)&x) + _i);
	}
	
	_numr operator [] ( int _i ) 
	{
		__gltl_assert__( ( 0 <= _i ) && ( _i <= 3 ) );
		return *(((_nump)&x) + _i);
	}
	_numrc operator [] ( int _i ) const
	{
		__gltl_assert__( ( 0 <= _i ) && ( _i <= 3 ) );
		return *(((_nump)&x) + _i);
	}

	bool operator == ( _v4rct _v ) const
	{
		return ( x==_v.x && y==_v.y && z==_v.z && w==_v.w ) ? true : false;
	}
	bool operator != ( _v4rct _v ) const
	{
		return ( x==_v.x && y==_v.y && z==_v.z && w==_v.w ) ? false : true;
	}

	_v4rt operator = ( _v4rct _v )
	{
		x = _v.x;
		y = _v.y;
		z = _v.z;
		w = _v.w;
		return (*this);
	}
	_v4rt operator = ( _v3rct _v )
	{
		x = _v.x;
		y = _v.y;
		z = _v.z;
		w = ((_num)0.0);
		return (*this);
	}
	
	_v4rt operator *= ( _numc _s ) 
	{
		 x *= _s;
		 y *= _s;
		 z *= _s;
		 w *= _s;
		return (*this);
	}
	_v4rt operator *= ( _mrct _mtx ) 
	{
		stat_multiplication( _mtx, *this, *this );
		return (*this);
	}
	_v4rt operator += ( _v4rct _v ) 
	{
		x += _v.x;
		y += _v.y;
		z += _v.z;
		w += _v.w;
		return (*this);
	}
	_v4rt operator -= ( _v4rct _v ) 
	{
		x -= _v.x;
		y -= _v.y;
		z -= _v.z;
		w -= _v.w;
		return (*this);
	}

};

template < class _num, class _ntr >
	inline vector4<_num,_ntr> operator * (
		const vector4<_num,_ntr> & _v,
		const _num _s
		) 
{
vector4<_num,_ntr> _ret_val;
	_ret_val.x = _v.x * _s;
	_ret_val.y = _v.y * _s;
	_ret_val.z = _v.z * _s;
	_ret_val.w = _v.w * _s;
	return _ret_val;
}

template < class _num, class _ntr >
	inline vector4<_num,_ntr> operator * (
		const _num _s,
		const vector4<_num,_ntr> & _v
		) 
{
vector4<_num,_ntr> _ret_val;
	_ret_val.x = _v.x * _s;
	_ret_val.y = _v.y * _s;
	_ret_val.z = _v.z * _s;
	_ret_val.w = _v.w * _s;
	return _ret_val;
}

template < class _num, class _ntr >
	inline _num operator * (
		const vector4<_num,_ntr> & _v1,
		const vector4<_num,_ntr> & _v2
		) 
{
	return ( _v1.x*_v2.x + _v1.y*_v2.y + _v1.z*_v2.z + _v1.w*_v2.w );
}

template < class _num, class _ntr >
	inline vector4<_num,_ntr> operator + (
		const vector4<_num,_ntr> & _v1,
		const vector4<_num,_ntr> & _v2
		) 
{
vector4<_num,_ntr> _ret_val;
	_ret_val.x = _v1.x + _v2.x;
	_ret_val.y = _v1.y + _v2.y;
	_ret_val.z = _v1.z + _v2.z;
	_ret_val.w = _v1.w + _v2.w;
	return _ret_val;
}

template < class _num, class _ntr >
	inline vector4<_num,_ntr> operator - (
		const vector4<_num,_ntr> & _v1,
		const vector4<_num,_ntr> & _v2
		) 
{
vector4<_num,_ntr> _ret_val;
	_ret_val.x = _v1.x - _v2.x;
	_ret_val.y = _v1.y - _v2.y;
	_ret_val.z = _v1.z - _v2.z;
	_ret_val.w = _v1.w - _v2.w;
	return _ret_val;
}

template < class _num, class _ntr >
	inline vector4<_num,_ntr> operator - (
		const vector4<_num,_ntr> & _v
		) 
{
vector4<_num,_ntr> _ret_val;
	_ret_val.x = - _v.x;
	_ret_val.y = - _v.y;
	_ret_val.z = - _v.z;
	_ret_val.w = - _v.w;
	return _ret_val;
}

template < class _num, class _ntr >
	inline vector4<_num,_ntr> operator + (
		const vector4<_num,_ntr> & _v
		) 
{
	return _v;
}

template < class _num, class _ntr >
	inline vector4<_num,_ntr> operator ~ (
		const vector4<_num,_ntr> & _v
		) 
{
_num _len = _v.get_length();
vector4<_num,_ntr> _ret_val;
	_ret_val.x = _v.x / _len;
	_ret_val.y = _v.y / _len;
	_ret_val.z = _v.z / _len;
	_ret_val.w = _v.w / _len;
	return _ret_val;
}

template < class _num, class _ntr >
	inline vector4<_num,_ntr> operator ^ (
		const vector4<_num,_ntr> & _v1,
		const vector4<_num,_ntr> & _v2
		) 
{
vector4<_num,_ntr> _ret_val;
	_ret_val.x = _v1.y*_v2.z - _v1.z*_v2.y;
	_ret_val.y = _v1.z*_v2.x - _v1.x*_v2.z;
	_ret_val.z = _v1.x*_v2.y - _v1.y*_v2.x;
	_ret_val.w = ((_num)0.0);
	return _ret_val;
}

template < class _num, class _ntr >
	inline vector3<_num,_ntr> & vector3<_num,_ntr>::operator = (
		const vector4<_num,_ntr> & _v
		)
{
	x = _v.x;
	y = _v.y;
	z = _v.z;
	return (*this);
}

template < class _num, class _ntr >
	class quaternion : public vector4<_num,_ntr>
{
public:

	typedef quaternion<_num,_ntr> _q4t;
	typedef quaternion<_num,_ntr> & _q4rt;
	typedef const quaternion<_num,_ntr> & _q4rct;

	quaternion()
	{
		load_quaternion();
	}

	quaternion( _q4rct _q )
	{
		set_quaternion( _q );
	}

	quaternion( _numc _x, _numc _y, _numc _z )
	{
		set_quaternion( _x, _y, _z );
	}

	quaternion( _v3rct _v )
	{
		set_quaternion( _v );
	}

	quaternion( _numc _angle, _numc _x, _numc _y, _numc _z )
	{
		set_quaternion( _angle, _x, _y, _z );
	}
	
	quaternion( _numc _mtx[3][3] )
	{
		_num _tr, _s, _q[4];
		int _i, _j, _k, _nxt_val[3] = {1, 2, 0};
		_tr = _mtx[0][0] + _mtx[1][1] + _mtx[2][2];
		if( _tr > 0.0 ) 
		{
			_s = _ntr::sqrt( _tr + ((_num)1.0) );
			w  = _s / ((_num)2.0);
			_s = ((_num)0.5) / _s;
			x  = (_mtx[1][2] - _mtx[2][1]) * _s;
			y  = (_mtx[2][0] - _mtx[0][2]) * _s;
			z  = (_mtx[0][1] - _mtx[1][0]) * _s;
		} 
		else 
		{		
    		_i = 0;
			if( _mtx[1][1] > _mtx[0][0] )
				_i = 1;
			if( _mtx[2][2] > _mtx[_i][_i] )
				_i = 2;
			_j = _nxt_val[_i];
			_k = _nxt_val[_j];
			_s =
				_ntr::sqrt(
					( _mtx[_i][_i] - ( _mtx[_j][_j] + _mtx[_k][_k] ) )
					+ 1.0
					);
			_q[_i] = _s * ((_num)0.5);
			if( _s != ((_num)0.0) )
				_s = ((_num)0.5) / _s;
			_q[3]  = (_mtx[_j][_k] - _mtx[_k][_j]) * _s;
			_q[_j] = (_mtx[_i][_j] + _mtx[_j][_i]) * _s;
			_q[_k] = (_mtx[_i][_k] + _mtx[_k][_i]) * _s;
			x = _q[0];
			y = _q[1];
			z = _q[2];
			w = _q[3];
		}
	}

	quaternion( _mrt _mtx );

	void load_quaternion(
		_numc _x = ((_num)0.0),
		_numc _y = ((_num)0.0),
		_numc _z = ((_num)0.0),
		_numc _w = ((_num)1.0)
		)
	{
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}
	
	void set_quaternion( _num _angle, _num _x, _num _y, _num _z )
	{
		_numc _fct = _x*_x+_y*_y+_z*_z;
		__gltl_assert__( _fct != 0 );
		_numc _scale_by( ((_num)1.0) / _ntr::sqrt(_fct) );
		_x = _x * _scale_by;
		_y = _y * _scale_by;
		_z = _z * _scale_by;
		w = _ntr::cos( _angle / ((_num)2.0) );
		_numc _sin_half( _ntr::sin( _angle / ((_num)2.0) ) );
		x = _x * _sin_half;
		y = _y * _sin_half;
		z = _z * _sin_half;
	}
	void set_quaternion( _numc _x, _numc _y, _numc _z )
	{
		_q4t _x_q( _x, ((_num)1.0), ((_num)0.0), ((_num)0.0) );
		_q4t _y_q( _y, ((_num)0.0), ((_num)1.0), ((_num)0.0) );
		_q4t _z_q( _z, ((_num)0.0), ((_num)0.0), ((_num)1.0) );
		set_quaternion( _x_q );
		(*this) *= _y_q;
		(*this) *= _z_q;
	}
	void set_quaternion( _v3rct _v )
	{
		set_quaternion( _v.x, _v.y, _v.z );
	}
	void set_quaternion( _q4rct _q )
	{
		x = _q.x;
		y = _q.y;
		z = _q.z;
		w = _q.w;
	}

	void normalize()
	{
		_num _fct = get_length_sq();
		__gltl_assert__( _fct != 0 );
		_num _scale_by( ((_num)1.0) / _ntr::sqrt(_fct) );
		x = x * _scale_by;
		y = y * _scale_by;
		z = z * _scale_by;
		w = w * _scale_by;
	}

	void get_as_matrix( _mrt _mtx );
	_mt get_as_matrix() const;

	void get_axis_angle( _numr axis_x, _numr axis_y, _numr axis_z, _numr _rot_angle ) const
	{
		_numc _vec_len2 = x*x + y*y + z*z;
		if( _vec_len2 < _ntr::get_def_epsilon() )
		{
			axis_x = ((_num)1.0);
			axis_y = ((_num)0.0);
			axis_z = ((_num)0.0);
			_rot_angle = ((_num)0.0);
		}
		else
		{
			_numc _len_inv = ((_num)1.0) / _ntr::sqrt(_vec_len2);
			axis_x = x * _len_inv;
			axis_y = y * _len_inv;
			axis_z = z * _len_inv;
			_rot_angle = ((_num)2.0) * _ntr::acos(w);
		}
	}
	void get_axis_angle( _v3rt _v, _numr _rot_angle ) const
	{
		get_axis_angle( _v.x, _v.y, _v.z, _rot_angle );
	}

	void get_lt_ln( _numr _angle_lat, _numr _angle_long ) const
	{
		_num _x, _y, _z;
		get_direction_vector( _x, _y, _z );
		_angle_lat = _ntr::asin( _y );
		if( _ntr::abs(_y) > ( ((_num)1.0) - _ntr::get_def_epsilon() ) )
			_angle_long = ((_num)0.0);
		else
		{
			if( _ntr::abs(_z) < _ntr::get_def_epsilon() )
				_angle_long = _x > ((_num)0.0)
					? ( _ntr::get_pi() / ((_num)2.0) )
					: ( _ntr::get_pi() * ((_num)3.0) / ((_num)2.0) )
					;
			else
			{
				if( _z >= ((_num)0.0) )
					_angle_long = _ntr::atan( _x / _z );
				else
					_angle_long = _ntr::get_pi() + _ntr::atan( _x / _z );

				if( _angle_long < ((_num)0.0) )
					_angle_long += _ntr::get_pi() * ((_num)2.0);
			}
		}
	}

	void get_direction_vector( _numr _x, _numr _y, _numr _z )
	{
		normalize();
		_x = ((_num)2.0) * ( x * z - w * y );
		_y = ((_num)2.0) * ( y * z + w * x );
		_z = ((_num)1.0) - ((_num)2.0) * ( x * x + y * y );
	}
	void get_direction_vector( _v3rt _v )
	{
		get_direction_vector( _v.x, _v.y, _v.z );
	}

	static _q4t stat_multiplication( _q4rct _q1, _q4rct _q2 )
	{
		_q4t _q;
		_q.x =
			  _q2.w*_q1.x
			+ _q2.x*_q1.w
			+ _q2.y*_q1.z
			- _q2.z*_q1.y;
		_q.y =
			  _q2.w*_q1.y
			- _q2.x*_q1.z
			+ _q2.y*_q1.w
			+ _q2.z*_q1.x;
		_q.z =
			  _q2.w*_q1.z
			+ _q2.x*_q1.y
			- _q2.y*_q1.x
			+ _q2.z*_q1.w;
		_q.w =
			  _q2.w*_q1.w
			- _q2.x*_q1.x
			- _q2.y*_q1.y
			- _q2.z*_q1.z;
		return _q;
	}

	_q4rt operator *= ( _q4rct _q2 )
	{
		_q4t _holder( *this );
		(*this) = stat_multiplication( _holder, _q2 );
		return (*this);
	}

};

template < class _num, class _ntr >
	class matrix
{
public:

	typedef const _num _numc;
	typedef _num & _numr;
	typedef const _num & _numrc;
	typedef _num * _nump;
	typedef const _num * _numpc;

	typedef vector3<_num,_ntr> _v3t;
	typedef vector3<_num,_ntr> & _v3rt;
	typedef const vector3<_num,_ntr> & _v3rct;
	
	typedef vector4<_num,_ntr> _v4t;
	typedef vector4<_num,_ntr> & _v4rt;
	typedef const vector4<_num,_ntr> & _v4rct;

	typedef quaternion<_num,_ntr> _q4t;
	typedef quaternion<_num,_ntr> & _q4rt;
	typedef const quaternion<_num,_ntr> & _q4rct;

	typedef matrix<_num,_ntr> _mt;
	typedef matrix<_num,_ntr> & _mrt;
	typedef const matrix<_num,_ntr> & _mrct;

	union
	{
		struct
		{
			_num
				m11, m21, m31, m41,
				m12, m22, m32, m42,
				m13, m23, m33, m43,
				m14, m24, m34, m44;
		};
		_num arr[16];
		_num _xx[4][4];
	};

	matrix(
		_num src11 = ((_num)1.0), _num src21 = ((_num)0.0), _num src31 = ((_num)0.0), _num src41 = ((_num)0.0),
		_num src12 = ((_num)0.0), _num src22 = ((_num)1.0), _num src32 = ((_num)0.0), _num src42 = ((_num)0.0),
		_num src13 = ((_num)0.0), _num src23 = ((_num)0.0), _num src33 = ((_num)1.0), _num src43 = ((_num)0.0),
		_num src14 = ((_num)0.0), _num src24 = ((_num)0.0), _num src34 = ((_num)0.0), _num src44 = ((_num)1.0)
		)
		: m14(src14), m13(src13), m12(src12), m11(src11)
		, m24(src24), m23(src23), m22(src22), m21(src21)
		, m34(src34), m33(src33), m32(src32), m31(src31)
		, m44(src44), m43(src43), m42(src42), m41(src41)
	{
	}

	matrix( _nump p_src )
		: m14(p_src[ 3]), m13(p_src[ 2]), m12(p_src[ 1]), m11(p_src[ 0])
		, m24(p_src[ 7]), m23(p_src[ 6]), m22(p_src[ 5]), m21(p_src[ 4])
		, m34(p_src[11]), m33(p_src[10]), m32(p_src[ 9]), m31(p_src[ 8])
		, m44(p_src[15]), m43(p_src[14]), m42(p_src[13]), m41(p_src[12])
	{
	}

	matrix( _num _mtx[3][3] )
		: m14(_mtx[0][0]),  m13(_mtx[0][1]),  m12(_mtx[0][2]),  m11(((_num)0.0))
		, m24(_mtx[1][0]),  m23(_mtx[1][1]),  m22(_mtx[1][2]),  m21(((_num)0.0))
		, m34(_mtx[2][0]),  m33(_mtx[2][1]),  m32(_mtx[2][2]),  m31(((_num)0.0))
		, m44(((_num)0.0)), m43(((_num)0.0)), m42(((_num)0.0)), m41(((_num)1.0))
	{
	}

	_numr get_item_ref_xy( int _i = 0, int _j = 0 )
	{
		__gltl_assert__( (0<=_i) && (_i<=3) && (0<=_j) && (_j<=3) );
		return *(((_nump)&m11) + (_j<<2)+_i);
	}
	_numrc get_item_ref_xy( int _i = 0, int _j = 0 ) const
	{
		__gltl_assert__( (0<=_i) && (_i<=3) && (0<=_j) && (_j<=3) );
		return *(((_nump)&m11) + (_j<<2)+_i);
	}
	_nump get_item_ptr_xy( int _i = 0, int _j = 0 )
	{
		__gltl_assert__( (0<=_i) && (_i<=3) && (0<=_j) && (_j<=3) );
		return (((_nump)&m11) + (_j<<2)+_i);
	}
	_numpc get_item_ptr_xy( int _i = 0, int _j = 0 ) const
	{
		__gltl_assert__( (0<=_i) && (_i<=3) && (0<=_j) && (_j<=3) );
		return (((_nump)&m11) + (_j<<2)+_i);
	}
	_numr operator () ( int _i, int _j )
	{
		return get_item_ref_xy( _i, _j );
	}
	_numrc operator () ( int _i, int _j ) const
	{
		return get_item_ref_xy( _i, _j );
	}
	_numr get_item_ref_yx( int _i = 0, int _j = 0 )
	{
		return get_item_ref_xy( _j, _i );
	}
	_numrc get_item_ref_yx( int _i = 0, int _j = 0 ) const
	{
		return get_item_ref_xy( _j, _i );
	}
	_nump get_item_ptr_yx( int _i = 0, int _j = 0 )
	{
		return get_item_ptr_xy( _j, _i );
	}
	_numpc get_item_ptr_yx( int _i = 0, int _j = 0 ) const
	{
		return get_item_ptr_xy( _j, _i );
	}

	bool operator == ( _mrct _mtx ) const
	{
		return (
			   m11 == _mtx.m11 && m12 == _mtx.m12 && m13 == _mtx.m13 && m14 == _mtx.m14
			&& m21 == _mtx.m21 && m22 == _mtx.m22 && m23 == _mtx.m23 && m24 == _mtx.m24
			&& m31 == _mtx.m31 && m32 == _mtx.m32 && m33 == _mtx.m33 && m34 == _mtx.m34
			&& m41 == _mtx.m41 && m42 == _mtx.m42 && m43 == _mtx.m43 && m44 == _mtx.m44
			) ? true : false;
	}
	bool operator != ( _mrct _mtx ) const
	{
		return (
			   m11 == _mtx.m11 && m12 == _mtx.m12 && m13 == _mtx.m13 && m14 == _mtx.m14
			&& m21 == _mtx.m21 && m22 == _mtx.m22 && m23 == _mtx.m23 && m24 == _mtx.m24
			&& m31 == _mtx.m31 && m32 == _mtx.m32 && m33 == _mtx.m33 && m34 == _mtx.m34
			&& m41 == _mtx.m41 && m42 == _mtx.m42 && m43 == _mtx.m43 && m44 == _mtx.m44
			) ? false : true;
	}
	
	_mrt operator = ( _mrct _mtx ) 
	{
		m11 = _mtx.m11; m12 = _mtx.m12; m13 = _mtx.m13; m14 = _mtx.m14;
		m21 = _mtx.m21; m22 = _mtx.m22; m23 = _mtx.m23; m24 = _mtx.m24;
		m31 = _mtx.m31; m32 = _mtx.m32; m33 = _mtx.m33; m34 = _mtx.m34;
		m41 = _mtx.m41; m42 = _mtx.m42; m43 = _mtx.m43; m44 = _mtx.m44;
		return (*this);
	}

	static void stat_multiplication(
		_mrct _mtx1,
		_mrct _mtx2,
		_mrt _ret_val
		) 
	{
		_mt _ret_val_prep;
		_ret_val_prep.m11 = _mtx1.m11*_mtx2.m11 + _mtx1.m12*_mtx2.m21 + _mtx1.m13*_mtx2.m31 + _mtx1.m14*_mtx2.m41;
		_ret_val_prep.m12 = _mtx1.m11*_mtx2.m12 + _mtx1.m12*_mtx2.m22 + _mtx1.m13*_mtx2.m32 + _mtx1.m14*_mtx2.m42;
		_ret_val_prep.m13 = _mtx1.m11*_mtx2.m13 + _mtx1.m12*_mtx2.m23 + _mtx1.m13*_mtx2.m33 + _mtx1.m14*_mtx2.m43;
		_ret_val_prep.m14 = _mtx1.m11*_mtx2.m14 + _mtx1.m12*_mtx2.m24 + _mtx1.m13*_mtx2.m34 + _mtx1.m14*_mtx2.m44;
		_ret_val_prep.m21 = _mtx1.m21*_mtx2.m11 + _mtx1.m22*_mtx2.m21 + _mtx1.m23*_mtx2.m31 + _mtx1.m24*_mtx2.m41;
		_ret_val_prep.m22 = _mtx1.m21*_mtx2.m12 + _mtx1.m22*_mtx2.m22 + _mtx1.m23*_mtx2.m32 + _mtx1.m24*_mtx2.m42;
		_ret_val_prep.m23 = _mtx1.m21*_mtx2.m13 + _mtx1.m22*_mtx2.m23 + _mtx1.m23*_mtx2.m33 + _mtx1.m24*_mtx2.m43;
		_ret_val_prep.m24 = _mtx1.m21*_mtx2.m14 + _mtx1.m22*_mtx2.m24 + _mtx1.m23*_mtx2.m34 + _mtx1.m24*_mtx2.m44;
		_ret_val_prep.m31 = _mtx1.m31*_mtx2.m11 + _mtx1.m32*_mtx2.m21 + _mtx1.m33*_mtx2.m31 + _mtx1.m34*_mtx2.m41;
		_ret_val_prep.m32 = _mtx1.m31*_mtx2.m12 + _mtx1.m32*_mtx2.m22 + _mtx1.m33*_mtx2.m32 + _mtx1.m34*_mtx2.m42;
		_ret_val_prep.m33 = _mtx1.m31*_mtx2.m13 + _mtx1.m32*_mtx2.m23 + _mtx1.m33*_mtx2.m33 + _mtx1.m34*_mtx2.m43;
		_ret_val_prep.m34 = _mtx1.m31*_mtx2.m14 + _mtx1.m32*_mtx2.m24 + _mtx1.m33*_mtx2.m34 + _mtx1.m34*_mtx2.m44;
		_ret_val_prep.m41 = _mtx1.m41*_mtx2.m11 + _mtx1.m42*_mtx2.m21 + _mtx1.m43*_mtx2.m31 + _mtx1.m44*_mtx2.m41;
		_ret_val_prep.m42 = _mtx1.m41*_mtx2.m12 + _mtx1.m42*_mtx2.m22 + _mtx1.m43*_mtx2.m32 + _mtx1.m44*_mtx2.m42;
		_ret_val_prep.m43 = _mtx1.m41*_mtx2.m13 + _mtx1.m42*_mtx2.m23 + _mtx1.m43*_mtx2.m33 + _mtx1.m44*_mtx2.m43;
		_ret_val_prep.m44 = _mtx1.m41*_mtx2.m14 + _mtx1.m42*_mtx2.m24 + _mtx1.m43*_mtx2.m34 + _mtx1.m44*_mtx2.m44;
//		for( int _i = 0; _i < 4; _i++ )
//		{
//			for( int _j = 0; _j < 4; _j++ )
//			{
//				_ret_val_prep._xx[_i][_j] = 0;
//				for( int _k = 0; _k < 4; _k++ )
//				{
//					_ret_val_prep._xx[_i][_j] +=
//						_mtx2._xx[_i][_k] * _mtx1._xx[_k][_j];
//				}
//			}
//		}
		_ret_val = _ret_val_prep;
	}

	static _mt stat_multiplication(
		_mrct _mtx1,
		_mrct _mtx2
		) 
	{
		_mt _ret_val;
		_ret_val.m11 = _mtx1.m11*_mtx2.m11 + _mtx1.m12*_mtx2.m21 + _mtx1.m13*_mtx2.m31 + _mtx1.m14*_mtx2.m41;
		_ret_val.m12 = _mtx1.m11*_mtx2.m12 + _mtx1.m12*_mtx2.m22 + _mtx1.m13*_mtx2.m32 + _mtx1.m14*_mtx2.m42;
		_ret_val.m13 = _mtx1.m11*_mtx2.m13 + _mtx1.m12*_mtx2.m23 + _mtx1.m13*_mtx2.m33 + _mtx1.m14*_mtx2.m43;
		_ret_val.m14 = _mtx1.m11*_mtx2.m14 + _mtx1.m12*_mtx2.m24 + _mtx1.m13*_mtx2.m34 + _mtx1.m14*_mtx2.m44;
		_ret_val.m21 = _mtx1.m21*_mtx2.m11 + _mtx1.m22*_mtx2.m21 + _mtx1.m23*_mtx2.m31 + _mtx1.m24*_mtx2.m41;
		_ret_val.m22 = _mtx1.m21*_mtx2.m12 + _mtx1.m22*_mtx2.m22 + _mtx1.m23*_mtx2.m32 + _mtx1.m24*_mtx2.m42;
		_ret_val.m23 = _mtx1.m21*_mtx2.m13 + _mtx1.m22*_mtx2.m23 + _mtx1.m23*_mtx2.m33 + _mtx1.m24*_mtx2.m43;
		_ret_val.m24 = _mtx1.m21*_mtx2.m14 + _mtx1.m22*_mtx2.m24 + _mtx1.m23*_mtx2.m34 + _mtx1.m24*_mtx2.m44;
		_ret_val.m31 = _mtx1.m31*_mtx2.m11 + _mtx1.m32*_mtx2.m21 + _mtx1.m33*_mtx2.m31 + _mtx1.m34*_mtx2.m41;
		_ret_val.m32 = _mtx1.m31*_mtx2.m12 + _mtx1.m32*_mtx2.m22 + _mtx1.m33*_mtx2.m32 + _mtx1.m34*_mtx2.m42;
		_ret_val.m33 = _mtx1.m31*_mtx2.m13 + _mtx1.m32*_mtx2.m23 + _mtx1.m33*_mtx2.m33 + _mtx1.m34*_mtx2.m43;
		_ret_val.m34 = _mtx1.m31*_mtx2.m14 + _mtx1.m32*_mtx2.m24 + _mtx1.m33*_mtx2.m34 + _mtx1.m34*_mtx2.m44;
		_ret_val.m41 = _mtx1.m41*_mtx2.m11 + _mtx1.m42*_mtx2.m21 + _mtx1.m43*_mtx2.m31 + _mtx1.m44*_mtx2.m41;
		_ret_val.m42 = _mtx1.m41*_mtx2.m12 + _mtx1.m42*_mtx2.m22 + _mtx1.m43*_mtx2.m32 + _mtx1.m44*_mtx2.m42;
		_ret_val.m43 = _mtx1.m41*_mtx2.m13 + _mtx1.m42*_mtx2.m23 + _mtx1.m43*_mtx2.m33 + _mtx1.m44*_mtx2.m43;
		_ret_val.m44 = _mtx1.m41*_mtx2.m14 + _mtx1.m42*_mtx2.m24 + _mtx1.m43*_mtx2.m34 + _mtx1.m44*_mtx2.m44;
//		for( int _i = 0; _i < 4; _i++ )
//		{
//			for( int _j = 0; _j < 4; _j++ )
//			{
//				_ret_val._xx[_i][_j] = 0;
//				for( int _k = 0; _k < 4; _k++ )
//				{
//					_ret_val._xx[_i][_j] +=
//						_mtx2._xx[_i][_k] * _mtx1._xx[_k][_j];
//				}
//			}
//		}
		return _ret_val;
	}

	_mrt operator *= ( _q4rct _q ); 
	_mrt operator *= ( _mrct _mtx ) 
	{
		stat_multiplication( *this, _mtx, *this );
		return (*this);
	}
	_mrt operator *= ( _numc _v ) 
	{
		for( int _i = 0; _i < 16; _i++ )
			*(((_num*)&m11)+_i) = *(((_num*)&m11)+_i) * _v;
		return (*this);
	}
	_mrt operator += ( _mrct _mtx )
	{
		for( int _i = 0; _i < 16; _i++ )
			*(((_num*)&m11)+_i) = *(((_num*)&m11)+_i) + *(((_num*)&_mtx.m11)+_i);
		return (*this);
	}
	_mrt operator -= ( _mrct _mtx ) 
	{
		for( int _i = 0; _i < 16; _i++ )
			*(((_num*)&m11)+_i) = *(((_num*)&m11)+_i) - *(((_num*)&_mtx.m11)+_i);
		return (*this);
	}

	void load_transposition() 
	{
		_num _swap_val;
		_swap_val = m12; m12 = m21; m21 = _swap_val;
		_swap_val = m13; m13 = m31; m31 = _swap_val;
		_swap_val = m14; m14 = m41; m41 = _swap_val;
		_swap_val = m23; m23 = m32; m32 = _swap_val;
		_swap_val = m24; m24 = m42; m42 = _swap_val;
		_swap_val = m34; m34 = m43; m43 = _swap_val;
	}
	
	_num load_inversion()
	{
		_num _dest[16], _swap[12], _src[16];
		for( int _i = 0; _i < 4; _i++ ) 
		{
			_src[_i]      = *(((_nump)&m11) + (_i<<2));
			_src[_i +  4] = *(((_nump)&m11) + (_i<<2) + 1);
			_src[_i +  8] = *(((_nump)&m11) + (_i<<2) + 2);
			_src[_i + 12] = *(((_nump)&m11) + (_i<<2) + 3);
		}
		_swap[ 0] = _src[10] * _src[15];
		_swap[ 1] = _src[11] * _src[14];
		_swap[ 2] = _src[ 9] * _src[15];
		_swap[ 3] = _src[11] * _src[13];
		_swap[ 4] = _src[ 9] * _src[14];
		_swap[ 5] = _src[10] * _src[13];
		_swap[ 6] = _src[ 8] * _src[15];
		_swap[ 7] = _src[11] * _src[12];
		_swap[ 8] = _src[ 8] * _src[14];
		_swap[ 9] = _src[10] * _src[12];
		_swap[10] = _src[ 8] * _src[13];
		_swap[11] = _src[ 9] * _src[12];
		_dest[ 0] = _swap[0]*_src[5] + _swap[3]*_src[6] + _swap[ 4]*_src[7];
		_dest[ 0]-= _swap[1]*_src[5] + _swap[2]*_src[6] + _swap[ 5]*_src[7];
		_dest[ 1] = _swap[1]*_src[4] + _swap[6]*_src[6] + _swap[ 9]*_src[7];
		_dest[ 1]-= _swap[0]*_src[4] + _swap[7]*_src[6] + _swap[ 8]*_src[7];
		_dest[ 2] = _swap[2]*_src[4] + _swap[7]*_src[5] + _swap[10]*_src[7];
		_dest[ 2]-= _swap[3]*_src[4] + _swap[6]*_src[5] + _swap[11]*_src[7];
		_dest[ 3] = _swap[5]*_src[4] + _swap[8]*_src[5] + _swap[11]*_src[6];
		_dest[ 3]-= _swap[4]*_src[4] + _swap[9]*_src[5] + _swap[10]*_src[6];
		_dest[ 4] = _swap[1]*_src[1] + _swap[2]*_src[2] + _swap[ 5]*_src[3];
		_dest[ 4]-= _swap[0]*_src[1] + _swap[3]*_src[2] + _swap[ 4]*_src[3];
		_dest[ 5] = _swap[0]*_src[0] + _swap[7]*_src[2] + _swap[ 8]*_src[3];
		_dest[ 5]-= _swap[1]*_src[0] + _swap[6]*_src[2] + _swap[ 9]*_src[3];
		_dest[ 6] = _swap[3]*_src[0] + _swap[6]*_src[1] + _swap[11]*_src[3];
		_dest[ 6]-= _swap[2]*_src[0] + _swap[7]*_src[1] + _swap[10]*_src[3];
		_dest[ 7] = _swap[4]*_src[0] + _swap[9]*_src[1] + _swap[10]*_src[2];
		_dest[ 7]-= _swap[5]*_src[0] + _swap[8]*_src[1] + _swap[11]*_src[2];
		_swap[ 0] = _src[2]*_src[7];
		_swap[ 1] = _src[3]*_src[6];
		_swap[ 2] = _src[1]*_src[7];
		_swap[ 3] = _src[3]*_src[5];
		_swap[ 4] = _src[1]*_src[6];
		_swap[ 5] = _src[2]*_src[5];
		_swap[ 6] = _src[0]*_src[7];
		_swap[ 7] = _src[3]*_src[4];
		_swap[ 8] = _src[0]*_src[6];
		_swap[ 9] = _src[2]*_src[4];
		_swap[10] = _src[0]*_src[5];
		_swap[11] = _src[1]*_src[4];
		_dest[ 8] = _swap[ 0]*_src[13] + _swap[ 3]*_src[14] + _swap[ 4]*_src[15];
		_dest[ 8]-= _swap[ 1]*_src[13] + _swap[ 2]*_src[14] + _swap[ 5]*_src[15];
		_dest[ 9] = _swap[ 1]*_src[12] + _swap[ 6]*_src[14] + _swap[ 9]*_src[15];
		_dest[ 9]-= _swap[ 0]*_src[12] + _swap[ 7]*_src[14] + _swap[ 8]*_src[15];
		_dest[10] = _swap[ 2]*_src[12] + _swap[ 7]*_src[13] + _swap[10]*_src[15];
		_dest[10]-= _swap[ 3]*_src[12] + _swap[ 6]*_src[13] + _swap[11]*_src[15];
		_dest[11] = _swap[ 5]*_src[12] + _swap[ 8]*_src[13] + _swap[11]*_src[14];
		_dest[11]-= _swap[ 4]*_src[12] + _swap[ 9]*_src[13] + _swap[10]*_src[14];
		_dest[12] = _swap[ 2]*_src[10] + _swap[ 5]*_src[11] + _swap[ 1]*_src[ 9];
		_dest[12]-= _swap[ 4]*_src[11] + _swap[ 0]*_src[ 9] + _swap[ 3]*_src[10];
		_dest[13] = _swap[ 8]*_src[11] + _swap[ 0]*_src[ 8] + _swap[ 7]*_src[10];
		_dest[13]-= _swap[ 6]*_src[10] + _swap[ 9]*_src[11] + _swap[ 1]*_src[ 8];
		_dest[14] = _swap[ 6]*_src[ 9] + _swap[11]*_src[11] + _swap[ 3]*_src[ 8];
		_dest[14]-= _swap[10]*_src[11] + _swap[ 2]*_src[ 8] + _swap[ 7]*_src[ 9];
		_dest[15] = _swap[10]*_src[10] + _swap[ 4]*_src[ 8] + _swap[ 9]*_src[ 9];
		_dest[15]-= _swap[ 8]*_src[ 9] + _swap[11]*_src[10] + _swap[ 5]*_src[ 8];
		_num _det1 =
			  _src[0]*_dest[0]
			+ _src[1]*_dest[1]
			+ _src[2]*_dest[2]
			+ _src[3]*_dest[3];
		_num _det2 = ((_num)1.0) / _det1;
		for( int _j = 0; _j < 16; _j++ )
		{
			_dest[_j] *= _det2;
			*(((_nump)&m11) + _j) = _dest[_j];
		}
		return _det1;	
	}

	_num get_determinant() 
	{
		_num _src[16], _swap[12], _dest[4];
		for( int _i = 0; _i < 16; _i++ )
			_src[_i] = *(((_nump)&m11) + _i);
		_swap[ 0] = _src[10] * _src[15];
		_swap[ 1] = _src[11] * _src[14];
		_swap[ 2] = _src[ 9] * _src[15];
		_swap[ 3] = _src[11] * _src[13];
		_swap[ 4] = _src[ 9] * _src[14];
		_swap[ 5] = _src[10] * _src[13];
		_swap[ 6] = _src[ 8] * _src[15];
		_swap[ 7] = _src[11] * _src[12];
		_swap[ 8] = _src[ 8] * _src[14];
		_swap[ 9] = _src[10] * _src[12];
		_swap[10] = _src[ 8] * _src[13];
		_swap[11] = _src[ 9] * _src[12];
		_dest[ 0] = _swap[0]*_src[5] + _swap[3]*_src[6] + _swap[ 4]*_src[7];
		_dest[ 0]-= _swap[1]*_src[5] + _swap[2]*_src[6] + _swap[ 5]*_src[7];
		_dest[ 1] = _swap[1]*_src[4] + _swap[6]*_src[6] + _swap[ 9]*_src[7];
		_dest[ 1]-= _swap[0]*_src[4] + _swap[7]*_src[6] + _swap[ 8]*_src[7];
		_dest[ 2] = _swap[2]*_src[4] + _swap[7]*_src[5] + _swap[10]*_src[7];
		_dest[ 2]-= _swap[3]*_src[4] + _swap[6]*_src[5] + _swap[11]*_src[7];
		_dest[ 3] = _swap[5]*_src[4] + _swap[8]*_src[5] + _swap[11]*_src[6];
		_dest[ 3]-= _swap[4]*_src[4] + _swap[9]*_src[5] + _swap[10]*_src[6];
		_num _det =
			  _src[0]*_dest[0]
			+ _src[1]*_dest[1]
			+ _src[2]*_dest[2]
			+ _src[3]*_dest[3];
		return _det;
	}

	_v3t get_angles()
	{
		_num yy, xx, zz, _k, rd;
		rd = _ntr::sqrt( m13*m13 + m23*m23 + m33*m33 );
		_k = ( ((_num)1.0) - _ntr::get_safe_min() ) * rd;
		if( m23 > _k )
		{
			zz = ((_num)0.0);
			xx = - _ntr::get_pi() / ((_num)2.0);
			yy = _ntr::atan2( -m12, m11 );
		}
		else if( m23 < (-_k) )
		{
			zz = ((_num)0.0);
			xx = _ntr::get_pi() / ((_num)2.0);
			yy = _ntr::atan2( m12, m11 );
		}
		else
		{
			if( rd == 0)
				yy = xx = zz = ((_num)0.0);
			else
			{
				xx =  - _ntr::asin( m23 / rd );
				yy =    _ntr::atan2( m13, m33 );
				zz =    _ntr::atan2( m21, m22 );
			}
		}
		return _v3t( xx, yy, zz );
	}

	_v3t get_position()
	{
		return _v3t( m14, m24, m34 );
	}
	
	void load_zero()
	{
		  m11 = m12 = m13 = m14
		= m21 = m22 = m23 = m24
		= m31 = m32 = m33 = m34
		= m41 = m42 = m43 = m44
		= ((_num)0.0);
	}

	void load_identity()
	{
		load_zero();
		m11 = m22 = m33 = m44 = ((_num)1.0);
	}

	void load_translation(
		_numc dx,
		_numc dy,
		_numc dz
		)
	{
		load_identity();
		m14 = dx;
		m24 = dy;
		m34 = dz;
	}
	void load_translation( _v3rct _v )
	{
		load_identity();
		m14 = _v.x;
		m24 = _v.y;
		m34 = _v.z;
	}

	void load_rotation_x( _numc _angle_radians )
	{
		_num _c = _ntr::cos(_angle_radians), _s = _ntr::sin(_angle_radians);
		load_identity();
		m22 = + _c;
		m23 = - _s;
		m32 = + _s;
		m33 = + _c;
	}
	void load_rotation_y( _numc _angle_radians )
	{
		_num _c = _ntr::cos(_angle_radians), _s = _ntr::sin(_angle_radians);
		load_identity();
		m11 = + _c;
		m13 = + _s;
		m31 = - _s;
		m33 = + _c;
	}
	void load_rotation_z( _numc _angle_radians )
	{
		_num _c = _ntr::cos(_angle_radians), _s = _ntr::sin(_angle_radians);
		load_identity();
		m11 = + _c;
		m12 = - _s;
		m21 = + _s;
		m22 = + _c;
	}
	void load_rotation_xyz( _numc _x, _numc _y, _numc _z )
	{
		_mt mtx_rotation_x, mtx_rotation_y, mtx_rotation_z;
		mtx_rotation_x.load_rotation_x(_x);
		mtx_rotation_y.load_rotation_y(_y);
		mtx_rotation_z.load_rotation_z(_z);
		(*this) = mtx_rotation_x * mtx_rotation_y * mtx_rotation_z;
	}
	void load_rotation_xyz( _v3rct _v )
	{
		load_rotation_xyz( _v.x, _v.y, _v.z );
	}

	void load_scale( _numc _x, _numc _y, _numc _z )
	{
		load_identity();
		m11 = _x;
		m22 = _y;
		m33 = _z;
	} 
	void load_scale( _v3rct _v )
	{
		load_identity();
		m11 = _v.x;
		m22 = _v.y;
		m33 = _v.z;
	} 
	void load_scale( _numc _val )
	{
		load_identity();
		m11 = m22 = m33 = _val;
	}

	void load_perspective(
		_num fovy,
		_num aspect,
		_num znear,
		_num zfar
		)
	{
		load_identity();
		_num _ctan_fov =
			((_num)1.0)
			/
			( _ntr::tan( fovy / ((_num)2.0)) );
		m11 =   _ctan_fov / aspect;
		m22 =   _ctan_fov;
		m33 = - (zfar + znear) / (zfar - znear);
		m34 = - ((_num)2.0) * (zfar * znear) / ( zfar - znear );
		m43 = - ((_num)1.0);
		m44 =   ((_num)0.0);
	}

	void load_rotation_about_vector(
		_numc _angle,
		_v3rct _vec
		)
	{
		_v3t _v( _vec );
		_v.normalize();
		_num _x = _v._x, _y = _v._y, _z = _v._z;
		_num _c = _ntr::cos(_angle);
		_num _s = _ntr::sin(_angle);
		_num xx = _x * _x;
		_num xy = _y * _x;
		_num xz = _z * _x;
		_num yy = _y * _y;
		_num yz = _y * _z;
		_num zz = _z * _z;
		_num xs = _x * _s;
		_num ys = _y * _s;
		_num zs = _z * _s;
		_num _1c = 1 - _c;
		m11 = xx*_1c+_c;
		m12 = xy*_1c+zs;
		m13 = xz*_1c-ys;
		m14 = 0;
		m21 = xy*_1c-zs;
		m22 = yy*_1c+_c;
		m23 = yz*_1c+xs;
		m24 = 0;
		m31 = xz*_1c+ys;
		m32 = yz*_1c-xs;
		m33 = zz*_1c+_c;
		m34 = 0;
		m41 = 0;
		m42 = 0;
		m43 = 0;
		m44 = 1;
	}
	void load_rotation_about_vector(
		_numc _angle,
		_numc _x,
		_numc _y,
		_numc _z
		)
	{
		load_rotation_about_vector(
			_angle,
			_v3t( _x, _y, _z )
			);
	}
	void load_rotation_about_vector(
		_numc _angle,
		_v4rct _vec
		)
	{
		load_rotation_about_vector(
			_angle,
			_v3t(_vec.x,_vec.y,_vec.z)
			);
	}

	void load_reflection(
		_numc _a,
		_numc _b,
		_numc _c,
		_numc _d
		)
	{
		get_item_ref_yx( 0, 0 ) = _numc(1.0) - _numc(2.0) * _a * _a;
		get_item_ref_yx( 1, 0 ) =            - _numc(2.0) * _a * _b;
		get_item_ref_yx( 2, 0 ) =            - _numc(2.0) * _a * _c;
		get_item_ref_yx( 3, 0 ) =            - _numc(2.0) * _d * _a;
		get_item_ref_yx( 0, 1 ) =            - _numc(2.0) * _b * _a;
		get_item_ref_yx( 1, 1 ) = _numc(1.0) - _numc(2.0) * _b * _b;
		get_item_ref_yx( 2, 1 ) =            - _numc(2.0) * _b * _c;
		get_item_ref_yx( 3, 1 ) =            - _numc(2.0) * _d * _b;
		get_item_ref_yx( 0, 2 ) =            - _numc(2.0) * _c * _a;
		get_item_ref_yx( 1, 2 ) =            - _numc(2.0) * _c * _b;
		get_item_ref_yx( 2, 2 ) = _numc(1.0) - _numc(2.0) * _c * _c;
		get_item_ref_yx( 3, 2 ) =            - _numc(2.0) * _d * _c;
		get_item_ref_yx( 0, 3 ) = _numc(0.0);
		get_item_ref_yx( 1, 3 ) = _numc(0.0);
		get_item_ref_yx( 2, 3 ) = _numc(0.0);
		get_item_ref_yx( 3, 3 ) = _numc(1.0);
	}
	void load_reflection( _v4rct plane )
	{
		load_reflection( plane.a, plane.b, plane.c, plane.d );
	}

};

template < class _num, class _ntr >
	inline matrix<_num,_ntr> operator * (
		const matrix<_num,_ntr> &_mtx,
		const _num _v
		) 
{
matrix<_num,_ntr> _ret_val;
	for( int _i = 0; _i < 16; _i++ )
		*(((_num*)&_ret_val.m11)+_i) = *(((_num*)&_mtx.m11)+_i) * _v;
	return _ret_val;
}

template < class _num, class _ntr >
	inline matrix<_num,_ntr> operator * (
		const _num _v,
		const matrix<_num,_ntr> & _mtx
		) 
{
matrix<_num,_ntr> _ret_val;
	for( int _i = 0; _i < 16; _i++ )
		*(((_num*)&_ret_val.m11)+_i) = *(((_num*)&_mtx.m11)+_i) * _v;
	return _ret_val;
}

template < class _num, class _ntr >
	inline matrix<_num,_ntr> operator * (
		const matrix<_num,_ntr> & _mtx1,
		const matrix<_num,_ntr> & _mtx2
		) 
{
	return matrix<_num,_ntr>::stat_multiplication( _mtx1, _mtx2 );
}

template < class _num, class _ntr >
	inline matrix<_num,_ntr> operator + (
		const matrix<_num,_ntr> & _mtx1,
		const matrix<_num,_ntr> & _mtx2
		) 
{
matrix<_num,_ntr> _ret_val;
	for( int _i = 0; _i < 16; _i++ )
		*(((_num*)&_ret_val.m11)+_i) = *(((_num*)&_mtx1.m11)+_i) + *(((_num*)&_mtx2.m11)+_i);
	return _ret_val;
}

template < class _num, class _ntr >
	inline matrix<_num,_ntr> operator - (
		const matrix<_num,_ntr> & _mtx1,
		const matrix<_num,_ntr> & _mtx2
		)
{
matrix<_num,_ntr> _ret_val;
	for( int _i = 0; _i < 16; _i++ )
		*(((_num*)&_ret_val.m11)+_i) = *(((_num*)&_mtx1.m11)+_i) - *(((_num*)&_mtx2.m11)+_i);
	return _ret_val;
}

template < class _num, class _ntr >
	inline matrix<_num,_ntr> operator - ( const matrix<_num,_ntr> & _mtx ) 
{
matrix<_num,_ntr> _ret_val;
	for( int _i = 0; _i < 16; _i++ )
		*(((_num*)&_ret_val.m11)+_i) = - *(((_num*)&_mtx.m11)+_i);
	return _ret_val;
}

template < class _num, class _ntr >
	inline matrix<_num,_ntr> operator + ( const matrix<_num,_ntr> & _mtx ) 
{
	return _mtx;
}

template < class _num, class _ntr >
	void inline vector3<_num,_ntr>::stat_multiplication(
		const matrix<_num,_ntr> & _mtx,
		const vector3<_num,_ntr> & _v,
		vector3<_num,_ntr> & _ret_val
		)
{
vector3<_num,_ntr> _ret_val_prep;
	_ret_val_prep.x = _mtx.m11*_v.x + _mtx.m12*_v.y + _mtx.m13*_v.z + _mtx.m14;
	_ret_val_prep.y = _mtx.m21*_v.x + _mtx.m22*_v.y + _mtx.m23*_v.z + _mtx.m24;
	_ret_val_prep.z = _mtx.m31*_v.x + _mtx.m32*_v.y + _mtx.m33*_v.z + _mtx.m34;
	_ret_val.x = _ret_val_prep.x;
	_ret_val.y = _ret_val_prep.y;
	_ret_val.z = _ret_val_prep.z;
}

template < class _num, class _ntr >
	inline vector3<_num,_ntr> vector3<_num,_ntr>::stat_multiplication(
		const matrix<_num,_ntr> & _mtx,
		const vector3<_num,_ntr> & _v
		)
{
vector3<_num,_ntr> _ret_val;
	_ret_val.x = _mtx.m11*_v.x + _mtx.m12*_v.y + _mtx.m13*_v.z + _mtx.m14;
	_ret_val.y = _mtx.m21*_v.x + _mtx.m22*_v.y + _mtx.m23*_v.z + _mtx.m24;
	_ret_val.z = _mtx.m31*_v.x + _mtx.m32*_v.y + _mtx.m33*_v.z + _mtx.m34;
	return _ret_val;
}

template < class _num, class _ntr >
	inline vector3<_num,_ntr> operator * (
		const matrix<_num,_ntr> & _mtx,
		const vector3<_num,_ntr> & _v
		)
{
	return vector3<_num,_ntr>::stat_multiplication( _mtx, _v );
}

template < class _num, class _ntr >
	inline void vector4<_num,_ntr>::stat_multiplication(
		const matrix<_num,_ntr> & _mtx,
		_v4rct _v,
		_v4rt _ret_val
		)
{
vector4<_num,_ntr> _ret_val_prep;
	_ret_val_prep.x = _mtx.m11*_v.x + _mtx.m12*_v.y + _mtx.m13*_v.z + _mtx.m14*_v.w;
	_ret_val_prep.y = _mtx.m21*_v.x + _mtx.m22*_v.y + _mtx.m23*_v.z + _mtx.m24*_v.w;
	_ret_val_prep.z = _mtx.m31*_v.x + _mtx.m32*_v.y + _mtx.m33*_v.z + _mtx.m34*_v.w;
	_ret_val_prep.w = _mtx.m41*_v.x + _mtx.m42*_v.y + _mtx.m43*_v.z + _mtx.m44*_v.w;
	_ret_val.x = _ret_val_prep.x;
	_ret_val.y = _ret_val_prep.y;
	_ret_val.z = _ret_val_prep.z;
	_ret_val.w = _ret_val_prep.w;
}

template < class _num, class _ntr >
	inline vector4<_num,_ntr> vector4<_num,_ntr>::stat_multiplication(
		const matrix<_num,_ntr> & _mtx,
		_v4rct _v
		)
{
vector4<_num,_ntr> _ret_val;
	_ret_val.x = _mtx.m11*_v.x + _mtx.m12*_v.y + _mtx.m13*_v.z + _mtx.m14*_v.w;
	_ret_val.y = _mtx.m21*_v.x + _mtx.m22*_v.y + _mtx.m23*_v.z + _mtx.m24*_v.w;
	_ret_val.z = _mtx.m31*_v.x + _mtx.m32*_v.y + _mtx.m33*_v.z + _mtx.m34*_v.w;
	_ret_val.w = _mtx.m41*_v.x + _mtx.m42*_v.y + _mtx.m43*_v.z + _mtx.m44*_v.w;
	return _ret_val;
}

template < class _num, class _ntr >
	inline vector4<_num,_ntr> operator * (
		const matrix<_num,_ntr> & _mtx,
		const vector4<_num,_ntr> & _v
		)
{
	return vector4<_num,_ntr>::stat_multiplication( _mtx, _v );
}

template < class _num, class _ntr >
	inline quaternion<_num,_ntr>::quaternion(
		matrix<_num,_ntr> & _mtx
		)
{
_num _tr, _s, _q[4];
int _i, _j, _k;
int _nxt_val[3] = { 1, 2, 0 };
	_tr = _mtx.m11 + _mtx.m22 + _mtx.m33;
	if( _tr > ((_num)0.0) ) 
	{
		_s = _ntr::sqrt( _tr + ((_num)1.0) );
		w  = _s / ((_num)2.0);
		_s = ((_num)0.5) / _s;
		x  = (_mtx.m23 - _mtx.m32) * _s;
		y  = (_mtx.m31 - _mtx.m13) * _s;
		z  = (_mtx.m12 - _mtx.m21) * _s;
	} 
	else 
	{		
    	_i = 0;
		if( _mtx.m22 > _mtx.m11 )
			_i = 1;
		if( _mtx.m33 > _mtx.arr[4*_i+_i] )
			_i = 2;
		_j = _nxt_val[_i];
		_k = _nxt_val[_j];
		_s =
			_ntr::sqrt(
				( _mtx.arr[4*_i+_i] -
					( _mtx.arr[4*_j+_j] + _mtx.arr[4*_k+_k] )
				) + ((_num)1.0)
				);
		_q[_i] = _s * ((_num)0.5);
		if( _s != ((_num)0.0) )
			_s = ((_num)0.5) / _s;
		_q[3]  = (_mtx.arr[4*_j+_k] - _mtx.arr[4*_k+_j] ) * _s;
		_q[_j] = (_mtx.arr[4*_i+_j] + _mtx.arr[4*_j+_i] ) * _s;
		_q[_k] = (_mtx.arr[4*_i+_k] + _mtx.arr[4*_k+_i] ) * _s;
		x = _q[0];
		y = _q[1];
		z = _q[2];
		w = _q[3];
	}
}

template < class _num, class _ntr >
	inline void quaternion<_num,_ntr>::get_as_matrix(
		matrix<_num,_ntr> & _mtx
		)
{
	normalize();
_num xx = x * x;
_num yy = y * y;
_num zz = z * z;
	_mtx.get_item_ref_yx( 0, 0 ) = ((_num)1.0) - ((_num)2.0) * ( yy + zz );
	_mtx.get_item_ref_yx( 1, 0 ) = ((_num)2.0) * ( x * y + w * z );
	_mtx.get_item_ref_yx( 2, 0 ) = ((_num)2.0) * ( x * z - w * y );
	_mtx.get_item_ref_yx( 3, 0 ) = ((_num)0.0);
	_mtx.get_item_ref_yx( 0, 1 ) = ((_num)2.0) * ( x * y - w * z );
	_mtx.get_item_ref_yx( 1, 1 ) = ((_num)1.0) - ((_num)2.0) * ( xx + zz );
	_mtx.get_item_ref_yx( 2, 1 ) = ((_num)2.0) * ( y * z + w * x );
	_mtx.get_item_ref_yx( 3, 1 ) = ((_num)0.0);
	_mtx.get_item_ref_yx( 0, 2 ) = ((_num)2.0) * ( x * z + w * y );
	_mtx.get_item_ref_yx( 1, 2 ) = ((_num)2.0) * ( y * z - w * x );
	_mtx.get_item_ref_yx( 2, 2 ) = ((_num)1.0) - ((_num)2.0) * ( xx + yy );
	_mtx.get_item_ref_yx( 3, 2 ) = ((_num)0.0);
	_mtx.get_item_ref_yx( 0, 3 ) = ((_num)0.0);
	_mtx.get_item_ref_yx( 1, 3 ) = ((_num)0.0);
	_mtx.get_item_ref_yx( 2, 3 ) = ((_num)0.0);
	_mtx.get_item_ref_yx( 3, 3 ) = ((_num)1.0);
}

template < class _num, class _ntr >
	inline matrix<_num,_ntr> quaternion<_num,_ntr>::
		get_as_matrix() const
{
_q4t _q = (*this);
_mt _mtx;
	_q.get_as_matrix( _mtx );
	return _mtx;
}

template < class _num, class _ntr >
	inline matrix<_num,_ntr> &
		matrix<_num,_ntr>::operator *= (
			const quaternion<_num,_ntr> & _q
			)
{
	(*this) *= _q.get_as_matrix();
	return (*this);
}

template < class _num, class _ntr >
	inline matrix<_num,_ntr> operator * (
		const matrix<_num,_ntr> & _mtx,
		const quaternion<_num,_ntr> & _q
		)
{
matrix<_num,_ntr> _ret_val = _mtx;
	_ret_val *= _q;
	return _ret_val;
}

template < class _num, class _ntr >
	inline matrix<_num,_ntr>
		vector3<_num,_ntr>::get_as_scale() const
{
_mt _mtx;
	_mtx.load_scale( *this );
	return _mtx;
}

template < class _num, class _ntr >
	inline matrix<_num,_ntr>
		vector3<_num,_ntr>::get_as_translation() const
{
_mt _mtx;
	_mtx.load_translation( *this );
	return _mtx;
}

template < class _num, class _ntr >
	inline matrix<_num,_ntr>
		vector3<_num,_ntr>::get_as_rotation_xyz() const
{
_mt _mtx;
	_mtx.load_rotation_xyz( *this );
	return _mtx;
}

template < class _num, class _ntr >
	inline matrix<_num,_ntr>
		vector3<_num,_ntr>::get_as_rotation_quaternion() const
{
quaternion<_num,_ntr> _q( *this );
	return _q.get_as_matrix();
}

}; // namespace gltl

// rollback warning C4201
#pragma warning( pop )

#endif // __GLTL_H



