#pragma once
#include <map>
#include <string>
#include <string_view>
#include <vector>
namespace worldgen {
enum class Phase { solid, liquid, gas, supercritical, unknown };
[[nodiscard]] std::string_view phase_name(Phase) noexcept;
struct ChemicalSpecies {
 std::string formula,name;
 double molar_mass_g_mol{};
 double triple_temperature_k{},triple_pressure_pa{};
 double critical_temperature_k{},critical_pressure_pa{};
 double melting_temperature_k{},boiling_temperature_k{};
 double vapor_reference_temperature_k{},vapor_reference_pressure_pa{},latent_vaporization_j_mol{};
 double liquid_density_kg_m3{},liquid_viscosity_mpa_s{},surface_tension_mn_m{};
 bool atmospheric_condensable=true;
};
class ChemicalDatabase {
public:
 ChemicalDatabase();
 [[nodiscard]] const ChemicalSpecies* find(std::string_view formula)const noexcept;
 [[nodiscard]] const std::map<std::string,ChemicalSpecies>& species()const noexcept{return species_;}
private:std::map<std::string,ChemicalSpecies> species_;
};
[[nodiscard]] double saturation_vapor_pressure_pa(const ChemicalSpecies&,double temperature_k) noexcept;
[[nodiscard]] Phase phase_at(const ChemicalSpecies&,double temperature_k,double pressure_pa) noexcept;
[[nodiscard]] double liquid_density_kg_m3(const ChemicalSpecies&,double temperature_k) noexcept;
struct AtmosphereComposition { double surface_pressure_pa=101325.0;std::map<std::string,double> mole_fraction; };
struct CondensateCandidate {std::string formula;double partial_pressure_pa{},saturation_pressure_pa{},supersaturation{};Phase condensed_phase=Phase::unknown;};
[[nodiscard]] std::vector<CondensateCandidate> atmospheric_condensates(const ChemicalDatabase&,const AtmosphereComposition&,double temperature_k,double minimum_mole_fraction=1e-8);
struct PhotochemicalProduct {std::string product;double production_index{};std::string pathway;};
[[nodiscard]] std::vector<PhotochemicalProduct> photochemistry_products(const AtmosphereComposition&,double uv_flux_relative_earth);
struct GreenhouseResult {double equilibrium_temperature_k{},optical_depth{},surface_temperature_k{},greenhouse_delta_k{};std::map<std::string,double> optical_depth_by_species;};
[[nodiscard]] GreenhouseResult estimate_greenhouse(const AtmosphereComposition&,double stellar_luminosity_solar,double orbital_distance_au,double bond_albedo);
} // namespace worldgen
