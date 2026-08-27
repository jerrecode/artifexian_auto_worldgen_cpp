#include "worldgen/config.hpp"
#include "worldgen/field_io.hpp"
#include "worldgen/generator.hpp"
#include "worldgen/grid.hpp"
#include "worldgen/output.hpp"
#include "worldgen/profile.hpp"
#include "worldgen/progress.hpp"
#include <filesystem>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

namespace {
void print_help(){
 std::cout
  << "worldgen C++\n"
  << "  --generate PROFILE   Generate an exoworld from a planet-profile YAML\n"
  << "  --out DIR            Output directory for generation (default: world-output)\n"
  << "  --no-progress        Disable progress reporting\n"
  << "  --no-progress-bar    Use line-oriented progress instead of an updating bar\n"
  << "  --verbose            Enable detailed progress logging\n"
  << "  --config FILE        Validate legacy world configuration YAML\n"
  << "  --set path=value     Override legacy configuration value\n"
  << "  --runtime-info       Print runtime/core information\n";
}
}

int main(int argc,char**argv){
 try{
  worldgen::WorldConfig cfg;
  std::vector<std::string>overrides;
  std::optional<std::filesystem::path>profile_path;
  std::filesystem::path out_dir="world-output";
  bool info=false,progress_enabled=true,progress_bar=true,verbose=false;
  for(int i=1;i<argc;++i){
   const std::string a=argv[i];
   if(a=="--config"&&i+1<argc)cfg=worldgen::WorldConfig::load_yaml(argv[++i]);
   else if(a=="--set"&&i+1<argc)overrides.emplace_back(argv[++i]);
   else if(a=="--generate"&&i+1<argc)profile_path=std::filesystem::path(argv[++i]);
   else if(a=="--out"&&i+1<argc)out_dir=std::filesystem::path(argv[++i]);
   else if(a=="--no-progress")progress_enabled=false;
   else if(a=="--no-progress-bar")progress_bar=false;
   else if(a=="--verbose")verbose=true;
   else if(a=="--runtime-info")info=true;
   else if(a=="--help"||a=="-h"){print_help();return 0;}
   else throw std::invalid_argument("unknown or incomplete argument: "+a);
  }
  if(profile_path){
   const auto profile=worldgen::PlanetProfile::load_yaml(*profile_path);
   worldgen::ProgressTracker tracker({progress_enabled,progress_bar,verbose},&std::cerr);
   auto world=worldgen::generate_planet(profile,progress_enabled?&tracker:nullptr);
   std::filesystem::create_directories(out_dir);
   const auto signature=worldgen::field_set_signature(world.fields);
   worldgen::save_field_set_atomic(out_dir/"fields.wgf",world.fields,signature);
   worldgen::write_heightlevel_pgm16(out_dir/"heightlevel.pgm",world.fields.at("elevation_km"),false);
   worldgen::write_generation_summary_json(out_dir/"summary.json",profile,world);
   std::cout
    << "generated="<<profile_path->string()<<'\n'
    << "resolution="<<profile.width<<'x'<<profile.height<<'\n'
    << "surface_temperature_k="<<world.greenhouse.surface_temperature_k<<'\n'
    << "greenhouse_delta_k="<<world.greenhouse.greenhouse_delta_k<<'\n'
    << "tidal_heat_flux_w_m2="<<world.tides.host_surface_flux_w_m2<<'\n'
    << "geodynamic_regime="<<world.metadata.at("geodynamic_regime")<<'\n'
    << "dominant_surface_liquid="<<world.metadata.at("dominant_surface_liquid")<<'\n'
    << "surface_liquid_level_km="<<world.volatile_cycle.surface_liquid.level_km<<'\n'
    << "surface_liquid_volume_km3="<<world.volatile_cycle.surface_liquid.volume_m3/1e9<<'\n'
    << "fields="<<world.fields.size()<<'\n'
    << "field_signature="<<signature<<'\n'
    << "output_dir="<<out_dir.string()<<'\n';
   return 0;
  }
  for(const auto&s:overrides)cfg.apply_override(s);
  cfg.validate();
  worldgen::SphereGrid grid(static_cast<std::size_t>(cfg.resolution.width),static_cast<std::size_t>(cfg.resolution.height));
  if(info)std::cout<<"C++20 core; grid="<<grid.width()<<"x"<<grid.height()<<"; deterministic stage RNG=v1-xoshiro256ss\n";
  else std::cout<<"Configuration valid. C++ numerical core ready for "<<grid.width()<<"x"<<grid.height()<<" world. Use --generate PROFILE to produce a world.\n";
  return 0;
 }catch(const std::exception&e){std::cerr<<"worldgen: "<<e.what()<<'\n';return 2;}
}
