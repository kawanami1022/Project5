//#include "Vector2.h"



template<class T> Vector2Template<T>::Vector2Template()
{
	x = 0;
	y = 0;
}

template<class T> Vector2Template<T>::Vector2Template(T x, T y)
{
	this->x = x;
	this->y = y;
}


template<class T> Vector2Template<T>::~Vector2Template()
{

}

template<class T>Vector2 &Vector2Template<T>::operator= (const Vector2Template<T> & vec)
{
	x = vec.x;
	y = vec.y;
	return *this;
}

template<class T>int & Vector2Template<T>::operator[](T i)
{
	if (i == 0)
	{
		return x;
	}
	else if(i == 1)
	{
		return y;
	}
	else
	{
		// ó·äOèàóù
		return x;
	}
	
}

template<class T>bool Vector2Template<T>::operator==(const Vector2Template<T> & vec) const
{
	return ((this->x == vec.x)&&(this->y == vec.y));
}

template<class T>bool Vector2Template<T>::operator!=(const Vector2Template<T> & vec) const
{
	return !((this->x == vec.x) && (this->y == vec.y));
}

template<class T>bool Vector2Template<T>::operator>(const Vector2Template<T> & vec) const
{
	return ((this->x > vec.x) && (this->x > vec.x));
}

template<class T>bool Vector2Template<T>::operator>=(const Vector2Template<T> & vec) const
{
	return ((this->x >= vec.x) && (this->x >= vec.x));
}

template<class T>bool Vector2Template<T>::operator<(const Vector2Template<T> & vec) const
{
	return ((this->x < vec.x) && (this->x < vec.x));
}

template<class T>bool Vector2Template<T>::operator<=(const Vector2Template<T> & vec) const
{
	return ((this->x <= vec.x) && (this->x <= vec.x));
}

template<class T>Vector2 & Vector2Template<T>::operator+=(const Vector2Template<T> & vec)
{
	x += vec.x;
	y += vec.y;
	return *this;
}

template<class T>Vector2 & Vector2Template<T>::operator-=(const Vector2Template<T> & vec)
{
	x -= vec.x;
	y -= vec.y;
	return *this;
}

template<class T>Vector2 & Vector2Template<T>::operator*=(T k)
{
	x *= k;
	y *= k;
	return *this;
}

template<class T>Vector2 & Vector2Template<T>::operator/=(T k)
{
	x /= k;
	y /= k;
	return *this;
}

template<class T>Vector2 Vector2Template<T>::operator+()const
{
	return *this;
}

template<class T>Vector2 Vector2Template<T>::operator-()const
{
	return Vector2Template<T>(-this->x , -this->y);
}

template<class T>Vector2 operator+(const Vector2Template<T> & u, T v)
{
	Vector2Template<T> vec;
	vec.x = u.x + v;
	vec.y = u.y + v;

	return vec;
}

template<class T>Vector2 operator-(const Vector2Template<T> & u, T v)
{
	Vector2Template<T> vec;
	vec.x = u.x - v;
	vec.y = u.y - v;

	return vec;
}

template<class T>Vector2 operator+(const Vector2Template<T> & u, const Vector2Template<T> & v)
{
	Vector2Template<T> vec;
	vec.x = u.x + v.x;
	vec.y = u.y + v.y;

	return vec;
}

template<class T>Vector2 operator-(const Vector2Template<T> & u, const Vector2Template<T> & v)
{
	Vector2Template<T> vec;
	vec.x = u.x - v.x;
	vec.y = u.y - v.y;

	return vec;
}

template<class T>Vector2 operator*(const Vector2Template<T> & u, const Vector2Template<T> & v)
{
	Vector2Template<T> vec;
	vec.x = u.x * v.x;
	vec.y = u.y * v.y;

	return vec;
}

template<class T>Vector2 operator/(const Vector2Template<T> & u, const Vector2Template<T> & v)
{
	Vector2Template<T> vec;
	vec.x = u.x / v.x;
	vec.y = u.y / v.y;

	return vec;
}

template<class T>Vector2 operator%(const Vector2Template<T> & u, const Vector2Template<T> & v)
{
	Vector2Template<T> vec;
	vec.x = u.x % v.x;
	vec.y = u.y % v.y;

	return vec;
}

template<class T>Vector2 operator%(const Vector2Template<T> & u, T v)
{
	Vector2Template<T> vec;
	vec.x = u.x % v;
	vec.y = u.y % v;

	return vec;
}

template<class T>Vector2 operator*(const Vector2Template<T> & u, T v)
{
	Vector2Template<T> vec;
	vec.x = u.x * v;
	vec.y = u.y * v;

	return vec;
}

template<class T>Vector2 operator*(T u, Vector2Template<T> & v)
{
	Vector2Template<T> vec;
	vec.x = u * v.x;
	vec.y = u * v.y;

	return vec;
}

template<class T>Vector2 operator/(const Vector2Template<T> & u, T v)
{
	Vector2Template<T> vec;
	vec.x = u.x / v;
	vec.y = u.y / v;

	return vec;
}
