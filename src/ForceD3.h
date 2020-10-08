/*
 * Created 190201 lynnl
 */

#ifndef __IOEXAMPLE_H__
#define __IOEXAMPLE_H__

#include <IOKit/IOService.h>

#ifndef __iokit_makefile__
#error Use makefile.
#endif

class __IO_CLASS__ : public IOService {
    OSDeclareDefaultStructors(__IO_CLASS__)

public:
    bool start(IOService *provider) override;
    void stop(IOService *provider) override;

    virtual IOReturn powerStateDidChangeTo(
        IOPMPowerFlags capabilities,
        unsigned long stateNumber,
        IOService *whatDevice );
};

#endif      /* __IOEXAMPLE_H__ */

