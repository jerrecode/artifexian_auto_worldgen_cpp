#include "worldgen/grid.hpp"
#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <utility>
#include <vector>
namespace worldgen {
SphereGrid::SphereGrid(std::size_t w,std::size_t h,double r):w_(w),h_(h),r_(r),lon_(w),lat_(h),dx_(h),aw_(h),xyz_(checked_mul(w,h)){
 if(w<2||h<2||w!=2*h)throw std::invalid_argument("SphereGrid requires width == 2*height");if(!(r>0)||!std::isfinite(r))throw std::invalid_argument("invalid sphere radius");
 dlat_=kPi/static_cast<double>(h);dlon_=2*kPi/static_cast<double>(w);dy_=r*dlat_;double sum=0;
 for(std::size_t x=0;x<w;++x)lon_[x]=-180+360*static_cast<double>(x)/static_cast<double>(w);
 for(std::size_t y=0;y<h;++y){lat_[y]=90-(static_cast<double>(y)+.5)*180/static_cast<double>(h);const double lr=lat_[y]*kDegToRad,c=std::cos(lr);dx_[y]=r*dlon_*std::max(c,1e-3);aw_[y]=c;sum+=c*static_cast<double>(w);for(std::size_t x=0;x<w;++x){const double lo=lon_[x]*kDegToRad;xyz_[y*w+x]={c*std::cos(lo),c*std::sin(lo),std::sin(lr)};}}
 for(double&v:aw_)v/=sum;
}
double SphereGrid::weighted_fraction(const RasterBool&m)const{if(m.height()!=h_||m.width()!=w_)throw std::invalid_argument("shape mismatch");double s=0,corr=0;for(std::size_t y=0;y<h_;++y)for(std::size_t x=0;x<w_;++x)if(m(y,x)){const double v=aw_[y],t=s+v;corr+=(std::abs(s)>=std::abs(v))?(s-t)+v:(v-t)+s;s=t;}return s+corr;}
template<class T> static double wq(const SphereGrid&g,const Raster<T>&a,double q){if(a.height()!=g.height()||a.width()!=g.width())throw std::invalid_argument("shape mismatch");q=std::clamp(q,0.,1.);std::vector<std::pair<double,double>>v;v.reserve(a.size());for(std::size_t y=0;y<g.height();++y)for(std::size_t x=0;x<g.width();++x){double z=static_cast<double>(a(y,x));if(std::isfinite(z))v.emplace_back(z,g.area_weight(y));}if(v.empty())return std::numeric_limits<double>::quiet_NaN();std::sort(v.begin(),v.end(),[](auto&a,auto&b){return a.first<b.first;});double total=0;for(auto&p:v)total+=p.second;double target=q*total,c=0;for(auto&p:v){c+=p.second;if(c>=target)return p.first;}return v.back().first;}
double SphereGrid::weighted_quantile(const RasterF&a,double q)const{return wq(*this,a,q);}double SphereGrid::weighted_quantile(const RasterD&a,double q)const{return wq(*this,a,q);}
double SphereGrid::great_circle_km(double lat1,double lon1,double lat2,double lon2)const noexcept{const double p1=lat1*kDegToRad,p2=lat2*kDegToRad,dl=(lon2-lon1)*kDegToRad,dp=p2-p1;const double a=std::sin(dp/2)*std::sin(dp/2)+std::cos(p1)*std::cos(p2)*std::sin(dl/2)*std::sin(dl/2);return 2*r_*std::asin(std::sqrt(std::clamp(a,0.,1.)));}
} // namespace worldgen
