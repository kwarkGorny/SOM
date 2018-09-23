#ifndef POINT2D_H
#define POINT2D_H
#include <utility>
#include <algorithm>
#include <cmath>
template <class T>
class Point2D
{
public:

    constexpr Point2D() noexcept= default;
    constexpr Point2D(Point2D&&) noexcept= default;
    constexpr Point2D(Point2D const&) noexcept= default;
    Point2D& operator=(Point2D const& other)noexcept = default;
    Point2D& operator=(Point2D&& other) noexcept = default;

    constexpr inline Point2D(T x, T y)noexcept
        : x(std::move<T>(x)),
          y(std::move<T>(y))
    {
    }
    constexpr inline Point2D(T&& x, T&&y)noexcept
        : x(std::forward<T>(x)),
          y(std::forward<T>(y))
    {
    }
    constexpr inline Point2D operator+(const Point2D& v2) const noexcept{
        return Point2D(x + v2.x, y + v2.y);
    }
    friend inline Point2D& operator+=(Point2D& v1, const Point2D& v2)noexcept{
        v1.x += v2.x;
        v1.y += v2.y;
        return v1;
    }

    constexpr inline Point2D operator-(const Point2D& v2) const noexcept{
        return Point2D(x - v2.x, y - v2.y);
    }
    friend inline Point2D& operator-=(Point2D& v1, const Point2D& v2)noexcept{
        v1.x -= v2.x;
        v1.y -= v2.y;
        return v1;
    }
    template<class Q>
    inline Point2D operator*(Q&& scalar)noexcept{
        return Point2D(x * scalar, y * scalar);
    }
    template<class Q>
    inline Point2D& operator*=(Q&& scalar)noexcept{
        x *= scalar;
        y *= scalar;
        return *this;
    }
    template<class Q>
    constexpr inline Point2D operator/(Q&& scalar)const noexcept{
        return Point2D(x / scalar, y / scalar);
    }
    template<class Q>
    inline Point2D& operator/=(Q&& scalar)noexcept{
        x /= scalar;
        y /= scalar;
        return *this;
    }
    template<typename Q>
    constexpr inline const Q& CalcSqrDistance(Q&& _x,Q&& _y )const noexcept{
        const Q val1 = x - _x;
        const Q val2 = y - _y;
        return 
    }
    constexpr inline const T& GetMagitudeSqr()const noexcept{
        return y * y + x * x;
    }
    constexpr inline const T& GetMagitude()const noexcept{
        return std::sqrt(GetMagitudeSqr());
    }
    template<class Q>
    inline void Rotate(Q&& angle)const noexcept{
        const auto s = sin(angle);
        const auto c = cos(angle);
        *this = Point2D{x * c + y * s, x * s + y * c};
    }
    constexpr inline float Cross(Point2D const& p)const noexcept{
        return (x * p.y) - (y * p.x);
    }

    constexpr inline float Dot(Point2D const& p)const noexcept{
        return x * p.x + y * p.y;
    }

    inline void SetXY(T&& x, T&& y)noexcept{
        SetX(std::forward<T>(x));
        SetY(std::forward<T>(y));
    }

    inline void SetX(T&& x)noexcept{
        this->x = std::forward<T>(x);
    }
    constexpr inline const T& GetX()const noexcept{
        return x;
    }

    inline void SetY(T&& y) noexcept{
        this->y = std::forward<T>(y);
    }
    constexpr inline const T& GetY()const noexcept{
        return y;
    }

    T x = T();
    T y = T();
protected:


};
using Point = Point2D<double>;
using PointF = Point2D<float>;
using PointI = Point2D<int>;
using Point32I = Point2D<int32_t>;

#endif // POINT2D_H
