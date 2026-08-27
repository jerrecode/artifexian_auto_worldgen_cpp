#include "worldgen/config.hpp"
#include "worldgen/grid.hpp"
#include "worldgen/noise.hpp"
#include "worldgen/rng.hpp"
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
int main(int argc,char**argv){try{worldgen::WorldConfig cfg;std::vector<std::string>overrides;bool info=false;for(int i=1;i<argc;++i){std::string a=argv[i];if(a=="--config"&&i+1<argc)cfg=worldgen::WorldConfig::load_yaml(argv[++i]);else if(a=="--set"&&i+1<argc)overrides.emplace_back(argv[++i]);else if(a=="--runtime-info")info=true;else if(a=="--help"){std::cout<<"worldgen C++\n  --config FILE  --set section.key=value  --runtime-info\n";return 0;}else throw std::invalid_argument("unknown argument: "+a);}for(auto&s:overrides)cfg.apply_override(s);cfg.validate();worldgen::SphereGrid grid(static_cast<std::size_t>(cfg.resolution.width),static_cast<std::size_t>(cfg.resolution.height));if(info)std::cout<<"C++20 core; grid="<<grid.width()<<"x"<<grid.height()<<"; deterministic stage RNG=v1-xoshiro256ss\n";else std::cout<<"Configuration valid. C++ numerical core ready for "<<grid.width()<<"x"<<grid.height()<<" world.\n";return 0;}catch(const std::exception&e){std::cerr<<"worldgen: "<<e.what()<<'\n';return 2;}}
