#include "stdafx.h"
#include "cequalw2componentinfo.h"
#include "spatial/geometryfactory.h"
#include "cequalw2component.h"




using namespace HydroCouple;

CEQUALW2ComponentInfo::CEQUALW2ComponentInfo(QObject *parent)
  :AbstractModelComponentInfo(parent)
{
  GeometryFactory::registerGDAL();

  setId("A CE-QUAL-W2 Component");
  setCaption("CE-QUAL-W2 Component");
  setIconFilePath(":/CEQUALW2Component/cequalw2componenticon");
  setDescription("A component for the laterally average CE-QUAL-W2 Hydrodynamic Model");

  setCategory("Hydrodyanmics\\Heat & Solute Transport");
  setCopyright("");
  setVendor("");
  setUrl("www.hydrocouple.org");
  setEmail("caleb.buahin@gmail.com");
  setVersion("1.0.0");

  QStringList documentation;
  documentation << "Several sources";
  setDocumentation(documentation);

}

CEQUALW2ComponentInfo::~CEQUALW2ComponentInfo()
{
  deleteAllFilesAndHandles();
}

IModelComponent *CEQUALW2ComponentInfo::createComponentInstance()
{
  QFileInfo filePath(libraryFilePath());
  QString extension = filePath.suffix();

#ifdef _WIN32 // note the underscore: without it, it's not msdn official!
  QString lib = filePath.dir().absolutePath() + "/CE_QUAL_W2.4.1.0.dll";
#elif __unix__ // all unices, not all compilers
  QString lib = filePath.dir().absolutePath() + "/CE_QUAL_W2.so.4.1.0";
#elif __linux__
  QString lib = filePath.dir().absolutePath() + "/CE_QUAL_W2.so.4.1.0";
#elif __APPLE__
  QString lib = filePath.dir().absolutePath() + "/CE_QUAL_W2.4.1.0.dylib";
#endif

  QFileInfo libInfo(lib);

  if(libInfo.exists() && libInfo.isFile())
  {
    //Initialize component with handle to library
    QString libCopy = filePath.dir().absolutePath() + "/" + QUuid::createUuid().toString().replace("{","").replace("}","") + filePath.fileName();
    QFile::copy(lib, libCopy);

#ifdef _WIN32 // note the underscore: without it, it's not msdn official!
    void *libHandle = LoadLibrary(lib.toStdString().c_str());

    if(!libHandle)
    {
      printf("Error: %s\n", getLastErrorAsString().c_str());
    }
#else

    void *libHandle = dlopen(libCopy.toStdString().c_str(), RTLD_LAZY|RTLD_LOCAL);

    if (libHandle == nullptr)
    {
      /* fail to load the library */
      printf("Error: %s\n", dlerror());
      return nullptr;
    }
#endif

    QString id =  QUuid::createUuid().toString();
    CEQUALW2Component *component = new CEQUALW2Component(id, libHandle, this);
    component->setDescription("A CE-QUAL-W2 Model Instance");
    m_copiedLibraries[component] = QPair<QString,void*>(libCopy, libHandle);

    connect(component,&CEQUALW2Component::onAboutToDelete, this, &CEQUALW2ComponentInfo::onComponentDeleting);

    return component;
  }
  else
  {
    return nullptr;
  }
}

#ifdef _WIN32 // note the underscore: without it, it's not msdn official!
std::string CEQUALW2ComponentInfo::getLastErrorAsString()
{
  //Get the error message, if any.
  DWORD errorMessageID = ::GetLastError();
  if(errorMessageID == 0)
    return std::string(); //No error message has been recorded

  LPSTR messageBuffer = nullptr;
  size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                               NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

  std::string message(messageBuffer, size);

  //Free the buffer.
  LocalFree(messageBuffer);

  return message;
}
#endif

void CEQUALW2ComponentInfo::onComponentDeleting(CEQUALW2Component *component)
{
  if(m_copiedLibraries.contains(component))
  {

#ifdef _WIN32 // note the underscore: without it, it's not msdn official!

    QPair<QString,void*> pair;
    if(pair.second != nullptr)
    {
      BOOL result = FreeLibrary(pair.second);
    }

#else
    QPair<QString,void*> pair;
    if(pair.second != nullptr)
    {
      int result = dlclose(pair.second);
    }
#endif

    QFileInfo file(pair.first);
    if(file.isFile() && file.exists())
    {
      file.dir().remove(file.fileName());
    }

    m_copiedLibraries.remove(component);
  }
}

void CEQUALW2ComponentInfo::deleteAllFilesAndHandles()
{
  QHash<CEQUALW2Component*, QPair<QString, void*>>::iterator it;

  for(it = m_copiedLibraries.begin(); it != m_copiedLibraries.end(); ++it)
  {
    QFileInfo file(it.value().first);

#ifdef _WIN32 // note the underscore: without it, it's not msdn official!

    if(it.value().second != nullptr)
    {
      BOOL result = FreeLibrary(it.value().second);
    }

#else
    if(it.value().second != nullptr)
    {
      int result = dlclose(it.value().second);
    }
#endif

    if(file.isFile() && file.exists())
    {
      file.dir().remove(file.fileName());
    }
  }
}
