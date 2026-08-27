#pragma once
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <stdexcept>
namespace worldgen {
constexpr double kPi = 3.141592653589793238462643383279502884;
constexpr double kDegToRad = kPi / 180.0;
constexpr double kRadToDeg = 180.0 / kPi;
constexpr double kEarthRadiusKm = 6371.0088;
[[nodiscard]] constexpr std::size_t checked_mul(std::size_t a, std::size_t b) {
    if (a != 0 && b > std::numeric_limits<std::size_t>::max()/a) throw std::overflow_error("size multiplication overflow");
    return a*b;
}
struct Vec3d { double x{},y{},z{}; constexpr Vec3d& operator+=(const Vec3d&o) noexcept{x+=o.x;y+=o.y;z+=o.z;return *this;} constexpr Vec3d& operator-=(const Vec3d&o) noexcept{x-=o.x;y-=o.y;z-=o.z;return *this;} constexpr Vec3d& operator*=(double s) noexcept{x*=s;y*=s;z*=s;return *this;} };
[[nodiscard]] constexpr Vec3d operator+(Vec3d a,const Vec3d&b) noexcept{return a+=b;}
[[nodiscard]] constexpr Vec3d operator-(Vec3d a,const Vec3d&b) noexcept{return a-=b;}
[[nodiscard]] constexpr Vec3d operator*(Vec3d a,double s) noexcept{return a*=s;}
[[nodiscard]] constexpr Vec3d operator*(double s,Vec3d a) noexcept{return a*=s;}
[[nodiscard]] constexpr double dot(const Vec3d&a,const Vec3d&b) noexcept{return a.x*b.x+a.y*b.y+a.z*b.z;}
[[nodiscard]] constexpr Vec3d cross(const Vec3d&a,const Vec3d&b) noexcept{return {a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x};}
[[nodiscard]] inline double norm(const Vec3d&a) noexcept{return std::sqrt(dot(a,a));}
[[nodiscard]] inline Vec3d normalized(const Vec3d&a){const double n=norm(a);if(!(n>1e-15)||!std::isfinite(n))throw std::domain_error("cannot normalize vector");return a*(1.0/n);}
} // namespace worldgen
