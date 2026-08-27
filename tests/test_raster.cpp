#include "test.hpp"
#include "worldgen/raster.hpp"
WG_TEST(raster_raii_and_bounds){worldgen::RasterF a(3,4,1);a(2,3)=7;WG_CHECK(a.size()==12);WG_CHECK(a.at(2,3)==7);bool threw=false;try{(void)a.at(3,0);}catch(const std::out_of_range&){threw=true;}WG_CHECK(threw);worldgen::Tensor3F t(12,2,4);WG_CHECK(t.size()==96);}
