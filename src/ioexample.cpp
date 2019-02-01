/*
 * Created 190201 lynnl
 */

#include <IOKit/IOLib.h>
#include "ioexample.h"

/*
 * This required macro defines the class's constructors, destructors
 *  and several other methods I/O Kit requires.
 */
OSDefineMetaClassAndStructors(__IO_CLASS__, super)

bool __IO_CLASS__::init(OSDictionary *dict)
{
    bool result = super::init(dict);
    IOLog("Initializing\n");
    return result;
}

void __IO_CLASS__::free(void)
{
    super::free();
    IOLog("Freeing\n");
}

IOService *__IO_CLASS__::probe(IOService *provider, SInt32 *score)
{
    IOService *result = super::probe(provider, score);
    IOLog("Probing\n");
    return result;
}

bool __IO_CLASS__::start(IOService *provider)
{
    bool result = super::start(provider);
    IOLog("Starting\n");
    return result;
}

void __IO_CLASS__::stop(IOService *provider)
{
    super::stop(provider);
    IOLog("Stopping\n");
}

