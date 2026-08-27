#pragma once
#include "worldgen/raster.hpp"
#include <cstdint>
#include <filesystem>
#include <map>
#include <string>
namespace worldgen {
using FieldSet=std::map<std::string,RasterF>;
[[nodiscard]] std::uint64_t field_set_signature(const FieldSet&) noexcept;
void save_field_set_atomic(const std::filesystem::path&,const FieldSet&,std::uint64_t signature=0);
[[nodiscard]] FieldSet load_field_set(const std::filesystem::path&,std::uint64_t* signature=nullptr);
} // namespace worldgen
