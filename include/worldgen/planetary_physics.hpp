#pragma once
#include "worldgen/chemistry.hpp"
#include "worldgen/grid.hpp"
#include "worldgen/raster.hpp"
#include <map>
#include <string>
#include <string_view>
#include <vector>
namespace worldgen {
struct SatelliteOrbit {std::string name;double mass_earth{},radius_km{},semimajor_axis_km{},eccentricity{},k2=0.30,quality_factor=100.0;};
struct WorldOrbit {bool is_moon=false;double star_mass_solar=1.0,star_luminosity_solar=1.0,stellar_distance_au=1.0;double host_mass_earth{},host_radius_km{},host_semimajor_axis_km{},host_eccentricity{},world_k2=0.30,world_quality_factor=100.0;std::vector<SatelliteOrbit> moons;};
struct TidalHeatingResult {double host_tidal_power_w{},host_surface_flux_w_m2{},satellite_tidal_forcing_index{};std::map<std::string,double> moon_relative_tidal_forcing;};
[[nodiscard]] TidalHeatingResult tidal_heating(const WorldOrbit&,double world_mass_earth,double world_radius_km);
struct VolatileInventory {std::string formula;double total_mass_kg{};};
struct VolatilePartition {std::string formula;double atmosphere_mass_kg{},ice_mass_kg{},liquid_mass_kg{},liquid_density_kg_m3{};double gas_capacity_kg{};double liquid_stability_fraction{},ice_stability_fraction{};};
struct SurfaceLiquidResult {double level_km{},volume_m3{},mass_kg{},effective_density_kg_m3{};RasterF depth_m;std::map<std::string,double> liquid_mass_fraction;};
struct VolatileCycleResult {std::vector<VolatilePartition> species;SurfaceLiquidResult surface_liquid;std::vector<std::string> precipitating_species;std::string dominant_surface_liquid;double geomorphic_stream_power_factor=1.0,sediment_transport_factor=1.0;};
[[nodiscard]] VolatileCycleResult solve_volatile_cycle(const ChemicalDatabase&,const SphereGrid&,const RasterF& elevation_km,const RasterF& temperature_k,double total_surface_pressure_pa,double gravity_m_s2,const std::vector<VolatileInventory>&,double ice_trapping_efficiency=0.65);
enum class GeodynamicRegime { silicate_mobile_lid, silicate_stagnant_lid, heat_pipe, cryotectonic, mixed_silicate_cryo, frozen_inactive };
[[nodiscard]] std::string_view geodynamic_regime_name(GeodynamicRegime)noexcept;
struct GeodynamicResult {GeodynamicRegime regime=GeodynamicRegime::silicate_stagnant_lid;double internal_heat_flux_w_m2{},tidal_fraction{},estimated_ice_shell_km{};double cryovolcanism_index{};std::vector<std::string> plausible_cryomelts;};
[[nodiscard]] GeodynamicResult classify_geodynamics(double mass_earth,double radius_earth,double age_gyr,double surface_temperature_k,double tidal_heat_flux_w_m2,const VolatileCycleResult&);
} // namespace worldgen
