#pragma once
#include "worldgen/config.hpp"
#include "worldgen/grid.hpp"
#include "worldgen/raster.hpp"
#include "worldgen/rng.hpp"
namespace worldgen {
struct NoiseBlend{double value=.44,ridge=.25,billow=.16,wave=.15;};
inline constexpr NoiseBlend kTerrainBlend{.40,.31,.14,.15};
inline constexpr NoiseBlend kTectonicBlend{.34,.24,.10,.32};
inline constexpr NoiseBlend kClimateBlend{.54,.08,.22,.16};
inline constexpr NoiseBlend kOceanBlend{.38,.32,.12,.18};
inline constexpr NoiseBlend kHydroBlend{.42,.22,.12,.24};
inline constexpr NoiseBlend kGeologyBlend{.46,.20,.20,.14};
[[nodiscard]] RasterF hybrid_multifractal(const SphereGrid&,Rng&,const NoiseConfig&,NoiseBlend blend=kTerrainBlend,int octaves_override=-1,double base_scale_px=-1,double robust_clip_sigma=3.6);
[[nodiscard]] RasterF hybrid_noise01(const SphereGrid&,Rng&,const NoiseConfig&,NoiseBlend blend=kTerrainBlend,int octaves_override=-1,double base_scale_px=-1);
struct StaticNoiseFields{RasterF ocean_fine,climate_texture,convective_texture,geology_lith,geology_igneous,hydro_wiggle,delta_texture;};
[[nodiscard]] StaticNoiseFields build_static_noise_fields(const SphereGrid&,const NoiseConfig&,const RngPool&);
} // namespace worldgen
