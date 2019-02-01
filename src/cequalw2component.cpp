/*!
   *  \file    CEQUALW2Component.cpp
   *  \author  Caleb Amoa Buahin <caleb.buahin@gmail.com>
   *  \version 1.0.0
   *  \section Description
   *  This file and its associated files and libraries are free software;
   *  you can redistribute it and/or modify it under the terms of the
   *  Lesser GNU Lesser General Public License as published by the Free Software Foundation;
   *  either version 3 of the License, or (at your option) any later version.
   *  fvhmcompopnent.h its associated files is distributed in the hope that it will be useful,
   *  but WITHOUT ANY WARRANTY; without even the implied warranty of
   *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.(see <http://www.gnu.org/licenses/> for details)
   *  \date 2018
   *  \pre
   *  \bug
   *  \todo
   *  \warning
   */

#include "stdafx.h"
#include "cequalw2component.h"

#include "core/dimension.h"
#include "core/valuedefinition.h"
#include "spatial/linestring.h"
#include "spatial/point.h"
#include "core/idbasedargument.h"
#include "progresschecker.h"
#include "temporal/timedata.h"
#include "core/abstractoutput.h"
#include "core/unit.h"
#include "core/unitdimensions.h"

using namespace HydroCouple;

CEQUALW2Component::CEQUALW2Component(const QString &id, CEQUALW2ComponentInfo *modelComponentInfo)
  : AbstractTimeModelComponent(id, modelComponentInfo),
    m_inputFilesArgument(nullptr),
    m_parent(nullptr)
{
//  m_timeDimension = new Dimension("TimeDimension",this);
//  m_geometryDimension = new Dimension("ElementGeometryDimension", this);

//  m_radiationFluxUnit = new Unit(this);
//  m_radiationFluxUnit->setCaption("Radiation Flux (W/m^2)");
//  m_radiationFluxUnit->setConversionFactorToSI(1.0);
//  m_radiationFluxUnit->setOffsetToSI(0.0);
//  m_radiationFluxUnit->dimensionsInternal()->setPower(HydroCouple::Mass, 1.0);
//  m_radiationFluxUnit->dimensionsInternal()->setPower(HydroCouple::Time, -3.0);

//  m_heatFluxUnit = new Unit(this);
//  m_heatFluxUnit->setCaption("Heat Source (W or J/s)");
//  m_heatFluxUnit->setConversionFactorToSI(1.0);
//  m_heatFluxUnit->setOffsetToSI(0.0);
//  m_heatFluxUnit->dimensionsInternal()->setPower(HydroCouple::Mass, 1.0);
//  m_heatFluxUnit->dimensionsInternal()->setPower(HydroCouple::Length, 2.0);
//  m_heatFluxUnit->dimensionsInternal()->setPower(HydroCouple::Time, -3.0);

//  m_temperatureUnit = new Unit(this);
//  m_temperatureUnit->setCaption("Temperature (°C)");
//  m_temperatureUnit->setConversionFactorToSI(1.0);
//  m_temperatureUnit->setOffsetToSI(273.15);
//  m_temperatureUnit->dimensionsInternal()->setPower(HydroCouple::Temperature, 1.0);

//  m_soluteFluxUnit = new Unit(this);
//  m_soluteFluxUnit->setCaption("Mass Flux (kg/s)");
//  m_soluteFluxUnit->dimensionsInternal()->setPower(HydroCouple::Mass, 1.0);
//  m_soluteFluxUnit->dimensionsInternal()->setPower(HydroCouple::Time, -1.0);

//  m_soluteUnit = new Unit(this);
//  m_soluteUnit->setCaption("Concentration (kg/m^3)");
//  m_soluteUnit->dimensionsInternal()->setPower(HydroCouple::Mass, 1.0);
//  m_soluteUnit->dimensionsInternal()->setPower(HydroCouple::Length, -3.0);

//  m_soluteConcQuantity = new Quantity(QVariant::Double, m_soluteUnit, this);
//  m_soluteConcFluxQuantity = new Quantity(QVariant::Double, m_soluteFluxUnit, this);

  createArguments();
}

CEQUALW2Component::~CEQUALW2Component()
{

  initializeFailureCleanUp();

  while (m_clones.size())
  {
    CEQUALW2Component *clone =  dynamic_cast<CEQUALW2Component*>(m_clones.first());
    removeClone(clone);
    delete clone;
  }

  if(m_parent)
  {
    m_parent->removeClone(this);
    m_parent = nullptr;
  }
}

QList<QString> CEQUALW2Component::validate()
{
  if(isInitialized())
  {
    setStatus(IModelComponent::Validating,"Validating...");

    //check connections

    setStatus(IModelComponent::Valid,"");
  }
  else
  {
    //throw has not been initialized yet.
  }

  return QList<QString>();
}

void CEQUALW2Component::prepare()
{
//  if(!isPrepared() && isInitialized() && m_modelInstance)
//  {
//    for(auto output :  outputsInternal())
//    {
//      for(auto adaptedOutput : output->adaptedOutputs())
//      {
//        adaptedOutput->initialize();
//      }
//    }

//    updateOutputValues(QList<HydroCouple::IOutput*>());

//    setStatus(IModelComponent::Updated ,"Finished preparing model");
//    setPrepared(true);
//  }
//  else
//  {
//    setPrepared(false);
//    setStatus(IModelComponent::Failed ,"Error occured when preparing model");
//  }
}

void CEQUALW2Component::update(const QList<HydroCouple::IOutput*> &requiredOutputs)
{
  if(status() == IModelComponent::Updated)
  {
//    setStatus(IModelComponent::Updating);

//    double minConsumerTime = std::max(m_modelInstance->currentDateTime(), getMinimumConsumerTime());

//    while (m_modelInstance->currentDateTime() <= minConsumerTime &&
//           m_modelInstance->currentDateTime() < m_modelInstance->endDateTime())
//    {
//      m_modelInstance->update();

//      if(progressChecker()->performStep(m_modelInstance->currentDateTime()))
//      {
//        setStatus(IModelComponent::Updated , "Simulation performed time-step | DateTime: " + QString::number(m_modelInstance->currentDateTime(), 'f') , progressChecker()->progress());
//      }
//    }

//    updateOutputValues(requiredOutputs);

//    currentDateTimeInternal()->setJulianDay(m_modelInstance->currentDateTime());

//    if(m_modelInstance->currentDateTime() >=  m_modelInstance->endDateTime())
//    {
//      setStatus(IModelComponent::Done , "Simulation finished successfully", 100);
//    }
//    else
//    {
//      if(progressChecker()->performStep(m_modelInstance->currentDateTime()))
//      {
//        setStatus(IModelComponent::Updated , "Simulation performed time-step | DateTime: " + QString::number(m_modelInstance->currentDateTime(), 'f') , progressChecker()->progress());
//      }
//      else
//      {
//        setStatus(IModelComponent::Updated);
//      }
//    }
  }
}

void CEQUALW2Component::finish()
{
//  if(isPrepared())
//  {
//    setStatus(IModelComponent::Finishing , "CEQUALW2Component with id " + id() + " is being disposed" , 100);

//    std::list<std::string> errors;
//    m_modelInstance->finalize(errors);
//    initializeFailureCleanUp();

//    setPrepared(false);
//    setInitialized(false);

//    setStatus(IModelComponent::Finished , "CEQUALW2Component with id " + id() + " has been disposed" , 100);
//    setStatus(IModelComponent::Created , "CEQUALW2Component with id " + id() + " ran successfully and has been re-created" , 100);
//  }
}

ICloneableModelComponent *CEQUALW2Component::parent() const
{
  return m_parent;
}

ICloneableModelComponent *CEQUALW2Component::clone()
{
  if(isInitialized())
  {
    //    CEQUALW2Component *cloneComponent = dynamic_cast<CEQUALW2Component*>(componentInfo()->createComponentInstance());
    //    cloneComponent->setReferenceDirectory(referenceDirectory());

    //    IdBasedArgumentString *identifierArg = identifierArgument();
    //    IdBasedArgumentString *cloneIndentifierArg = cloneComponent->identifierArgument();

    //    (*cloneIndentifierArg)["Id"] = QString((*identifierArg)["Id"]);
    //    (*cloneIndentifierArg)["Caption"] = QString((*identifierArg)["Caption"]);
    //    (*cloneIndentifierArg)["Description"] = QString((*identifierArg)["Description"]);

    //    QString appendName = "_clone_" + QString::number(m_clones.size()) + "_" + QUuid::createUuid().toString().replace("{","").replace("}","");

    //    //(*cloneComponent->m_inputFilesArgument)["Input File"] = QString((*m_inputFilesArgument)["Input File"]);

    //    QString inputFilePath = QString((*m_inputFilesArgument)["Input File"]);
    //    QFileInfo inputFile = getAbsoluteFilePath(inputFilePath);

    //    if(inputFile.absoluteDir().exists())
    //    {
    //      QString suffix = "." + inputFile.completeSuffix();
    //      inputFilePath = inputFile.absoluteFilePath().replace(suffix,"") + appendName + suffix;
    //      QFile::copy(inputFile.absoluteFilePath(), inputFilePath);
    //      (*cloneComponent->m_inputFilesArgument)["Input File"] = inputFilePath;
    //    }

    //    QString outputNetCDFFilePath = QString((*m_inputFilesArgument)["Output NetCDF File"]);
    //    QFileInfo outputNetCDFFile = getAbsoluteFilePath(outputNetCDFFilePath);

    //    if(!outputNetCDFFilePath.isEmpty() && outputNetCDFFile.absoluteDir().exists())
    //    {
    //      QString suffix = "." + outputNetCDFFile.completeSuffix();
    //      outputNetCDFFilePath = outputNetCDFFile.absoluteFilePath().replace(suffix,"") + appendName + suffix;
    //      (*cloneComponent->m_inputFilesArgument)["Output NetCDF File"] = outputNetCDFFilePath;
    //    }

    //    QString  outputCSVFilePath = QString((*m_inputFilesArgument)["Output CSV File"]);
    //    QFileInfo outputCSVFile = getAbsoluteFilePath(outputCSVFilePath);

    //    if(!outputCSVFilePath.isEmpty() && outputCSVFile.absoluteDir().exists())
    //    {
    //      QString suffix = "." + outputCSVFile.completeSuffix();
    //      outputCSVFilePath = outputCSVFile.absoluteFilePath().replace(suffix,"") + appendName + suffix;
    //      (*cloneComponent->m_inputFilesArgument)["Output CSV File"] = outputCSVFilePath;
    //    }


    //    cloneComponent->m_parent = this;
    //    m_clones.append(cloneComponent);

    //    emit propertyChanged("Clones");

    //    cloneComponent->initialize();

    //    return cloneComponent;
  }

  return nullptr;
}

QList<ICloneableModelComponent*> CEQUALW2Component::clones() const
{
  return m_clones;
}

bool CEQUALW2Component::removeClone(CEQUALW2Component *component)
{
  int removed;

#ifdef USE_OPENMP
#pragma omp critical (CEQUALW2Component)
#endif
  {
    removed = m_clones.removeAll(component);
  }


  if(removed)
  {
    emit propertyChanged("Clones");
  }

  return removed;
}

void CEQUALW2Component::initializeFailureCleanUp()
{
//  if(m_modelInstance)
//  {
//    delete m_modelInstance;
//    m_modelInstance = nullptr;
//  }

}

void CEQUALW2Component::createArguments()
{
  createInputFileArguments();
}

void CEQUALW2Component::createInputFileArguments()
{
  QStringList fidentifiers;
  fidentifiers.append("W2 Input File");

  Quantity *fquantity = Quantity::unitLessValues("InputFilesQuantity", QVariant::String, this);
  fquantity->setDefaultValue("");
  fquantity->setMissingValue("");

  Dimension *dimension = new Dimension("IdDimension","Dimension for identifiers",this);

  m_inputFilesArgument = new IdBasedArgumentString("InputFiles", fidentifiers, dimension, fquantity, this);
  m_inputFilesArgument->setCaption("Model Input Files");
  m_inputFilesArgument->addFileFilter("Input File (*.con)");
  m_inputFilesArgument->setMatchIdentifiersWhenReading(true);

  addArgument(m_inputFilesArgument);
}

bool CEQUALW2Component::initializeArguments(QString &message)
{
  bool initialized = initializeInputFilesArguments(message);

  if(initialized)
  {
    createGeometries();

    for(AbstractOutput *output : outputsInternal())
      output->updateValues();
  }
  else
  {
    initializeFailureCleanUp();
  }

  return initialized;
}

bool CEQUALW2Component::initializeInputFilesArguments(QString &message)
{
  QString inputFilePath = QString((*m_inputFilesArgument)["Input File"]);
  QFileInfo inputFile = getAbsoluteFilePath(inputFilePath);

  //  if(inputFile.exists())
  //  {
  //    initializeFailureCleanUp();

  //    m_modelInstance = new CSHModel(this);
  //    m_modelInstance->setInputFile(inputFile);

  //    QString netCDFOutput = QString((*m_inputFilesArgument)["Output NetCDF File"]);
  //    if(!netCDFOutput.isEmpty() && !netCDFOutput.isNull())
  //      m_modelInstance->setOutputNetCDFFile(QFileInfo(netCDFOutput));

  //    QString csvOutput = QString((*m_inputFilesArgument)["Output CSV File"]);
  //    if(!csvOutput.isEmpty() && !csvOutput.isNull())
  //      m_modelInstance->setOutputCSVFile(QFileInfo(csvOutput));

  //    std::list<std::string> errors;
  //    bool initialized = m_modelInstance->initialize(errors);

  //    for (std::string errorMsg : errors)
  //    {
  //      message += "/n" + QString::fromStdString(errorMsg);
  //    }

  //    if(initialized)
  //    {
  //      timeHorizonInternal()->setJulianDay(m_modelInstance->startDateTime());
  //      timeHorizonInternal()->setDuration(m_modelInstance->endDateTime() - m_modelInstance->startDateTime());
  //      currentDateTimeInternal()->setJulianDay(m_modelInstance->startDateTime());
  //      progressChecker()->reset(m_modelInstance->startDateTime(), m_modelInstance->endDateTime());
  //    }


  //    return initialized;
  //  }
  //  else
  //  {
  //    message = "Input file does not exist: " + inputFile.absoluteFilePath();
  //    return false;
  //  }

  return true;
}

void CEQUALW2Component::createGeometries()
{

}

void CEQUALW2Component::createInputs()
{

}

void CEQUALW2Component::createOutputs()
{

}

