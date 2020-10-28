/*
 * Created 190201 lynnl
 */

#ifndef __IOEXAMPLE_H__
#define __IOEXAMPLE_H__

#include <IOKit/IOService.h>
#include <IOKit/pci/IOPCIDevice.h>
#include <IOKit/IOLib.h>
#include <IOKit/IOTimerEventSource.h>

#ifndef __iokit_makefile__
#error Use makefile.
#endif

class __IO_CLASS__ : public IOPCIDevice
{
    using super = IOPCIDevice;

    OSDeclareDefaultStructors(__IO_CLASS__);

public:
    virtual bool init(OSDictionary *dictionary = 0);
    virtual void free(void);
    virtual IOService *probe(IOService *provider, SInt32 *score);

    virtual bool start(IOService *provider) override;
    virtual void stop(IOService *provider) override;

    void watchdogAction(IOTimerEventSource *timer);

private:
    IOTimerEventSource *watchdogTimer;
    IOWorkLoop *fWatchdogWorkLoop;
};

#endif /* __IOEXAMPLE_H__ */
