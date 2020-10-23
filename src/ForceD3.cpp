/*
 * Created 190201 lynnl
 */

#include <IOKit/IOLib.h>
#include <IOKit/pci/IOPCIDevice.h>
#include "ForceD3.h"

/*
 * This required macro defines the class's constructors, destructors
 *  and several other methods I/O Kit requires.
 */
OSDefineMetaClassAndStructors(__IO_CLASS__, IOService);

bool forceSetPowerStateD3(IOPCIDevice *device)
{
    // From https://www.notion.so/Using-VoodooI2C-on-comet-lake-cpu-e-g-i5-10210u-142930887087445eaa533120455da5dc
    // I'm the author of that notion article :)
    IOByteCount offset;
    device->extendedFindPCICapability(kIOPCIPowerManagementCapability, &offset);

    // Let's play hack. Force D3 here
    uint16_t oldPMbits = device->extendedConfigRead16(offset + 0x4);
    uint16_t newPMbits = (oldPMbits & (~0x3)) | 0x3;
    device->extendedConfigWrite16(offset + 0x4, newPMbits);
}

bool __IO_CLASS__::start(IOService *provider_)
{
    bool result = IOService::start(provider_);
    IOLog("Starting\n");

    IOLog("Initializing ForceD3: inject to %s\n", provider_->getName());
    if (result)
    {
        auto provider = OSDynamicCast(IOPCIDevice, provider_);

        // register observer for powerStateDidChangeTo
        IOLog(" - registering observer for power state change notification\n");
        provider->registerInterestedDriver(this);

        // Allow D3. IDK this is required, but it's safe to have one.
        IOLog(" - enablePCIPowerManagement(D3)\n");
        provider->enablePCIPowerManagement(kPCIPMCSPowerStateD3);

        // Set!
        forceSetPowerStateD3(provider);
        IOLog(" - PCI device powered off (D3)\n");
        return true;
    }
    else
    {
        IOLog("Failed...\n");
        return false;
    }
}

void __IO_CLASS__::stop(IOService *provider)
{
    IOService::stop(provider);
    IOLog("Stopping\n");
}

IOReturn __IO_CLASS__::powerStateDidChangeTo(
    IOPMPowerFlags capabilities,
    unsigned long stateNumber,
    IOService *whatDevice)
{
    IOLog("powerStateDidChangeTo: %d, %d, %s\n", capabilities, stateNumber, whatDevice->getName());
    if (stateNumber == kIOPCIDeviceOnState)
    {
        auto provider = OSDynamicCast(IOPCIDevice, whatDevice);
        forceSetPowerStateD3(provider);
        IOLog(" - PCI device powered off due to powerStateDidChangeTo (%d)\n", stateNumber);
    }
    return IOPMAckImplied;
}
