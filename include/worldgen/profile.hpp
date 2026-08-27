#pragma once
#include "worldgen/config.hpp"
#include "worldgen/planetary_physics.hpp"
#include "worldgen/refinement.hpp"
#include <cstdint>
#include <filesystem>
#include <map>
#include <string>
namespace worldgen {
struct PlanetProfile {
 std::uint64_t seed=20260826;
 std::size_t width=768,height=384;
 NoiseConfig noise{};
 double star_mass_solar=1.0,star_luminosity_solar=1.0,star_uv_relative_earth=1.0;
 double world_mass_earth=1.0,world_radius_earth=1.0,world_age_gyr=4.5,orbital_distance_au=1.0;
 double rotation_hours=24.0,axial_tilt_deg=23.44,bond_albedo=0.30;
 double gravity_override_m_s2=0.0;
 AtmosphereComposition atmosphere{};
 WorldOrbit orbit{};
 std::map<std::string,VolatileInventory> volatiles;
 std::string geology_regime="auto";
 double relief_scale=1.0,ice_trapping_efficiency=0.65;
 int erosion_iterations=4;
 RefinementOptions refinement{};
 [[nodiscard]] double radius_km()const noexcept{return world_radius_earth*6371.0088;}
 [[nodiscard]] double gravity_m_s2()const noexcept;
 void validate()const;
 static PlanetProfile earthlike();
 static PlanetProfile load_yaml(const std::filesystem::path&);
};
} // namespace worldgen
