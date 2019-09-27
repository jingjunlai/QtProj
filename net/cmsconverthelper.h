#ifndef CMSCONVERTHELPER_H
#define CMSCONVERTHELPER_H


#include "module/moduleid.h"
#include "cmstype.h"

class CmsConvertHelper
{
public:
    CmsConvertHelper();

    static CmsModuleID convertToCmsModuleId(ModuleID id);

    static ModuleID convertToLocalModuleId(CmsModuleID id);

    static CmsLabelID convertToCmsLabelId(LabelID label);

    static LabelID convertToLocalLabelId(CmsLabelID label);

private:
    void initialize();

private:
    static QMap<CmsLabelID, LabelID> mLabelIdMap;
    static QMap<CmsModuleID, ModuleID> mModuleIdMap;
};

#endif // CMSCONVERTHELPER_H
