/* <editor-fold desc="MIT License">

Copyright(c) 2018 Robert Osfield

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

</editor-fold> */

#include <vsg/core/Exception.h>
#include <vsg/io/Options.h>
#include <vsg/state/BufferView.h>
#include <vsg/vk/Context.h>

using namespace vsg;

void BufferView::VulkanData::release()
{
    if (bufferView)
    {
        vkDestroyBufferView(*device, bufferView, device->getAllocationCallbacks());
        bufferView = VK_NULL_HANDLE;
        device = {};
    }
}

BufferView::~BufferView()
{
    for(auto& vd : _vulkanData) vd.release();
}

void BufferView::compile(Device* device)
{
    auto& vd = _vulkanData[device->deviceID];
    if (vd.bufferView != VK_NULL_HANDLE) return;

    VkBufferViewCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO;
    createInfo.buffer = buffer->vk(device->deviceID);
    createInfo.format = format;
    createInfo.offset = offset;
    createInfo.range = range;
    createInfo.pNext = nullptr;

    vd.device = device;

    if (VkResult result = vkCreateBufferView(*device, &createInfo, device->getAllocationCallbacks(), &vd.bufferView); result != VK_SUCCESS)
    {
        throw Exception{"Error: Failed to create BufferView.", result};
    }
}

void BufferView::compile(Context& context)
{
    compile(context.device);
}
