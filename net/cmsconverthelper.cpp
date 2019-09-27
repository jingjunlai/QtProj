#include "cmsconverthelper.h"

#include <QMap>
#include <QDate>

QMap<CmsLabelID, LabelID> CmsConvertHelper::mLabelIdMap;
QMap<CmsModuleID, ModuleID> CmsConvertHelper::mModuleIdMap;

static CmsConvertHelper dummy;

CmsConvertHelper::CmsConvertHelper()
{
    initialize();
}

void CmsConvertHelper::initialize()
{
    mLabelIdMap.insert(CMS_LabelEcg, LABEL_ECG);
    mLabelIdMap.insert(CMS_LabelSpo2, LABEL_SPO2);
    mLabelIdMap.insert(CMS_LabelNibp, LABEL_NIBP);
    mLabelIdMap.insert(CMS_LabelResp, LABEL_RESP);
    mLabelIdMap.insert(CMS_LabelCo2, LABEL_CO2);
    mLabelIdMap.insert(CMS_LabelIcg, LABEL_ICG);
    mLabelIdMap.insert(CMS_LabelBis, LABEL_BIS);
    mLabelIdMap.insert(CMS_LabelAa, LABEL_AA);
    mLabelIdMap.insert(CMS_LabelO2, LABEL_O2);
    mLabelIdMap.insert(CMS_LabelN2o, LABEL_N2O);
    mLabelIdMap.insert(CMS_LabelPr, LABEL_PR);
    mLabelIdMap.insert(CMS_LabelCo, LABEL_CO);

    mLabelIdMap.insert(CMS_LabelIbpP1, LABEL_IBP_P1);
    mLabelIdMap.insert(CMS_LabelIbpP2, LABEL_IBP_P2);
    mLabelIdMap.insert(CMS_LabelIbpArt, LABEL_IBP_ART);
    mLabelIdMap.insert(CMS_LabelIbpCvp, LABEL_IBP_CVP);
    mLabelIdMap.insert(CMS_LabelIbpPa, LABEL_IBP_PA);
    mLabelIdMap.insert(CMS_LabelIbpRap, LABEL_IBP_RAP);
    mLabelIdMap.insert(CMS_LabelIbpLap, LABEL_IBP_LAP);
    mLabelIdMap.insert(CMS_LabelIbpIcp, LABEL_IBP_ICP);
    mLabelIdMap.insert(CMS_LabelIbpPawp, LABEL_IBP_PAWP);
    mLabelIdMap.insert(CMS_LabelIbpUap, LABEL_IBP_UAP);
    mLabelIdMap.insert(CMS_LabelIbpBap, LABEL_IBP_BAP);
    mLabelIdMap.insert(CMS_LabelIbpFap, LABEL_IBP_FAP);
    mLabelIdMap.insert(CMS_LabelIbpUvp, LABEL_IBP_UVP);
    mLabelIdMap.insert(CMS_LabelIbpLv, LABEL_IBP_LV);
    mLabelIdMap.insert(CMS_LabelIbpAo, LABEL_IBP_AO);

    mLabelIdMap.insert(CMS_LabelTof, LABEL_TOF);
    mLabelIdMap.insert(CMS_LabelDm, LABEL_DM);

    mLabelIdMap.insert(CMS_LabelTempTA, LABEL_TEMP_A);
    mLabelIdMap.insert(CMS_LabelTempTB, LABEL_TEMP_B);
    mLabelIdMap.insert(CMS_LabelTempTC, LABEL_TEMP_C);
    mLabelIdMap.insert(CMS_LabelTempTD, LABEL_TEMP_D);
    mLabelIdMap.insert(CMS_LabelTempTE, LABEL_TEMP_E);
    mLabelIdMap.insert(CMS_LabelMachine, LABEL_SYSTEM);

    mModuleIdMap.insert(CMS_ModuleEcg, MODULE_ID_ECG);
    mModuleIdMap.insert(CMS_ModuleSpo2, MODULE_ID_SPO2);
    mModuleIdMap.insert(CMS_ModuleResp, MODULE_ID_RESP);
    mModuleIdMap.insert(CMS_ModuleNibp, MODULE_ID_NIBP);
    mModuleIdMap.insert(CMS_ModuleIbp, MODULE_ID_IBP);
    mModuleIdMap.insert(CMS_ModuleTemp, MODULE_ID_TEMP);
    mModuleIdMap.insert(CMS_ModuleCo2, MODULE_ID_CO2);
    mModuleIdMap.insert(CMS_ModuleO2, MODULE_ID_N2O);
    mModuleIdMap.insert(CMS_ModuleN2o, MODULE_ID_N2O);
    mModuleIdMap.insert(CMS_ModuleAa, MODULE_ID_AA);
    mModuleIdMap.insert(CMS_ModuleBis, MODULE_ID_BIS);
    mModuleIdMap.insert(CMS_ModuleCo, MODULE_ID_CO);
    mModuleIdMap.insert(CMS_ModuleRm, MODULE_ID_RM);
    mModuleIdMap.insert(CMS_ModuleDm, MODULE_ID_DM);
}

CmsModuleID CmsConvertHelper::convertToCmsModuleId(ModuleID id)
{
    CmsModuleID to = CMS_ModuleNull;
    auto it = mModuleIdMap.begin();
    while (it != mModuleIdMap.end())
    {
        if (it.value() == id)
        {
            to = it.key();
            break;
        }

        it++;
    }

    return to;
}

ModuleID CmsConvertHelper::convertToLocalModuleId(CmsModuleID id)
{
    ModuleID to = mModuleIdMap.value(id, MODULE_ID_NULL);
    return to;
}

CmsLabelID CmsConvertHelper::convertToCmsLabelId(LabelID label)
{
    CmsLabelID to = CMS_LabelNull;
    auto it = mLabelIdMap.begin();
    while (it != mLabelIdMap.end())
    {
        if (it.value() == label)
        {
            to = it.key();
            break;
        }

        it++;
    }

    return to;
}

LabelID CmsConvertHelper::convertToLocalLabelId(CmsLabelID label)
{
    LabelID to = mLabelIdMap.value(label, LABEL_NULL);
    return to;
}

