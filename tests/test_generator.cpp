#include "test.hpp"
#include "worldgen/generator.hpp"
#include <cmath>
WG_TEST(exoworld_generator_produces_coupled_fields){
 using namespace worldgen;
 auto p=PlanetProfile::earthlike();
 p.width=64;p.height=32;p.noise.octaves=2;p.erosion_iterations=1;p.volatiles["H2O"].total_mass_kg=2.0e19;
 const auto w=generate_planet(p);
 for(const char*name:{"elevation_km","temperature_k","surface_liquid_depth_m","surface_liquid_mask","internal_heat_flux_w_m2"}){
   const auto it=w.fields.find(name);WG_CHECK(it!=w.fields.end());WG_CHECK(it->second.height()==32);WG_CHECK(it->second.width()==64);for(float v:it->second.span())WG_CHECK(std::isfinite(v));
 }
 WG_CHECK(w.diagnostics.eroded_volume_km3>=0.0);WG_CHECK(w.diagnostics.deposited_volume_km3>=0.0);
}
WG_TEST(global_reconciliation_updates_liquid_after_refinement){
 using namespace worldgen;
 auto p=PlanetProfile::earthlike();p.width=64;p.height=32;p.noise.octaves=2;p.erosion_iterations=0;p.volatiles["H2O"].total_mass_kg=1e19;
 auto w=generate_planet(p);auto fields=w.fields;for(float&z:fields.at("elevation_km").span())z+=0.05f;VolatileCycleResult v;reconcile_global_planet_fields(p,fields,&v);WG_CHECK(fields.count("surface_liquid_depth_m")==1);WG_CHECK(v.surface_liquid.volume_m3>=0.0);
}
