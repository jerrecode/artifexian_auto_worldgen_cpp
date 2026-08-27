#pragma once
#include "worldgen/common.hpp"
#include <array>
#include <bit>
#include <cstdint>
#include <limits>
#include <string_view>
namespace worldgen {
class Rng {
public:
 explicit Rng(std::uint64_t seed=0x9e3779b97f4a7c15ULL) noexcept{std::uint64_t s=seed;for(auto&v:st_)v=splitmix64(s);if((st_[0]|st_[1]|st_[2]|st_[3])==0)st_[0]=1;}
 [[nodiscard]] std::uint64_t u64()noexcept{const auto r=std::rotl(st_[1]*5ULL,7)*9ULL;const auto t=st_[1]<<17;st_[2]^=st_[0];st_[3]^=st_[1];st_[1]^=st_[2];st_[0]^=st_[3];st_[2]^=t;st_[3]=std::rotl(st_[3],45);return r;}
 [[nodiscard]] double uniform01()noexcept{return static_cast<double>(u64()>>11)*0x1.0p-53;}
 [[nodiscard]] double uniform(double lo,double hi)noexcept{return lo+(hi-lo)*uniform01();}
 [[nodiscard]] std::uint64_t uniform_uint(std::uint64_t bound)noexcept{if(!bound)return 0;const auto threshold=-bound%bound;for(;;){const auto r=u64();if(r>=threshold)return r%bound;}}
 [[nodiscard]] bool bernoulli(double p)noexcept{return uniform01()<p;}
 [[nodiscard]] double normal(double mean=0,double sigma=1)noexcept{if(has_){has_=false;return mean+sigma*spare_;}double u1=uniform01();while(u1<=std::numeric_limits<double>::min())u1=uniform01();const double u2=uniform01(),r=std::sqrt(-2*std::log(u1)),theta=2*kPi*u2;spare_=r*std::sin(theta);has_=true;return mean+sigma*r*std::cos(theta);}
 [[nodiscard]] std::uint64_t poisson(double lambda)noexcept{if(!(lambda>0))return 0;if(lambda<30){const double limit=std::exp(-lambda);std::uint64_t k=0;double p=1;do{++k;p*=uniform01();}while(p>limit);return k-1;}for(;;){const double x=std::floor(normal(lambda,std::sqrt(lambda))+.5);if(x>=0)return static_cast<std::uint64_t>(x);}}
private:
 static std::uint64_t splitmix64(std::uint64_t&x)noexcept{std::uint64_t z=(x+=0x9e3779b97f4a7c15ULL);z=(z^(z>>30))*0xbf58476d1ce4e5b9ULL;z=(z^(z>>27))*0x94d049bb133111ebULL;return z^(z>>31);} std::array<std::uint64_t,4>st_{};bool has_{};double spare_{};
};
class RngPool { public: explicit RngPool(std::uint64_t root)noexcept:root_(root){} [[nodiscard]] Rng operator()(std::string_view stage)const noexcept{std::uint64_t h=1469598103934665603ULL^root_;for(unsigned char c:stage){h^=c;h*=1099511628211ULL;}h^=root_+0x9e3779b97f4a7c15ULL+(h<<6)+(h>>2);h^=h>>30;h*=0xbf58476d1ce4e5b9ULL;h^=h>>27;h*=0x94d049bb133111ebULL;h^=h>>31;return Rng(h);} private:std::uint64_t root_;};
} // namespace worldgen
