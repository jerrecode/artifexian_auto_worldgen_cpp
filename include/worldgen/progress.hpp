#pragma once
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <iosfwd>
#include <map>
#include <mutex>
#include <optional>
#include <string>
namespace worldgen {
struct ProgressOptions { bool enabled=true; bool progress_bar=true; bool verbose=false; std::size_t bar_width=28; };
class ProgressTracker {
public:
 explicit ProgressTracker(ProgressOptions options={}, std::ostream* stream=nullptr);
 void begin(const std::string& path,std::uint64_t total,const std::string& description={});
 void update(const std::string& path,std::uint64_t completed,const std::string& detail={});
 void advance(const std::string& path,std::uint64_t delta=1,const std::string& detail={});
 void finish(const std::string& path,const std::string& detail={});
 void message(const std::string& path,const std::string& text);
 [[nodiscard]] std::optional<double> eta_seconds(const std::string& path) const;
private:
 using Clock=std::chrono::steady_clock;
 struct Node{std::uint64_t total{},done{};Clock::time_point start{},last{};double ema_rate{};std::string description,detail;bool finished=false;};
 void emit_locked(const std::string& path,const Node& node,bool force_line=false);
 static std::size_t depth_of(const std::string& path);
 ProgressOptions options_;std::ostream* stream_;mutable std::mutex mutex_;std::map<std::string,Node> nodes_;
};
} // namespace worldgen
