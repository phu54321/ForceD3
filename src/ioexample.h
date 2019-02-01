/*
 * Created 190201 lynnl
 */

#ifndef __IOEXAMPLE_H__
#define __IOEXAMPLE_H__

#include <IOKit/IOService.h>

/* Define the driver's superclass */
//#define super IOService

#ifndef __iokit_makefile__
#define __IO_CLASS__        "com_example_driver_ioexample"
#endif

class __IO_CLASS__ : public IOService {
    OSDeclareDefaultStructors(__IO_CLASS__)

public:
    bool init(OSDictionary *dictionary = NULL) override;
    void free(void) override;
    IOService *probe(IOService *provider, SInt32 *score) override;
    bool start(IOService *provider) override;
    void stop(IOService *provider) override;
};

#endif      /* __IOEXAMPLE_H__ */

