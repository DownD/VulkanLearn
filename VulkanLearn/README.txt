Instance-> specify extension and needed validations layers for setup debug
SetupDebug layers
CreateSurface-> create the surface to draw
PickPhysicalDevice-> found physical device suitable (Check for swapChain and specific queues support) by a ranking criteria
CreateLogicalDevice-> create a logical device with features, queues and extensions needed
CreateSwapChain-> Create a swapchain (check for device and surface support) with desired presentation mode, usage and others
Compileshaders-> compile the shaders(can be at runtime using shaderc)
CreateGraphicsPipeline-> create graphics pipeline along renderpass and other settings
CreateVertexBuffer-> create buffer, check memory requirements and allocate memory

Fix Window Resize
	- RecreateSwapChain, there is other option without recreate the pipeline ->https://vulkan-tutorial.com/en/Drawing_a_triangle/Swap_chain_recreation
	  also, loading shaders file can be consuming
	- Checks on AcquireNextImageKHR and vkQueuePresentKHR for error or unooptimal use of swapchain
	- Some drivers don't garuatue feedback on surface resize, so its need to check for a surface resize using glfw

Best Performance on memory
	-For better perfomance it must be used a memory that is not accesible by the CPU (VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
	-2 Buffers are needed, one in RAM memory and other in device memory
	-To copy memory is needed a command buffer and a copy memory command to be submit to that buffer
	-In real world aplication vkAllocateMemory shouldn't be called for every individual buffer