#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#ifdef _WIN64
#define MAX_INT uint64_t
#else
#define MAX_INT uint32_t
#endif
//#include <vulkan/vulkan.h>

#include <iostream>
#include <stdexcept>
#include <functional>
#include <cstdlib>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <fstream>
#include <array>

#include <glm/glm.hpp>

const int MAX_FRAMES_IN_FLIGHT = 2;


struct Vertex {
	glm::vec2 pos;
	glm::vec3 color;

	static VkVertexInputBindingDescription getBindingDescription() {
		VkVertexInputBindingDescription bindingDescription = {};

		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
		std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions = {};
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, pos);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);

		return attributeDescriptions;
	}
};

const std::vector<Vertex> vertices = {
	{{0.0f, -0.5f}, {1.0f, 1.0f, 1.0f}},
	{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
	{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
};

template<typename T>
class Optional {
public:
	Optional() { valueSet = false; };
	T& getValue() { return value; }
	bool hasValue() { return valueSet; }

	T& operator=(T value) {
		this->value = value;
		valueSet = true;
		return value;
	}
private:
	T value;
	bool valueSet;
};

struct QueueFamilyIndices {

	Optional<uint32_t> graphicsFamily;
	Optional<uint32_t> presentFamily;

	bool isComplete() {
		return graphicsFamily.hasValue() && presentFamily.hasValue();
	}
};

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

class HelloTriangleApplication {
public:
	void run();

private:

	//DebugSetup
	void setupDebugMessenger();
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);


	//Instance creation
	void createInstance();
	bool checkRequiredExtensionsPresent(std::vector<VkExtensionProperties> & suppExtension, const char** extensions, int size_extensions);
	std::vector<const char*> getRequiredExtensions();
	bool checkValidationLayerSupport();


	//Chose a graphics card
	void pickPhysicalDevice();
	bool isDeviceSuitable(VkPhysicalDevice device);
	int rateDeviceSuitability(VkPhysicalDevice device); //return 0 if device isnot suitable
	bool checkDeviceExtensionSupport(VkPhysicalDevice device); //checks for device extensions present

	//SwapChain support and format
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice physicalDevice);
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats); //collor format
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);   //presentation mode
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);		//resulution of images
	void createSwapChain();
	void recreateSwapChain();

	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

	//Buffer helper functions
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	//Used to interact with swapchain images
	void createImageViews();

	//Return the index of the queues
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

	//Logical device
	void createLogicalDevice();

	//SurfaceCreation
	void createSurface();

	//Graphics pipeline and shaders
	VkShaderModule createShaderModule(const std::vector<char>& code);
	static std::vector<char> readFile(const std::string& filename);
	void createRenderPass();
	void createGraphicsPipeline();

	//VertexBuffer in graphics memory
	void createVertexBuffer();
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	//Framebuffers
	void createFrameBuffers();

	//Cammand
	void createCommandPool();
	void createCommandBuffers();

	//Draw
	void drawFrame();
	void createSyncObjects();

	//MainCreation
	void initWindow();
	void initVulkan();
	void mainLoop();

	//Cleanup
	void cleanup();
	void cleanupSwapChain();

private:
	GLFWwindow* window;
	VkInstance instance;
	VkPhysicalDevice physicalDevice;
	VkDevice device;
	VkSurfaceKHR surface;

	VkCommandPool commandPool;


	//Queues
	VkQueue graphicsQueue;
	VkQueue presentQueue;

	//Buffers
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;

	//PipelineStuff
	VkRenderPass renderPass;
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;

	//SwapChain stuff
	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;		//Implicited destruction, so no need to cleanup
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	std::vector<VkImageView> swapChainImageViews;

	//Draw semaphores
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	size_t currentFrame = 0;
	bool framebufferResized = false;

	//Buffers
	std::vector<VkFramebuffer> swapChainFramebuffers;
	std::vector<VkCommandBuffer> commandBuffers;

	//Window Settings
	const int WIDTH = 800;
	const int HEIGHT = 600;

	//Validation layers needed
	const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };

	VkDebugUtilsMessengerEXT debugMessenger;

	//Extensions needed in device
	const std::vector<const char*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

	//Device suitability
	bool needGeometryShader = true;

	//Presentation Settings

	/* Possible presentation modes
	VK_PRESENT_MODE_IMMEDIATE_KHR
	VK_PRESENT_MODE_FIFO_KHR -> Always available
	VK_PRESENT_MODE_FIFO_RELAXED_KHR
	VK_PRESENT_MODE_MAILBOX_KHR
	*/
	const uint32_t preferedPresentationMode = VK_PRESENT_MODE_MAILBOX_KHR;

#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif
};
