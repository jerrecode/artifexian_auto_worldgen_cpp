#pragma once
#include "worldgen/field_io.hpp"
#include "worldgen/progress.hpp"
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <string>
#include <vector>
namespace worldgen {
struct RefinementOptions {
 std::size_t levels=1,scale=2,tiles_y=2,tiles_x=2,halo=12;
 bool resume=true,checkpoint=true;
 std::string kernel_fingerprint="identity-v1";
 std::filesystem::path checkpoint_dir=".worldgen-checkpoints/refine";
};
struct RefinementContext {
 std::size_t level{},tile_y{},tile_x{},tile_rows{},tile_cols{},scale{},halo{};
 std::int64_t global_y0{},global_x0{};std::size_t core_height{},core_width{};
 double cumulative_scale=1.0;std::string path;
};
using RefineKernel=std::function<void(FieldSet&,const RefinementContext&)>;
struct RefinementLevelInfo {std::size_t level{},width{},height{},tiles{};std::uint64_t input_signature{},output_signature{};};
struct RefinementResult {FieldSet fields;std::vector<RefinementLevelInfo> levels;};
class RecursiveRefiner {
public:
 explicit RecursiveRefiner(RefinementOptions options={});
 [[nodiscard]] RefinementResult run(const FieldSet& base,const RefineKernel& kernel={},ProgressTracker* progress=nullptr) const;
 [[nodiscard]] const RefinementOptions& options()const noexcept{return options_;}
private:RefinementOptions options_;
};
} // namespace worldgen
