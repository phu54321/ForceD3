/*
 * Created 190201 lynnl
 */

#ifndef __IOEXAMPLE_H__
#define __IOEXAMPLE_H__

#include <IOKit/IOService.h>

#ifndef __iokit_makefile__
#define __IO_CLASS__        com_example_driver_ioexample
#endif

class __IO_CLASS__ : public IOService {
    OSDeclareDefaultStructors(__IO_CLASS__)

public:
    bool init(OSDictionary *dictionary = NULL);
    void free(void);
    IOService *probe(IOService *provider, SInt32 *score);
    bool start(IOService *provider);
    void stop(IOService *provider);
};

#endif      /* __IOEXAMPLE_H__ */

