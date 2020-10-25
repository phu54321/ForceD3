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

bool __IO_CLASS__::init(OSDictionary *dictionary)
{
    IOLog("init\n");
    return super::init(dictionary);
}

void __IO_CLASS__::free(void)
{
    IOLog("free\n");
    return super::free();
}

IOService *__IO_CLASS__::probe(IOService *provider, SInt32 *score)
{
    IOLog("probe %s\n", provider->getName());
    return super::probe(provider, score);
}

bool __IO_CLASS__::start(IOService *provider_)
{
    bool result = super::start(provider_);
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
        provider->setPowerState(kIOPCIDeviceOffState, this);
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
    super::stop(provider);
    IOLog("Stopping\n");
}

IOReturn __IO_CLASS__::powerStateDidChangeTo(
    IOPMPowerFlags capabilities,
    unsigned long stateNumber,
    IOService *whatDevice)
{
    IOLog("powerStateDidChangeTo: %lu, %lu, %s\n", capabilities, stateNumber, whatDevice->getName());
    if (
        whatDevice == getProvider() &&
        stateNumber == kIOPCIDeviceOnState)
    {
        auto provider = OSDynamicCast(IOPCIDevice, whatDevice);
        provider->setPowerState(kIOPCIDeviceOffState, this);
        IOLog(" - PCI device powered off due to powerStateDidChangeTo (%lu)\n", stateNumber);
    }
    return IOPMAckImplied;
}
