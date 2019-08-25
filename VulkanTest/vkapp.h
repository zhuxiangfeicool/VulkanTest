#pragma once

#include <vector>
#include <iostream>
#include<io.h>
#include "assert.h"
#define VK_USE_PLATFORM_WIN32_KHR
#include "vulkan/vulkan.h"
#include "vulkan/vulkan_win32.h"
#define GLFW_INCLUDE_VULKAN
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include<glm/glm.hpp>

class vkapp {
public:
    VkResult init( HINSTANCE hinstance, GLFWwindow* window );
    void finish( );
    VkResult enumLayerProperties( );
    VkResult enumExtensionProperties( );
    VkResult createBuffer( );
    VkResult createImage( );
    VkResult createCommandBuffers( );
    VkResult createSwapchain( );
    VkResult createPipeline( );
    ~vkapp( );
private:
    HINSTANCE hinstance = nullptr;
    GLFWwindow* window = nullptr;
    VkSurfaceKHR mSurface;
    std::vector<VkSurfaceFormatKHR> mSurfaceFormats;
    VkSwapchainKHR mSwapchain;
    std::vector<VkImage> mSwapchainImages;
    VkSurfaceCapabilitiesKHR mSurfaceCapabilities;

    std::vector<VkSubpassDescription> mSubpassDescriptions;
    VkRenderPass mRenderPass;
    VkPipeline mPipeline;
    std::vector<VkFramebuffer> mFramebuffers;
private:
    VkInstance mVulkanInstance;
    std::vector<VkPhysicalDevice> mPhysicalDevices;
    std::vector<VkPhysicalDeviceProperties> mPhysicalDevicesProperties;
    std::vector<VkPhysicalDeviceFeatures> mPhysicalDevicesFeatures;
    std::vector<VkPhysicalDeviceMemoryProperties> mPhysicalDevicesMemoryProperties;
    std::vector<std::vector<VkQueueFamilyProperties>> mQueueFamiliesProperties;
    std::vector<VkDevice> mDevices;

    VkQueue mQueue;
    VkCommandPool mCommandPool;
    std::vector<VkCommandBuffer> mCommandBuffers;

    std::vector<VkLayerProperties> mInstanceLayerProperties;
    std::vector<std::vector<VkLayerProperties>> mDevicesLayerProperties;
    std::vector<VkExtensionProperties> mInstanceExtensionProperties;
    std::vector<std::vector<VkExtensionProperties>> mDevicesExtensionProperties;

    VkBuffer mBuffer;
    VkImage mImage;
};