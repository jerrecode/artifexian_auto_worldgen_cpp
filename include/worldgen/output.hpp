#pragma once
#include "worldgen/raster.hpp"
#include <filesystem>
namespace worldgen {
void write_heightlevel_pgm16(const std::filesystem::path&,const RasterF& elevation_km,bool robust=false);
} // namespace worldgen
