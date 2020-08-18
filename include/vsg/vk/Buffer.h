#pragma once

/* <editor-fold desc="MIT License">

Copyright(c) 2018 Robert Osfield

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

</editor-fold> */

#include <vsg/vk/DeviceMemory.h>
#include <vsg/vk/vk_buffer.h>

namespace vsg
{
    // forward declare
    class Context;

    class VSG_DECLSPEC Buffer : public Inherit<Object, Buffer>
    {
    public:
        Buffer(Device* device, VkDeviceSize size, VkBufferUsageFlags usage, VkSharingMode sharingMode);


        /// Vulkan VkImage handle
        VkBuffer vk(uint32_t deviceID) const { return _vulkanData[deviceID].buffer; }


        VkBufferUsageFlags usage() const { return _usage; }
        VkSharingMode shaderMode() const { return _sharingMode; }


        VkResult bind(DeviceMemory* deviceMemory, VkDeviceSize memoryOffset);

        MemorySlots::OptionalOffset reserve(VkDeviceSize size, VkDeviceSize alignment) { return _memorySlots.reserve(size, alignment); }
        void release(VkDeviceSize offset, VkDeviceSize size) { _memorySlots.release(offset, size); }
        bool full() const { return _memorySlots.full(); }
        VkDeviceSize maximumAvailableSpace() const { return _memorySlots.maximumAvailableSpace(); }
        const MemorySlots& memorySlots() const { return _memorySlots; }

        VkMemoryRequirements getMemoryRequirements(uint32_t deviceID) const;

        DeviceMemory* getDeviceMemory(uint32_t deviceID) { return _vulkanData[deviceID].deviceMemory; }
        const DeviceMemory* getDeviceMemory(uint32_t deviceID) const { return _vulkanData[deviceID].deviceMemory; }

        VkDeviceSize getMemoryOffset(uint32_t deviceID) const { return _vulkanData[deviceID].memoryOffset; }

    protected:
        virtual ~Buffer();

        struct VulkanData
        {
            VkBuffer buffer = VK_NULL_HANDLE;
            ref_ptr<DeviceMemory> deviceMemory;
            VkDeviceSize memoryOffset = 0;
            VkDeviceSize size = 0;
            ref_ptr<Device> device;

            void release();
        };

        vk_buffer<VulkanData> _vulkanData;

        VkBufferUsageFlags _usage;
        VkSharingMode _sharingMode;
        MemorySlots _memorySlots;
    };
    VSG_type_name(vsg::Buffer);

} // namespace vsg
