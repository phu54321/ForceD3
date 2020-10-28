/*
 * Created 190201 lynnl
 */

#include <IOKit/IOLib.h>
#include <IOKit/pci/IOPCIDevice.h>
#include "ForceD3.h"

const int kWatchDogTimerPeriod = 5000;

/*
 * This required macro defines the class's constructors, destructors
 *  and several other methods I/O Kit requires.
 */
OSDefineMetaClassAndStructors(__IO_CLASS__, __IO_CLASS__::super);

bool __IO_CLASS__::init(OSDictionary *dictionary)
{
    return super::init(dictionary);
}

void __IO_CLASS__::free(void)
{
    return super::free();
}

IOService *__IO_CLASS__::probe(IOService *provider, SInt32 *score)
{
    auto pcieSlot = provider->getProvider();
    auto slotListArray = OSDynamicCast(OSArray, getProperty("SlotList"));
    if (!slotListArray)
        return nullptr;

    for (int i = 0; i < slotListArray->getCount(); i++)
    {
        auto slotName = OSDynamicCast(OSString, slotListArray->getObject(i));
        if (slotName && strcmp(pcieSlot->getName(), slotName->getCStringNoCopy()) == 0)
        {
            return this;
        }
    }
    return nullptr;
}

bool __IO_CLASS__::start(IOService *provider_)
{
    bool result = super::start(provider_);
    if (!result)
        return false;

    fWatchdogWorkLoop = getWorkLoop();
    if (!fWatchdogWorkLoop)
    {
        IOLog(" - failed to get fWatchdogWorkLoop\n");
        super::stop(provider_);
        return false;
    }
    watchdogTimer = IOTimerEventSource::timerEventSource(this, OSMemberFunctionCast(IOTimerEventSource::Action, this, &__IO_CLASS__::watchdogAction));
    if (!watchdogTimer)
    {
        IOLog(" - failed to create watchdogTimer\n");
        fWatchdogWorkLoop = nullptr;
        super::stop(provider_);
        return false;
    }
    fWatchdogWorkLoop->addEventSource(watchdogTimer);

    watchdogTimer->setTimeoutMS(kWatchDogTimerPeriod);
    watchdogTimer->enable();
}

void __IO_CLASS__::stop(IOService *provider)
{
    if (fWatchdogWorkLoop && watchdogTimer)
    {
        watchdogTimer->cancelTimeout();
        fWatchdogWorkLoop->removeEventSource(watchdogTimer);
        watchdogTimer->release();
        watchdogTimer = NULL;
        fWatchdogWorkLoop = NULL;
    }

    super::stop(provider);
}

void __IO_CLASS__::watchdogAction(IOTimerEventSource *timer)
{
    auto provider = OSDynamicCast(IOPCIDevice, getProvider()->getProvider());
    if (!provider)
    {
        return;
    }

    int currentPowerState = provider->getPowerState();

    if (currentPowerState == kIOPCIDeviceOnState)
    {
        provider->enablePCIPowerManagement(kPCIPMCSPowerStateD3);
        int ack = provider->setPowerState(kIOPCIDeviceOffState, this);
        if (ack == IOPMAckImplied)
        {
            IOLog(" - %s: state %d to off\n", provider->getName(), currentPowerState);
        }
        else
        {
            IOLog(" - %s: state %d to off w/ %dms threshold\n", provider->getName(), currentPowerState, ack);
        }
    }
    watchdogTimer->setTimeoutMS(kWatchDogTimerPeriod);
}
