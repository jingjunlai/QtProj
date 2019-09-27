#ifndef ICMSDATA_H
#define ICMSDATA_H

#include "cmsmessageheader.h"
#include "mathtools.h"

class ICmsData
{
public:
    ICmsData()
    {

    }

    virtual ~ICmsData()
    {

    }

    virtual bool fromByteArray(const QByteArray &arr) = 0;

    virtual QByteArray toByteArray() const = 0;

    void setNeedResponse(bool response)
    {
        mNeedResponse = response;
    }

    bool needResponse() const {return mNeedResponse;}

protected:
    bool        mNeedResponse = false;
};


#endif // ICMSDATA_H
