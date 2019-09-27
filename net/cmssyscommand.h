#ifndef CMSSYSCOMMAND_H
#define CMSSYSCOMMAND_H


#include "cmscommanddata.h"

typedef enum
{
    male = 0,
    female,
    unknownSex
} CmsUserSex;

typedef enum
{
    adult = 0,
    child,
    neonate,
    unknownType,
    maxType
} CmsUserType;

struct UserInfo
{
    UserInfo()
    {
        sex = unknownSex;
        weight = 0;
        height = 0;
        type = unknownType;
    }
    CmsUserSex sex;
    int weight;
    int height;
    CmsUserType type;
};

struct SysCommandField
{
    UserInfo    user;
};

class CmsSysCommand : public CmsCommandData
{
public:
    CmsSysCommand(CmsModuleID identifier, CmsLabelID label);

    virtual ~CmsSysCommand();

    void setCommand(const SysCommandField &command);

    SysCommandField command() const;

protected:

    virtual bool decodePacket(const QByteArray &arr);

    virtual QByteArray encodePacket() const;

protected:

    SysCommandField     m_commandField;
};

Q_DECLARE_METATYPE(SysCommandField)

#endif // CMSSYSCOMMAND_H
