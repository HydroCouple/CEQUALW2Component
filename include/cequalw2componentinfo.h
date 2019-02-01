#ifndef CEQUALW2COMPONENTINFO_H
#define CEQUALW2COMPONENTINFO_H


#include "cequalw2component_global.h"
#include "core/abstractmodelcomponentinfo.h"


class CEQUALW2COMPONENT_EXPORT CEQUALW2ComponentInfo : public AbstractModelComponentInfo
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "CEQUALW2ComponentInfo")

  public:

    CEQUALW2ComponentInfo(QObject *parent = nullptr);

    virtual ~CEQUALW2ComponentInfo();

    HydroCouple::IModelComponent* createComponentInstance() override;
};

Q_DECLARE_METATYPE(CEQUALW2ComponentInfo*)


#endif // CEQUALW2COMPONENTINFO_H
