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
#include "branchinput.h"
#include "temporal/timedata.h"
#include  "branchoutput.h"

#ifdef _WIN32 // note the underscore: without it, it's not msdn official!
#include <windows.h>
#include <sstream>
#else
#include "dlfcn.h"
#endif



using namespace HydroCouple;
using namespace std;

CEQUALW2Component::CEQUALW2Component(const QString &id, VOID_P libraryHandle, CEQUALW2ComponentInfo *modelComponentInfo)
    : AbstractTimeModelComponent(id, modelComponentInfo),
      m_inputFilesArgument(nullptr),
      m_parent(nullptr),
      m_libHandle(libraryHandle)
{
    m_timeDimension = new Dimension("TimeDimension",this);

    m_heatFluxUnit = new Unit(this);
    m_heatFluxUnit->setCaption("Heat Source (W or J/s)");
    m_heatFluxUnit->setConversionFactorToSI(1.0);
    m_heatFluxUnit->setOffsetToSI(0.0);
    m_heatFluxUnit->dimensionsInternal()->setPower(HydroCouple::Mass, 1.0);
    m_heatFluxUnit->dimensionsInternal()->setPower(HydroCouple::Length, 2.0);
    m_heatFluxUnit->dimensionsInternal()->setPower(HydroCouple::Time, -3.0);

    m_temperatureUnit = new Unit(this);
    m_temperatureUnit->setCaption("Temperature (°C)");
    m_temperatureUnit->setConversionFactorToSI(1.0);
    m_temperatureUnit->setOffsetToSI(273.15);
    m_temperatureUnit->dimensionsInternal()->setPower(HydroCouple::Temperature, 1.0);

    setFunctionPointers();
    createArguments();
}

CEQUALW2Component::~CEQUALW2Component()
{
    emit onAboutToDelete(this);

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

void CEQUALW2Component::setFunctionPointers()
{
#ifdef _WIN32

    if (m_libHandle != nullptr)
    {
        auto checkerror = [](void* handle)
        {
            if(handle == nullptr)
            {
                printf("Error: %s\n", getLastErrorAsString().c_str());
                return true;
            }

            return false;
        };

        m_initializeFunction = (CE_QUAL_W2_Initialize)(GetProcAddress((HMODULE)m_libHandle, "CE_QUAL_W2_Initialize"));
        checkerror(m_initializeFunction);

        m_updateFunction = (CE_QUAL_W2_Generic)(GetProcAddress((HMODULE)m_libHandle, "CE_QUAL_W2_Performstep"));
        checkerror(m_updateFunction);

        m_finalizeFunction = (CE_QUAL_W2_Generic)(GetProcAddress((HMODULE)m_libHandle, "CE_QUAL_W2_Dispose"));
        checkerror(m_finalizeFunction);

        m_prepareForUpdateFunction = (CE_QUAL_W2_Generic)(GetProcAddress((HMODULE)m_libHandle, "CE_QUAL_W2_PrepareForStep"));
        checkerror(m_prepareForUpdateFunction);

        m_getArrayFunction = (CE_QUAL_W2_GetArray)(GetProcAddress((HMODULE)m_libHandle, "CE_QUAL_W2_GetArray"));
        checkerror(m_getArrayFunction);
    }
    else
    {
        m_initializeFunction = nullptr;
        m_updateFunction = nullptr;
        m_finalizeFunction = nullptr;
        m_prepareForUpdateFunction = nullptr;
    }

#else

    if (m_libHandle)
    {
        m_initializeFunction = (CE_QUAL_W2_Initialize)(dlsym(m_libHandle, "CE_QUAL_W2_Initialize"));
        m_updateFunction = (CE_QUAL_W2_Generic)(dlsym(m_libHandle, "CE_QUAL_W2_Performstep"));
        m_finalizeFunction = (CE_QUAL_W2_Generic)(dlsym(m_libHandle, "CE_QUAL_W2_Dispose"));
        m_prepareForUpdateFunction = (CE_QUAL_W2_Generic)(dlsym(m_libHandle, "CE_QUAL_W2_PrepareForStep"));
        m_getArrayFunction = (CE_QUAL_W2_GetArray)(dlsym(m_libHandle, "CE_QUAL_W2_GetArray"));
    }
    else
    {
        m_initializeFunction = nullptr;
        m_updateFunction = nullptr;
        m_finalizeFunction = nullptr;
        m_prepareForUpdateFunction = nullptr;
    }
#endif
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
    if(!isPrepared() && isInitialized() && m_libHandle)
    {
        for(auto output :  outputsInternal())
        {
            for(auto adaptedOutput : output->adaptedOutputs())
            {
                adaptedOutput->initialize();
            }
        }

        updateOutputValues(QList<HydroCouple::IOutput*>());

        setStatus(IModelComponent::Updated ,"Finished preparing model");
        setPrepared(true);
    }
    else
    {
        setPrepared(false);
        setStatus(IModelComponent::Failed ,"Error occured when preparing model");
    }
}

void CEQUALW2Component::update(const QList<HydroCouple::IOutput*> &requiredOutputs)
{
    if(status() == IModelComponent::Updated)
    {
        setStatus(IModelComponent::Updating);

        double minConsumerTime = max(getCurrentJulianDateTime(), getMinimumConsumerTime());
        bool errorOccured = false;

        while (getCurrentJulianDateTime() <= minConsumerTime && !(*m_modelFinished))
        {
            if((*m_prepareForUpdateFunction)() == 0)
            {
                applyInputValues();

                if((*m_updateFunction)() == 0)
                {
                    if(progressChecker()->performStep(*m_currentDateTime))
                    {
                        setStatus(IModelComponent::Updated , "Simulation performed time-step | DateTime: " + QString::number(getCurrentJulianDateTime(), 'f') , progressChecker()->progress());

                    }

                    currentDateTimeInternal()->setJulianDay(getCurrentJulianDateTime());
                }
                else
                {
                    errorOccured = true;
                    break;
                }
            }
            else
            {
                errorOccured = true;
                break;
            }
        }

        updateOutputValues(requiredOutputs);

        currentDateTimeInternal()->setJulianDay(getCurrentJulianDateTime());

        if(errorOccured)
        {
            setStatus(IModelComponent::Failed,"An unknown error has occurred");
        }
        else if((*m_modelFinished) == true)
        {
            setStatus(IModelComponent::Done , "Simulation finished successfully", 100);
        }
        else
        {
            if(progressChecker()->performStep(*m_currentDateTime))
            {
                setStatus(IModelComponent::Updated , "Simulation performed time-step | DateTime: " + QString::number(getCurrentJulianDateTime(), 'f') , progressChecker()->progress());
            }
            else
            {
                setStatus(IModelComponent::Updated);
            }
        }
    }
}

void CEQUALW2Component::finish()
{
    if(isPrepared())
    {
        setStatus(IModelComponent::Finishing , "CEQUALW2Component with id " + id() + " is being disposed" , 100);

        std::list<std::string> errors;
        (*m_finalizeFunction)();
        initializeFailureCleanUp();

        setPrepared(false);
        setInitialized(false);

        setStatus(IModelComponent::Finished , "CEQUALW2Component with id " + id() + " has been disposed" , 100);
        setStatus(IModelComponent::Created , "CEQUALW2Component with id " + id() + " ran successfully and has been re-created" , 100);
    }
}

ICloneableModelComponent* CEQUALW2Component::parent() const
{
    return m_parent;
}

ICloneableModelComponent* CEQUALW2Component::clone()
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

#ifdef _WIN32

std::wstring CEQUALW2Component::utf8toUtf16(const std::string & str)
{
   if (str.empty())
      return wstring();

   size_t charsNeeded = ::MultiByteToWideChar(CP_UTF8, 0,
      str.data(), (int)str.size(), NULL, 0);
   if (charsNeeded == 0)
      throw runtime_error("Failed converting UTF-8 string to UTF-16");

   vector<wchar_t> buffer(charsNeeded);
   int charsConverted = ::MultiByteToWideChar(CP_UTF8, 0,
      str.data(), (int)str.size(), &buffer[0], buffer.size());
   if (charsConverted == 0)
      throw runtime_error("Failed converting UTF-8 string to UTF-16");

   return std::wstring(&buffer[0], charsConverted);
}

std::string CEQUALW2Component::getLastErrorAsString()
{
    //Get the error message, if any.
    DWORD errorMessageID = GetLastError();
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

void CEQUALW2Component::createArguments()
{
    createInputFileArguments();
}

void CEQUALW2Component::createInputFileArguments()
{
    QStringList fidentifiers;
    fidentifiers.append("W2 Control File");

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
    QString inputFilePath = QString((*m_inputFilesArgument)["W2 Control File"]);
    QFileInfo inputFile = getAbsoluteFilePath(inputFilePath);

    if(inputFile.dir().exists())
    {
        initializeFailureCleanUp();

        if(m_libHandle == nullptr)
        {
            message = "CE-QUAL-W2 library could not be found";
            return false;
        }
        else
        {
            if(m_initializeFunction == nullptr)
            {
                message = "Pointer to initialize function in CE-QUAL-W2 library could not be loaded";
                return false;
            }
            else if(m_prepareForUpdateFunction == nullptr)
            {
                message = "Pointer to prepare for step function in CE-QUAL-W2 library could not be loaded";
                return false;
            }
            else if(m_updateFunction == nullptr)
            {
                message = "Pointer to update function in CE-QUAL-W2 library could not be loaded";
                return false;
            }
            else if(m_finalizeFunction == nullptr)
            {
                message = "Pointer to function finalize in CE-QUAL-W2 library could not be loaded";
                return false;
            }
        }

        int initialized = (*m_initializeFunction)(inputFile.dir().absolutePath().toStdString().c_str());

#ifdef _WIN32 // note the underscore: without it, it's not msdn official!

        m_startYear = (int*)GetProcAddress((HMODULE)m_libHandle, "GDAYC_YEAR");
        m_startDateTime = (double*)GetProcAddress((HMODULE)m_libHandle, "MAIN_TMSTRT");
        m_endDateTime = (double*)GetProcAddress((HMODULE)m_libHandle, "MAIN_TMEND");
        m_currentDateTime = (double*)GetProcAddress((HMODULE)m_libHandle, "SCREENC_JDAY");
        m_modelFinished  = (bool*)GetProcAddress((HMODULE)m_libHandle, "MAIN_END_RUN");

        m_NWB = (int*)GetProcAddress((HMODULE)m_libHandle,"GLOBAL_NWB");
        m_NBR = (int*)GetProcAddress((HMODULE)m_libHandle,"GLOBAL_NBR");
        m_KMX = (int*)GetProcAddress((HMODULE)m_libHandle,"GLOBAL_KMX");
        m_IMX = (int*)GetProcAddress((HMODULE)m_libHandle,"GLOBAL_IMX");
        m_NTR = (int*)GetProcAddress((HMODULE)m_libHandle,"GLOBAL_NTR");
        m_NST = (int*)GetProcAddress((HMODULE)m_libHandle,"GLOBAL_NST");
        m_NWD = (int*)GetProcAddress((HMODULE)m_libHandle,"GLOBAL_NWD");
#else
        m_startYear = (int*)dlsym(m_libHandle, "GDAYC_YEAR");
        m_startDateTime = (double*)dlsym(m_libHandle, "MAIN_TMSTRT");
        m_endDateTime = (double*)dlsym(m_libHandle, "MAIN_TMEND");
        m_currentDateTime = (double*)dlsym(m_libHandle, "SCREENC_JDAY");
        m_modelFinished  = (bool*)dlsym(m_libHandle, "MAIN_END_RUN");

        m_NWB = (int*)dlsym(m_libHandle,"GLOBAL_NWB");
        m_NBR = (int*)dlsym(m_libHandle,"GLOBAL_NBR");
        m_KMX = (int*)dlsym(m_libHandle,"GLOBAL_KMX");
        m_IMX = (int*)dlsym(m_libHandle,"GLOBAL_IMX");
        m_NTR = (int*)dlsym(m_libHandle,"GLOBAL_NTR");
        m_NST = (int*)dlsym(m_libHandle,"GLOBAL_NST");
        m_NWD = (int*)dlsym(m_libHandle,"GLOBAL_NWD");

#endif

        int key = 0;
        void *d0 = nullptr;
        (*m_getArrayFunction)(&key, &d0);
        m_NSTR = (int*)d0;

        void *d1 = nullptr;
        key = 1;
        (*m_getArrayFunction)(&key, &d1);
        m_QSTR = (double*)d1;

        void *d2 = nullptr;
        key = 2;
        (*m_getArrayFunction)(&key, &d2);
        m_TaveSTR = (double*)d2;

        void *d3 = nullptr;
        key = 3;
        (*m_getArrayFunction)(&key, &d3);
        m_QIND = (double*)d3;

        void *d4 = nullptr;
        key = 4;
        (*m_getArrayFunction)(&key, &d4);
        m_TIND = (double*)d4;

        if(initialized > 0)
        {
            return  false;
        }
        else
        {
            int year = (*m_startYear);

            double startime = (*m_startDateTime);
            double endtime = (*m_endDateTime);
            double diff = endtime - startime;
            QDate date = QDate(year - 1, 12, 31);
            QDateTime dateTime = QDateTime(date, QTime(), Qt::UTC).addSecs(startime * 86400);
            m_startJulianDate = SDKTemporal::DateTime::toJulianDays(dateTime);
            timeHorizonInternal()->setJulianDay(m_startJulianDate);
            timeHorizonInternal()->setDuration(diff);
            currentDateTimeInternal()->setJulianDay(m_startJulianDate);
            progressChecker()->reset(startime, endtime);
        }
    }
    else
    {
        message = "Model input file directory does not exist: " + inputFile.absoluteFilePath();
        return false;
    }

    return true;
}

void CEQUALW2Component::createInputs()
{
    QStringList branchIdentifiers;

    for (int i = 1; i <= (*m_NBR); i++)
    {
        QString id = "BR" + QString::number(i);
        branchIdentifiers.push_back(id);
    }

    Quantity *flowQuantity = Quantity::flowInCMS(this);
    Quantity *temperatureQuantity = new Quantity(QVariant::Double, m_temperatureUnit, this);
    Dimension *branchIdentifiersDimension = new Dimension("BranchIdentifiersDimension",this);

    CEQUALW2BranchInput *branchInflows = new CEQUALW2BranchInput("BranchInflows", branchIdentifiers, CEQUALW2BranchInput::Inflow,
                                                 branchIdentifiersDimension, m_timeDimension,  flowQuantity, this);
    branchInflows->setCaption("Branch Inflow (m^3/s)");

    CEQUALW2BranchInput *branchInflowTemp = new CEQUALW2BranchInput("BranchInflowTemperatures", branchIdentifiers, CEQUALW2BranchInput::Temperature,
                                                    branchIdentifiersDimension, m_timeDimension,  temperatureQuantity, this);
    branchInflowTemp->setCaption("Branch Inflow Temperature (ºC)");

    addInput(branchInflows);
    addInput(branchInflowTemp);

}

void CEQUALW2Component::createOutputs()
{
    QStringList branchIdentifiers;

    for (int i = 1; i <= (*m_NBR); i++)
    {
        QString id = "BR" + QString::number(i);
        branchIdentifiers.push_back(id);
    }

    Quantity *flowQuantity = Quantity::flowInCMS(this);
    Quantity *heatQuantity = new Quantity(QVariant::Double, m_heatFluxUnit, this);
    Dimension *branchIdentifiersDimension = new Dimension("BranchIdentifiersDimension",this);

    CEQUALW2BranchOutput *flowOutput = new CEQUALW2BranchOutput("OutletStructuresFlow",
                                                branchIdentifiersDimension,
                                                branchIdentifiers,
                                                m_timeDimension,
                                                CEQUALW2BranchOutput::Flow,
                                                flowQuantity,
                                                this);
    flowOutput->setCaption("Flow From Outlet Structures (m^3/s)");


    CEQUALW2BranchOutput *heatOutput = new CEQUALW2BranchOutput("OutletStructuresHeat",
                                                branchIdentifiersDimension,
                                                branchIdentifiers,
                                                m_timeDimension,
                                                CEQUALW2BranchOutput::Heat,
                                                heatQuantity,
                                                this);
    heatOutput->setCaption("Heat From Outlet Structures (J/s)");

    addOutput(flowOutput);
    addOutput(heatOutput);
}

double CEQUALW2Component::getCurrentJulianDateTime()
{
    return  m_startJulianDate + ((*m_currentDateTime)-(*m_startDateTime));
}
