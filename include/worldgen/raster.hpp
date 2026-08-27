#pragma once
#include "worldgen/common.hpp"
#include <algorithm>
#include <cassert>
#include <span>
#include <vector>
namespace worldgen {
template<class T> class Raster {
public:
    using value_type=T;
    Raster()=default;
    Raster(std::size_t h,std::size_t w):h_(h),w_(w),data_(checked_mul(h,w)){if(!h||!w)throw std::invalid_argument("Raster dimensions must be positive");}
    Raster(std::size_t h,std::size_t w,const T&v):Raster(h,w){fill(v);}
    [[nodiscard]] std::size_t height()const noexcept{return h_;} [[nodiscard]] std::size_t width()const noexcept{return w_;} [[nodiscard]] std::size_t size()const noexcept{return data_.size();} [[nodiscard]] bool empty()const noexcept{return data_.empty();}
    [[nodiscard]] T* data()noexcept{return data_.data();} [[nodiscard]] const T* data()const noexcept{return data_.data();}
    [[nodiscard]] std::span<T> span()noexcept{return data_;} [[nodiscard]] std::span<const T> span()const noexcept{return data_;}
    [[nodiscard]] T& operator()(std::size_t y,std::size_t x)noexcept{assert(y<h_&&x<w_);return data_[y*w_+x];}
    [[nodiscard]] const T& operator()(std::size_t y,std::size_t x)const noexcept{assert(y<h_&&x<w_);return data_[y*w_+x];}
    [[nodiscard]] T& at(std::size_t y,std::size_t x){if(y>=h_||x>=w_)throw std::out_of_range("Raster index");return data_[y*w_+x];}
    [[nodiscard]] const T& at(std::size_t y,std::size_t x)const{if(y>=h_||x>=w_)throw std::out_of_range("Raster index");return data_[y*w_+x];}
    [[nodiscard]] std::span<T> row(std::size_t y){if(y>=h_)throw std::out_of_range("Raster row");return {data_.data()+y*w_,w_};}
    [[nodiscard]] std::span<const T> row(std::size_t y)const{if(y>=h_)throw std::out_of_range("Raster row");return {data_.data()+y*w_,w_};}
    void fill(const T&v){std::fill(data_.begin(),data_.end(),v);}
private: std::size_t h_{},w_{};std::vector<T> data_;
};
template<class T> class Tensor3 {
public:
    Tensor3()=default; Tensor3(std::size_t p,std::size_t h,std::size_t w):p_(p),h_(h),w_(w),data_(checked_mul(p,checked_mul(h,w))){if(!p||!h||!w)throw std::invalid_argument("Tensor3 dimensions must be positive");}
    Tensor3(std::size_t p,std::size_t h,std::size_t w,const T&v):Tensor3(p,h,w){std::fill(data_.begin(),data_.end(),v);}
    [[nodiscard]] std::size_t planes()const noexcept{return p_;} [[nodiscard]] std::size_t height()const noexcept{return h_;} [[nodiscard]] std::size_t width()const noexcept{return w_;} [[nodiscard]] std::size_t size()const noexcept{return data_.size();}
    [[nodiscard]] std::span<T> span()noexcept{return data_;} [[nodiscard]] std::span<const T> span()const noexcept{return data_;}
    [[nodiscard]] T& operator()(std::size_t p,std::size_t y,std::size_t x)noexcept{assert(p<p_&&y<h_&&x<w_);return data_[(p*h_+y)*w_+x];}
    [[nodiscard]] const T& operator()(std::size_t p,std::size_t y,std::size_t x)const noexcept{assert(p<p_&&y<h_&&x<w_);return data_[(p*h_+y)*w_+x];}
    [[nodiscard]] std::span<T> plane(std::size_t p){if(p>=p_)throw std::out_of_range("Tensor plane");const auto n=checked_mul(h_,w_);return {data_.data()+p*n,n};}
    [[nodiscard]] std::span<const T> plane(std::size_t p)const{if(p>=p_)throw std::out_of_range("Tensor plane");const auto n=checked_mul(h_,w_);return {data_.data()+p*n,n};}
private:std::size_t p_{},h_{},w_{};std::vector<T> data_;
};
using RasterF=Raster<float>;using RasterD=Raster<double>;using RasterU8=Raster<std::uint8_t>;using RasterI16=Raster<std::int16_t>;using RasterI32=Raster<std::int32_t>;using RasterU32=Raster<std::uint32_t>;using RasterBool=Raster<std::uint8_t>;using Tensor3F=Tensor3<float>;
} // namespace worldgen
