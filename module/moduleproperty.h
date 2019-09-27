#ifndef MODULEPROPERTY
#define MODULEPROPERTY

#include <QtGlobal>
#include "moduleid.h"

namespace Protocol
{
typedef enum
{
    Protocol_Nbm,
    Protocol_A8,
    Protocol_Bm100a,
    Protocol_Cms,
    Protocol_Unknown,
} ProtocolType;
}

struct ModuleProperty
{
    ModuleProperty()
    {

    }

    bool operator == (const ModuleProperty &src)
    {
        return (this->device == src.device
                && this->channel == src.channel
                && this->index == src.index
                && this->labelId == src.labelId
                && this->moduleId == src.moduleId
                && this->physicalId == src.physicalId
                && this->enable == src.enable);
    }

    bool operator != (const ModuleProperty &src)
    {
        return !(*this == src);
    }

    uint device = 0;
    uint channel = 0;
    uint index = 0;
    uint physicalId = 0;
    ModuleID moduleId = MODULE_ID_NULL;
    LabelID labelId = LABEL_NULL;
    bool enable = false;
    Protocol::ProtocolType protocol = Protocol::Protocol_Unknown;
};

#endif // MODULEPROPERTY

