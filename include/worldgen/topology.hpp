#pragma once
#include "worldgen/grid.hpp"
#include "worldgen/raster.hpp"
#include <cstdint>
#include <utility>
#include <vector>
namespace worldgen {
struct CellIndex{std::int64_t y{},x{};};
[[nodiscard]] CellIndex map_spherical_index(std::int64_t y,std::int64_t x,std::size_t h,std::size_t w);
class SphericalRasterOps {
public:
 explicit SphericalRasterOps(const SphereGrid&g):g_(g){}
 [[nodiscard]] const SphereGrid& grid()const noexcept{return g_;}
 template<class T> [[nodiscard]] Raster<T> shift(const Raster<T>&a,int dy,int dx)const{
   check(a.height(),a.width());Raster<T>out(a.height(),a.width());for(std::size_t y=0;y<a.height();++y)for(std::size_t x=0;x<a.width();++x){auto p=map_spherical_index(static_cast<std::int64_t>(y)+dy,static_cast<std::int64_t>(x)+dx,a.height(),a.width());out(y,x)=a(static_cast<std::size_t>(p.y),static_cast<std::size_t>(p.x));}return out;
 }
 [[nodiscard]] RasterF resize_bilinear(const RasterF&,std::size_t,std::size_t)const;
 [[nodiscard]] RasterF gaussian_filter(const RasterF&,double sigma_y,double sigma_x,double truncate=4.0)const;
 [[nodiscard]] std::pair<RasterD,RasterD> metric_gradient(const RasterF&)const;
 [[nodiscard]] std::pair<RasterD,RasterD> metric_gradient(const RasterD&)const;
 [[nodiscard]] RasterD divergence(const RasterF&u,const RasterF&v)const;
 [[nodiscard]] RasterD curl(const RasterF&u,const RasterF&v)const;
 [[nodiscard]] RasterBool binary_dilation(const RasterBool&,int iterations=1)const;
 [[nodiscard]] RasterBool binary_erosion(const RasterBool&,int iterations=1)const;
 [[nodiscard]] RasterBool boundary(const RasterBool&)const;
 [[nodiscard]] std::pair<RasterI32,std::int32_t> connected_components(const RasterBool&)const;
private:
 void check(std::size_t h,std::size_t w)const{if(h!=g_.height()||w!=g_.width())throw std::invalid_argument("grid shape mismatch");}
 const SphereGrid&g_;
};
[[nodiscard]] RasterF normalize01(const RasterF&,bool robust=true);
[[nodiscard]] RasterF normalize01(const RasterD&,bool robust=true);
} // namespace worldgen
