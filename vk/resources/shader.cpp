#include "../internal.hpp"
#include <fstream>
using vk::internal::require_device;
using vk::internal::require_instance;
using vk::internal::require_physical;
namespace vk {
namespace shader {
std::vector<uint32_t> read(const std::string &path)
{
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file) throw std::runtime_error("cannot open SPIR-V file: " + path);
    auto bytes = file.tellg();
    if (bytes < 0 || bytes % 4) throw std::runtime_error("invalid SPIR-V size");
    std::vector<uint32_t> code(static_cast<size_t>(bytes) / 4);
    file.seekg(0);
    if (!file.read(reinterpret_cast<char *>(code.data()), bytes))
        throw std::runtime_error("cannot read SPIR-V file: " + path);
    return code;
}
vk::ShaderModule create(const uint32_t *words, size_t count)
{
    if (!words || count == 0) throw std::invalid_argument("empty SPIR-V shader");
    vk::ShaderModuleCreateInfo info{};
    info.codeSize = count * sizeof(uint32_t);
    info.pCode = words;
    return require_device().createShaderModule(info);
}
vk::ShaderModule create(const std::vector<uint32_t> &words)
{
    return create(words.data(), words.size());
}
void destroy(vk::ShaderModule handle)
{
    if (handle) require_device().destroyShaderModule(handle);
}
}
}
