#pragma once
#include "worldgen/field_io.hpp"
#include "worldgen/planetary_physics.hpp"
#include "worldgen/profile.hpp"
#include "worldgen/progress.hpp"
#include <filesystem>
#include <map>
#include <string>
#include <vector>
namespace worldgen {
struct GenerationDiagnostics {double eroded_volume_km3{},deposited_volume_km3{},exported_sediment_volume_km3{},initial_liquid_volume_km3{},final_liquid_volume_km3{};};
struct GeneratedWorld {
 FieldSet fields;
 AtmosphereComposition atmosphere;
 GreenhouseResult greenhouse;
 TidalHeatingResult tides;
 VolatileCycleResult volatile_cycle;
 GeodynamicResult geodynamics;
 std::vector<PhotochemicalProduct> photochemistry;
 GenerationDiagnostics diagnostics;
 std::map<std::string,std::string> metadata;
};
[[nodiscard]] GeneratedWorld generate_planet(const PlanetProfile&,ProgressTracker* progress=nullptr);
void reconcile_global_planet_fields(const PlanetProfile&,FieldSet&,VolatileCycleResult* volatile_cycle=nullptr,ProgressTracker* progress=nullptr);
void write_generation_summary_json(const std::filesystem::path&,const PlanetProfile&,const GeneratedWorld&);
} // namespace worldgen
