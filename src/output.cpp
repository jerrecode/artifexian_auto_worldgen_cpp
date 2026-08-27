#include "worldgen/output.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <stdexcept>
#include <vector>
namespace worldgen {
void write_heightlevel_pgm16(const std::filesystem::path&p,const RasterF&e,bool robust){if(e.size()==0)throw std::invalid_argument("empty elevation raster");std::vector<float>finite;finite.reserve(e.size());for(float v:e.span())if(std::isfinite(v))finite.push_back(v);if(finite.empty())throw std::invalid_argument("height map has no finite values");float lo{},hi{};if(robust&&finite.size()>100){const auto a=finite.begin()+static_cast<std::ptrdiff_t>(finite.size()/100);std::nth_element(finite.begin(),a,finite.end());lo=*a;const auto b=finite.begin()+static_cast<std::ptrdiff_t>((finite.size()*99)/100);std::nth_element(finite.begin(),b,finite.end());hi=*b;}else{auto mm=std::minmax_element(finite.begin(),finite.end());lo=*mm.first;hi=*mm.second;}if(!(hi>lo))hi=lo+1;std::filesystem::create_directories(p.parent_path().empty()?std::filesystem::path("."):p.parent_path());std::ofstream o(p,std::ios::binary|std::ios::trunc);if(!o)throw std::runtime_error("cannot create heightlevel map");o<<"P5\n"<<e.width()<<' '<<e.height()<<"\n65535\n";for(float v:e.span()){const double t=std::clamp((static_cast<double>(v)-lo)/(hi-lo),0.0,1.0);const auto q=static_cast<std::uint16_t>(std::llround(t*65535.0));const unsigned char bytes[2]{static_cast<unsigned char>((q>>8)&0xff),static_cast<unsigned char>(q&0xff)};o.write(reinterpret_cast<const char*>(bytes),2);}if(!o)throw std::runtime_error("heightlevel map write failed");}
} // namespace worldgen
