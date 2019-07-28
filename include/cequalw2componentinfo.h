#ifndef CEQUALW2COMPONENTINFO_H
#define CEQUALW2COMPONENTINFO_H

#ifdef _WIN32 // note the underscore: without it, it's not msdn official!
#include <windows.h>
#else
#include "dlfcn.h"
#endif

#include "cequalw2component_global.h"
#include "core/abstractmodelcomponentinfo.h"

typedef double (*AddFunction)(double *value);
typedef void* VOID_P;

class CEQUALW2Component;

class CEQUALW2COMPONENT_EXPORT CEQUALW2ComponentInfo : public AbstractModelComponentInfo
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "CEQUALW2ComponentInfo")

  public:

    CEQUALW2ComponentInfo(QObject *parent = nullptr);

    virtual ~CEQUALW2ComponentInfo() override;

    HydroCouple::IModelComponent* createComponentInstance() override;

#ifdef _WIN32

    static std::wstring utf8toUtf16(const std::string & str);

    static std::string getLastErrorAsString();

#endif

  private slots:

    void onComponentDeleting(CEQUALW2Component *component);

    void deleteAllFilesAndHandles();

  private:

    QHash<CEQUALW2Component*, QPair<QString,VOID_P>> m_copiedLibraries;
};

Q_DECLARE_METATYPE(CEQUALW2ComponentInfo*)


#endif // CEQUALW2COMPONENTINFO_H
