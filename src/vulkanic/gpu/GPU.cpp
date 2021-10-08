/* GPU.cpp
 *   by Lut99
 *
 * Created:
 *   07/10/2021, 22:01:13
 * Last edited:
 *   07/10/2021, 22:01:13
 * Auto updated?
 *   Yes
 *
 * Description:
 *   Contains the GPU class, which wraps around one or more GPUs, and uses
 *   them for rendering.
**/

#include <algorithm>

#include "tools/Logger.hpp"
#include "vulkanic/gpu/GPU.hpp"

using namespace std;
using namespace Makma3D;
using namespace Makma3D::Vulkanic;


/***** HELPER FUNCTIONS *****/






/***** GPU CLASS *****/
/* Constructor for the GPU class, which takes a Vulkan Instance to create the GPU with, a HardwareGPU to wrap around and flags that determine which GPU features are enabled. */
GPU::GPU(const Vulkanic::Instance& instance, const Vulkanic::HardwareGPU& physical_device, GPUFeatureFlags feature_flags) :
    instance(instance),
    physical_device(physical_device),
    feature_flags(feature_flags)
{
    // First, collect the list of queue infos (one per queue)


}

/* Move constructor for the GPU class. */
GPU::GPU(GPU&& other) :
    instance(other.instance),
    physical_device(std::move(other.physical_device)),
    vk_device(other.vk_device)
{
    other.vk_device = nullptr;
}

/* Destructor for the GPU class. */
GPU::~GPU() {
    if (this->vk_device != nullptr) {
        vkDestroyDevice(this->vk_device, nullptr);
    }
}



/* Swap operator for the GPU class. */
void Vulkanic::swap(GPU& g1, GPU& g2) {
    #ifndef NDEBUG
    if (&g1.instance != &g2.instance) { logger.fatalc(GPU::channel, "Cannot swap GPUs with different instances."); }
    #endif

    using std::swap;

    swap(g1.physical_device, g2.physical_device);
    swap(g1.vk_device, g2.vk_device);
}
