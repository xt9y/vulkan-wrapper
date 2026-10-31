#pragma once
#include "../common.hpp"
namespace vk::record {
void begin(vk::CommandBuffer command, const vk::CommandBufferBeginInfo &info = {});
void end(vk::CommandBuffer command);
void pipeline(vk::CommandBuffer command, vk::PipelineBindPoint point, vk::Pipeline pipeline);
void viewport(vk::CommandBuffer command, const vk::Viewport &viewport);
void scissor(vk::CommandBuffer command, const vk::Rect2D &rect);
void draw(vk::CommandBuffer command, uint32_t vertices, uint32_t instances = 1,
          uint32_t first_vertex = 0, uint32_t first_instance = 0);
void draw_indexed(vk::CommandBuffer command, uint32_t indices, uint32_t instances = 1,
                  uint32_t first_index = 0, int32_t vertex_offset = 0,
                  uint32_t first_instance = 0);
void dispatch(vk::CommandBuffer command, uint32_t x, uint32_t y = 1, uint32_t z = 1);
void copy(vk::CommandBuffer command, vk::Buffer source, vk::Buffer destination,
          const vk::BufferCopy &region);
}
