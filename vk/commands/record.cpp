#include "../internal.hpp"
namespace vk::record {
void begin(vk::CommandBuffer cmd, const vk::CommandBufferBeginInfo &info) { cmd.begin(info); }
void end(vk::CommandBuffer cmd) { cmd.end(); }
void pipeline(vk::CommandBuffer cmd, vk::PipelineBindPoint bind, vk::Pipeline p) {
    cmd.bindPipeline(bind, p);
}
void viewport(vk::CommandBuffer cmd, const vk::Viewport &view) { cmd.setViewport(0, view); }
void scissor(vk::CommandBuffer cmd, const vk::Rect2D &rect) { cmd.setScissor(0, rect); }
void draw(vk::CommandBuffer cmd, uint32_t v, uint32_t i, uint32_t fv, uint32_t fi) {
    cmd.draw(v, i, fv, fi);
}
void draw_indexed(vk::CommandBuffer cmd, uint32_t v, uint32_t i, uint32_t f,
                  int32_t offset, uint32_t first) {
    cmd.drawIndexed(v, i, f, offset, first);
}
void dispatch(vk::CommandBuffer cmd, uint32_t x, uint32_t y, uint32_t z) {
    cmd.dispatch(x, y, z);
}
void copy(vk::CommandBuffer cmd, vk::Buffer src, vk::Buffer dst,
          const vk::BufferCopy &region) { cmd.copyBuffer(src, dst, region); }
}
