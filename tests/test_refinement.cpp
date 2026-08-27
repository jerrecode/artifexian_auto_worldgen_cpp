#include "test.hpp"
#include "worldgen/field_io.hpp"
#include "worldgen/output.hpp"
#include "worldgen/refinement.hpp"
#include <cmath>
#include <filesystem>
WG_TEST(recursive_refinement_scales_and_stitches){using namespace worldgen;FieldSet f;RasterF z(8,16);for(std::size_t y=0;y<8;++y)for(std::size_t x=0;x<16;++x)z(y,x)=static_cast<float>(std::sin(0.2*static_cast<double>(x))+0.1*static_cast<double>(y));f.emplace("elevation_km",z);RefinementOptions o;o.levels=2;o.scale=2;o.tiles_y=2;o.tiles_x=2;o.halo=3;o.checkpoint=false;RecursiveRefiner r(o);auto result=r.run(f,[](FieldSet&patch,const RefinementContext&){for(auto&v:patch.at("elevation_km").span())v+=0.01f;});WG_CHECK(result.fields.at("elevation_km").height()==32);WG_CHECK(result.fields.at("elevation_km").width()==64);for(float v:result.fields.at("elevation_km").span())WG_CHECK(std::isfinite(v));WG_CHECK(result.levels.size()==2);}
WG_TEST(field_set_checkpoint_round_trip){using namespace worldgen;FieldSet f;f.emplace("a",RasterF(3,4,2.5f));const auto p=std::filesystem::temp_directory_path()/"wg-roundtrip.wgfs";save_field_set_atomic(p,f,1234567);std::uint64_t sig=0;auto g=load_field_set(p,&sig);WG_CHECK(sig==1234567);WG_CHECK(g.at("a").height()==3);WG_NEAR(g.at("a")(2,3),2.5f,1e-6f);std::filesystem::remove(p);}
WG_TEST(heightlevel_map_includes_negative_elevation){using namespace worldgen;RasterF z(2,3);z(0,0)=-8.0f;z(0,1)=-1.0f;z(0,2)=0.0f;z(1,0)=1.0f;z(1,1)=4.0f;z(1,2)=9.0f;const auto p=std::filesystem::temp_directory_path()/"wg-heightlevel.pgm";write_heightlevel_pgm16(p,z);WG_CHECK(std::filesystem::file_size(p)>20);std::filesystem::remove(p);}
