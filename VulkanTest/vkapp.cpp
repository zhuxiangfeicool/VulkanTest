
#include "pch.h"
#include "vkapp.h"

VkResult vkapp::init( HINSTANCE hinstance, GLFWwindow* window ) {
    VkResult res = VK_SUCCESS;

    this->hinstance = hinstance;
    this->window = window;

    VkApplicationInfo appInfo = {
         VK_STRUCTURE_TYPE_APPLICATION_INFO, nullptr,
         "VulkanTest",
         1,
         nullptr, 0,
         VK_MAKE_VERSION( 1, 0, 0 )
    };
    VkInstanceCreateInfo instanceCreateInfo = {
        VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO, nullptr, 0,
        &appInfo,
        0, nullptr,
        0, nullptr
    };

    res = vkCreateInstance( &instanceCreateInfo, nullptr, &mVulkanInstance );
    assert( VK_SUCCESS == res );

    uint32_t physicalDeviceCount = 0;
    res = vkEnumeratePhysicalDevices( mVulkanInstance, &physicalDeviceCount, nullptr );
    assert( VK_SUCCESS == res );
    mPhysicalDevices.resize( physicalDeviceCount );
    mPhysicalDevicesProperties.resize( physicalDeviceCount );
    mPhysicalDevicesFeatures.resize( physicalDeviceCount );
    mPhysicalDevicesMemoryProperties.resize( physicalDeviceCount );
    mQueueFamiliesProperties.resize( physicalDeviceCount );
    res = vkEnumeratePhysicalDevices( mVulkanInstance, &physicalDeviceCount, mPhysicalDevices.data( ) );
    assert( VK_SUCCESS == res );

    VkPhysicalDevice* tmpPhysicalDevice = nullptr;
    uint32_t tmpQueueFamilyPropertyCount = 0;
    for ( int i = 0; i < physicalDeviceCount; i++ ) {
        tmpPhysicalDevice = &mPhysicalDevices.at( i );
        vkGetPhysicalDeviceProperties( *tmpPhysicalDevice, &mPhysicalDevicesProperties.at( i ) );
        vkGetPhysicalDeviceFeatures( *tmpPhysicalDevice, &mPhysicalDevicesFeatures.at( i ) );
        vkGetPhysicalDeviceMemoryProperties( *tmpPhysicalDevice, &mPhysicalDevicesMemoryProperties.at( i ) );

        vkGetPhysicalDeviceQueueFamilyProperties( *tmpPhysicalDevice, &tmpQueueFamilyPropertyCount, nullptr );
        mQueueFamiliesProperties.at( i ).resize( tmpQueueFamilyPropertyCount );
        vkGetPhysicalDeviceQueueFamilyProperties( *tmpPhysicalDevice, &tmpQueueFamilyPropertyCount, mQueueFamiliesProperties.at( i ).data( ) );
    }

    uint32_t physicalDeviceIndex = 0;
    mDevices.resize( 1 );
    VkDeviceQueueCreateInfo deviceQueueCreateInfo = {
        VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO, nullptr, 0,
        0, 1, nullptr
    };

    VkPhysicalDeviceFeatures physicalDeviceFeatures = {};
    physicalDeviceFeatures.multiDrawIndirect = mPhysicalDevicesFeatures.at( physicalDeviceIndex ).multiDrawIndirect;
    //physicalDeviceFeatures.geometryShader = VK_TRUE;
    //physicalDeviceFeatures.tessellationShader = VK_TRUE;

    VkDeviceCreateInfo deviceCreateInfo = {
        VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO, nullptr, 0,
        1, &deviceQueueCreateInfo,
        0, nullptr, 0, nullptr,
        &physicalDeviceFeatures
    };

    std::cout << "Hello World!\n";
    res = vkCreateDevice( mPhysicalDevices.at( physicalDeviceIndex ), &deviceCreateInfo, nullptr, &mDevices.at( 0 ) );
    assert( VK_SUCCESS == res );

    vkGetDeviceQueue( mDevices.at( 0 ), 0, 0, &mQueue );
    return res;
}

void vkapp::finish( ) {
    for ( auto& item : mDevices ) {
        vkDestroyDevice( item, nullptr );
    }
    vkDestroyInstance( mVulkanInstance, nullptr );
}

VkResult vkapp::enumLayerProperties( ) {
    uint32_t tmpLayerPropertiesCount = 0;
    VkResult res = vkEnumerateInstanceLayerProperties( &tmpLayerPropertiesCount, nullptr );
    assert( VK_SUCCESS == res );
    mInstanceLayerProperties.resize( tmpLayerPropertiesCount );
    res = vkEnumerateInstanceLayerProperties( &tmpLayerPropertiesCount, mInstanceLayerProperties.data( ) );
    assert( VK_SUCCESS == res );

    mDevicesLayerProperties.resize( mPhysicalDevices.size( ) );
    for ( uint32_t i = 0; i < mPhysicalDevices.size( ); i++ ) {
        res = vkEnumerateDeviceLayerProperties( mPhysicalDevices.at( i ), &tmpLayerPropertiesCount, nullptr );
        assert( VK_SUCCESS == res );
        mDevicesLayerProperties.at( i ).resize( tmpLayerPropertiesCount );
        res = vkEnumerateDeviceLayerProperties( mPhysicalDevices.at( i ), &tmpLayerPropertiesCount, mDevicesLayerProperties.at( i ).data( ) );
        assert( VK_SUCCESS == res );
    }
    return res;
}

VkResult vkapp::enumExtensionProperties( ) {
    uint32_t tmpExtensionPropertiesCount = 0;
    VkResult res = vkEnumerateInstanceExtensionProperties( nullptr, &tmpExtensionPropertiesCount, nullptr );
    assert( VK_SUCCESS == res );
    mInstanceExtensionProperties.resize( tmpExtensionPropertiesCount );
    res = vkEnumerateInstanceExtensionProperties( nullptr, &tmpExtensionPropertiesCount, mInstanceExtensionProperties.data( ) );
    assert( VK_SUCCESS == res );

    mDevicesExtensionProperties.resize( mPhysicalDevices.size( ) );
    for ( uint32_t i = 0; i < mPhysicalDevices.size( ); i++ ) {
        res = vkEnumerateDeviceExtensionProperties( mPhysicalDevices.at( i ), nullptr, &tmpExtensionPropertiesCount, nullptr );
        assert( VK_SUCCESS == res );
        mDevicesExtensionProperties.resize( tmpExtensionPropertiesCount );
        res = vkEnumerateDeviceExtensionProperties( mPhysicalDevices.at( i ), nullptr, &tmpExtensionPropertiesCount, mDevicesExtensionProperties.at( i ).data( ) );
        assert( VK_SUCCESS == res );
    }
    return res;
}

VkResult vkapp::createBuffer( ) {
    VkBufferCreateInfo bufferCreateInfo = {
        VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        nullptr,
        0,
        1024 * 1024,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_SHARING_MODE_EXCLUSIVE,
        0, nullptr
    };

    VkResult res = vkCreateBuffer( mDevices.at( 0 ), &bufferCreateInfo, nullptr, &mBuffer );
    assert( VK_SUCCESS == res );
    return res;
}

VkResult vkapp::createImage( ) {
    VkImageCreateInfo imageCreateInfo = {
        VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        nullptr,
        0,
        VK_IMAGE_TYPE_2D,
        VK_FORMAT_R8G8B8A8_UNORM,
        {1024,1024,1},
        10,
        1,
        VK_SAMPLE_COUNT_1_BIT,
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_SHARING_MODE_EXCLUSIVE,
        0,
        nullptr,
        VK_IMAGE_LAYOUT_UNDEFINED
    };

    VkResult res = vkCreateImage( mDevices.at( 0 ), &imageCreateInfo, nullptr, &mImage );
    assert( VK_SUCCESS == res );
    return res;
}

VkResult vkapp::createCommandBuffers( ) {
    VkCommandPoolCreateInfo commandPoolCreateInfo = {
        VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO, nullptr,
        VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        0
    };
    VkResult res = vkCreateCommandPool( mDevices.at( 0 ), &commandPoolCreateInfo, nullptr, &mCommandPool );
    assert( VK_SUCCESS == res );

    mCommandBuffers.resize( 1 );
    VkCommandBufferAllocateInfo commandBufferAllocateInfo = {
        VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO, nullptr,
        mCommandPool,
        VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        1
    };
    res = vkAllocateCommandBuffers( mDevices.at( 0 ), &commandBufferAllocateInfo, &mCommandBuffers.at( 0 ) );
    assert( VK_SUCCESS == res );
    return res;
}

VkResult vkapp::createSwapchain( ) {
    bool presentSupport = vkGetPhysicalDeviceWin32PresentationSupportKHR(
        mPhysicalDevices.at( 0 ), 0 );
    assert( presentSupport );

    VkWin32SurfaceCreateInfoKHR win32SurfaceCreateInfo = {
        VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR, nullptr, 0,
        hinstance, glfwGetWin32Window( window )
    };
    VkResult res = vkCreateWin32SurfaceKHR( mVulkanInstance, &win32SurfaceCreateInfo, nullptr, &mSurface );
    assert( VK_SUCCESS == res );

    uint32_t formatsCount = 0;
    res = vkGetPhysicalDeviceSurfaceFormatsKHR( mPhysicalDevices.at( 0 ), mSurface, &formatsCount, nullptr );
    assert( VK_SUCCESS == res );
    mSurfaceFormats.resize( formatsCount );
    res = vkGetPhysicalDeviceSurfaceFormatsKHR( mPhysicalDevices.at( 0 ), mSurface, &formatsCount, mSurfaceFormats.data( ) );
    assert( VK_SUCCESS == res );

    res = vkGetPhysicalDeviceSurfaceCapabilitiesKHR( mPhysicalDevices.at( 0 ), mSurface, &mSurfaceCapabilities );
    assert( VK_SUCCESS == res );

    VkSwapchainCreateInfoKHR swapchainCreateInfo = {
        VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR, nullptr, 0,
        mSurface,
        3,
        VK_FORMAT_R8G8B8A8_UNORM,
        VK_COLORSPACE_SRGB_NONLINEAR_KHR,
        {mSurfaceCapabilities.currentExtent.width, mSurfaceCapabilities.currentExtent.height },
        1,
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        VK_SHARING_MODE_EXCLUSIVE,
        0, nullptr,
        mSurfaceCapabilities.currentTransform,
        VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        VK_PRESENT_MODE_FIFO_KHR,
        VK_TRUE,
        nullptr
    };
    res = vkCreateSwapchainKHR( mDevices.at( 0 ), &swapchainCreateInfo, nullptr, &mSwapchain );
    assert( VK_SUCCESS == res );

    uint32_t swapchainImagesCount = 0;
    res = vkGetSwapchainImagesKHR( mDevices.at( 0 ), mSwapchain, &swapchainImagesCount, nullptr );
    assert( VK_SUCCESS == res );
    mSwapchainImages.resize( swapchainImagesCount );
    res = vkGetSwapchainImagesKHR( mDevices.at( 0 ), mSwapchain, &swapchainImagesCount, mSwapchainImages.data( ) );
    assert( VK_SUCCESS == res );

    VkBool32 queueFamilySupportSurface = false;
    res = vkGetPhysicalDeviceSurfaceSupportKHR( mPhysicalDevices.at( 0 ), 0, mSurface, &queueFamilySupportSurface );
    return res;
}

VkResult vkapp::createPipeline( ) {
    FILE* file = nullptr;
    uint32_t size = 0;
    std::vector<char> codeVert, codeFrag;

    file = fopen( "vert.spv", "rb" );
    if ( file ) {
        size = _filelength( _fileno( file ) );
        codeVert.resize( size );
        fread( codeVert.data( ), 1, size, file );
        fclose( file );
    }
    VkShaderModuleCreateInfo vertexShaderCreateInfo = {
        VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO, nullptr, 0,
        size, reinterpret_cast<uint32_t const *>( codeVert.data( ) )
    };

    file = fopen( "frag.spv", "rb" );
    if ( file ) {
        size = _filelength( _fileno( file ) );
        codeFrag.resize( size );
        fread( codeFrag.data( ), 1, size, file );
        fclose( file );
    }
    VkShaderModuleCreateInfo fragmentShaderCreateInfo = {
        VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO, nullptr, 0,
        size, reinterpret_cast<uint32_t const *>( codeFrag.data( ) )
    };

    VkShaderModule vertShaderModule, fragShaderModule;
    VkResult res = vkCreateShaderModule( mDevices.at( 0 ), &vertexShaderCreateInfo, nullptr, &vertShaderModule );
    assert( VK_SUCCESS == res );
    res = vkCreateShaderModule( mDevices.at( 0 ), &fragmentShaderCreateInfo, nullptr, &fragShaderModule );
    assert( VK_SUCCESS == res );

    VkDescriptorSetLayoutBinding uniformBlock = {
        0,
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        1,
        VK_SHADER_STAGE_ALL,
        nullptr
    };
    VkDescriptorSetLayoutCreateInfo uniformSetLayoutCreateInfo = {
        VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO, nullptr, 0,
        1, &uniformBlock
    };
    VkDescriptorSetLayout uniformSetLayout;
    vkCreateDescriptorSetLayout( mDevices.at( 0 ), &uniformSetLayoutCreateInfo, nullptr, &uniformSetLayout );
    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {
        VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO, nullptr, 0,
        1, &uniformSetLayout, 0, nullptr
    };
    VkPipelineLayout pipelineLayout;
    vkCreatePipelineLayout( mDevices.at( 0 ), &pipelineLayoutCreateInfo, nullptr, &pipelineLayout );

    VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfos[2];
    pipelineShaderStageCreateInfos[0] = {
        VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, nullptr, 0,
        VK_SHADER_STAGE_VERTEX_BIT, vertShaderModule, "main",
        nullptr
    };
    pipelineShaderStageCreateInfos[1] = {
        VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, nullptr, 0,
        VK_SHADER_STAGE_FRAGMENT_BIT, fragShaderModule, "main",
        nullptr
    };
    typedef struct vertex_t {
        glm::vec4 vertexCoord;
        glm::vec4 vertexColor;
    } vertex;
    VkVertexInputBindingDescription vertexInputBindingDescriptions[ ] = {
        { 0, sizeof( vertex ), VK_VERTEX_INPUT_RATE_VERTEX }
    };
    VkVertexInputAttributeDescription vertexInputAttributeDescriptions[ ] = {
        { 0, 0, VK_FORMAT_R32G32B32A32_SFLOAT, 0 },
        { 1, 0, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof( vertex, vertexColor )}
    };
    VkPipelineVertexInputStateCreateInfo pipelineVertexInputStageCreateInfo = {
        VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO, nullptr, 0,
        ARRAYSIZE( vertexInputBindingDescriptions ), vertexInputBindingDescriptions,
        ARRAYSIZE( vertexInputAttributeDescriptions ), vertexInputAttributeDescriptions
    };

    VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo = {
        VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO, nullptr, 0,
        VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FALSE
    };
    VkViewport viewports[ ] = {
        {0, 0, 1.0f, 1.0f, 0.1f, 1000.0f}
    };
    VkRect2D scissor[ ] = {
        {{0, 0},{1,1} }
    };
    VkPipelineViewportStateCreateInfo pipelineViewportStateCreateInfo = {
        VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO, nullptr, 0,
        sizeof( viewports ) / sizeof( VkViewport ), viewports,
        sizeof( scissor ) / sizeof( VkRect2D ), scissor
    };
    VkPipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo = {
        VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO, nullptr, 0,
        VK_FALSE, VK_TRUE,
        VK_POLYGON_MODE_FILL,
        VK_CULL_MODE_BACK_BIT,
        VK_FRONT_FACE_COUNTER_CLOCKWISE,
        VK_FALSE,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    VkAttachmentDescription attachmentDescriptions[ ] = {
        {
            0, VK_FORMAT_R8G8B8A8_UNORM,VK_SAMPLE_COUNT_1_BIT,VK_ATTACHMENT_LOAD_OP_CLEAR,VK_ATTACHMENT_STORE_OP_STORE,
            VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE,
            VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
        }
    };
    VkAttachmentReference attachmentReference[ ] = {
        {
            0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
        }
    };
    mSubpassDescriptions.resize( 1 );
    mSubpassDescriptions[0] = {
        0, VK_PIPELINE_BIND_POINT_GRAPHICS,
        0, nullptr,
        1, attachmentReference, nullptr, nullptr,
        0, nullptr
    };
    VkRenderPassCreateInfo renderPassCreateInfo = {
        VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO, nullptr, 0,
        sizeof( attachmentDescriptions ) / sizeof( VkAttachmentDescription ), attachmentDescriptions,
        1, mSubpassDescriptions.data(),
        0, nullptr
    };
    vkCreateRenderPass(mDevices.at(0), &renderPassCreateInfo, nullptr, &mRenderPass );
    VkGraphicsPipelineCreateInfo pipelineCreateInfo = {
        VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO, nullptr,0,
        sizeof( pipelineShaderStageCreateInfos ) / sizeof( VkPipelineShaderStageCreateInfo ), pipelineShaderStageCreateInfos,
        &pipelineVertexInputStageCreateInfo,
        &pipelineInputAssemblyStateCreateInfo,
        nullptr,
        &pipelineViewportStateCreateInfo,
        &pipelineRasterizationStateCreateInfo,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        VK_NULL_HANDLE,
        mRenderPass,
        0,
        VK_NULL_HANDLE,
        0,
    };
    res = vkCreateGraphicsPipelines(mDevices.at(0), VK_NULL_HANDLE,
                               1, &pipelineCreateInfo,
                               nullptr, &mPipeline);
    assert( VK_SUCCESS == res );

    std::vector<VkImageViewCreateInfo> vkImageViewCreateInfos;
    std::vector<VkImageView> vkImageViews;
    std::vector<VkFramebufferCreateInfo> framebufferCreateInfos;
    vkImageViewCreateInfos.resize( mSwapchainImages.size( ) );
    vkImageViews.resize( mSwapchainImages.size( ) );
    framebufferCreateInfos.resize( mSwapchainImages.size( ) );
    mFramebuffers.resize( mSwapchainImages.size( ) );
    for ( uint32_t i = 0; i < mSwapchainImages.size( ); i++ ) {
        vkImageViewCreateInfos[i] = {
            VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO, nullptr, 0,
            mSwapchainImages[i],
            VK_IMAGE_VIEW_TYPE_2D,
            VK_FORMAT_R8G8B8A8_UNORM,
            { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A },
            { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 },
        };
        vkCreateImageView( mDevices.at( 0 ), &( vkImageViewCreateInfos[i] ), nullptr, &vkImageViews[i] );
        VkFramebufferCreateInfo framebufferCreateInfo = {
            VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO, nullptr, 0,
            mRenderPass,
            1, &vkImageViews[i],
            mSurfaceCapabilities.currentExtent.width, mSurfaceCapabilities.currentExtent.height,
            1
        };
        vkCreateFramebuffer( mDevices[0], &framebufferCreateInfos[i], nullptr, &mFramebuffers[i] );
    }
    return res;
}

vkapp::~vkapp( ) {
    /*vkFreeCommandBuffers( mDevices.at( 0 ), mCommandPool, mCommandBuffers.size( ), mCommandBuffers.data( ) );
    vkDestroyCommandPool( mDevices.at( 0 ), mCommandPool, nullptr );
    */
}