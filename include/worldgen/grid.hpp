#pragma once
#include "worldgen/common.hpp"
#include "worldgen/raster.hpp"
#include <cstddef>
#include <vector>
namespace worldgen {
class SphereGrid {
public:
 SphereGrid(std::size_t width,std::size_t height,double radius_km=kEarthRadiusKm);
 [[nodiscard]] std::size_t width()const noexcept{return w_;} [[nodiscard]] std::size_t height()const noexcept{return h_;} [[nodiscard]] double radius_km()const noexcept{return r_;}
 [[nodiscard]] double dlat_rad()const noexcept{return dlat_;} [[nodiscard]] double dlon_rad()const noexcept{return dlon_;} [[nodiscard]] double dy_km()const noexcept{return dy_;}
 [[nodiscard]] double lon(std::size_t x)const noexcept{return lon_[x];} [[nodiscard]] double lat(std::size_t y)const noexcept{return lat_[y];} [[nodiscard]] double dx_km(std::size_t y)const noexcept{return dx_[y];}
 [[nodiscard]] double area_weight(std::size_t y)const noexcept{return aw_[y];} [[nodiscard]] const Vec3d& xyz(std::size_t y,std::size_t x)const noexcept{return xyz_[y*w_+x];}
 [[nodiscard]] double weighted_fraction(const RasterBool&)const; [[nodiscard]] double weighted_quantile(const RasterF&,double)const; [[nodiscard]] double weighted_quantile(const RasterD&,double)const;
 [[nodiscard]] double great_circle_km(double,double,double,double)const noexcept;
private:std::size_t w_{},h_{};double r_{},dlat_{},dlon_{},dy_{};std::vector<double>lon_,lat_,dx_,aw_;std::vector<Vec3d>xyz_;
};
} // namespace worldgen
