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
}

IModelComponent *CEQUALW2ComponentInfo::createComponentInstance()
{
  QString id =  QUuid::createUuid().toString();
  CEQUALW2Component *component = new CEQUALW2Component(id, this);
  component->setDescription("A CE-QUAL-W2 Model Instance");
  return component;
}
