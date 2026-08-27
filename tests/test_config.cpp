#include "test.hpp"
#include "worldgen/config.hpp"
#include <filesystem>
#include <fstream>
WG_TEST(config_validates_and_overrides){worldgen::WorldConfig c;c.apply_override("resolution.width=256");c.apply_override("resolution.height=128");c.apply_override("hydrology.flow_refresh_mode=adaptive");c.validate();WG_CHECK(c.resolution.width==256);WG_CHECK(c.hydrology.flow_refresh_mode=="adaptive");bool threw=false;try{c.apply_override("x.y=1");}catch(const std::invalid_argument&){threw=true;}WG_CHECK(threw);}
WG_TEST(yaml_subset_loads_nested_values){auto p=std::filesystem::temp_directory_path()/"worldgen_cfg_test.yaml";{std::ofstream o(p);o<<"seed: 7\nresolution:\n  width: 128\n  height: 64\nastronomy:\n  atmosphere:\n    N2: 0.7\n    O2: 0.3\n";}auto c=worldgen::WorldConfig::load_yaml(p);std::filesystem::remove(p);WG_CHECK(c.seed==7);WG_CHECK(c.resolution.width==128);WG_NEAR(c.astronomy.atmosphere.at("N2"),.7,1e-12);}
