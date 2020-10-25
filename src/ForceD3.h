/*
 * Created 190201 lynnl
 */

#ifndef __IOEXAMPLE_H__
#define __IOEXAMPLE_H__

#include <IOKit/IOService.h>

#ifndef __iokit_makefile__
#error Use makefile.
#endif

class __IO_CLASS__ : public IOService
{
    using super = IOService;

    OSDeclareDefaultStructors(__IO_CLASS__);

public:
    virtual bool init(OSDictionary *dictionary = 0);
    virtual void free(void);
    virtual IOService *probe(IOService *provider, SInt32 *score);

    virtual bool start(IOService *provider) override;
    virtual void stop(IOService *provider) override;

    virtual IOReturn powerStateDidChangeTo(
        IOPMPowerFlags capabilities,
        unsigned long stateNumber,
        IOService *whatDevice) override;
};

#endif /* __IOEXAMPLE_H__ */
