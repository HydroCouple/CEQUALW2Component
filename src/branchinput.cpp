#include "stdafx.h"
#include "branchinput.h"
#include "cequalw2component.h"
#include "core/dimension.h"
#include "core/valuedefinition.h"
#include "hydrocouplespatial.h"
#include "hydrocoupletemporal.h"
#include "hydrocouplespatiotemporal.h"

#include "temporal/timedata.h"

using namespace SDKTemporal;

using namespace HydroCouple;
using namespace HydroCouple::Spatial;
using namespace HydroCouple::Temporal;
using namespace HydroCouple::SpatioTemporal;

BranchInput::BranchInput(const QString &inputId,
                         const QStringList& identifiers,
                         InputType inputType,
                         Dimension *identifierDimension,
                         Dimension *timeDimension,
                         Quantity *quantity,
                         CEQUALW2Component *cequalComponent):
  TimeSeriesIdBasedInputDouble(inputId, identifierDimension, timeDimension, quantity, cequalComponent),
  m_inputType(inputType),
  m_cequalComponent(cequalComponent)
{
  DateTime *dt1 = new DateTime(0, this);
  addTime(dt1);

  DateTime *dt2 = new DateTime(0.1, this);
  addTime(dt2);

  timeInternal(0)->setJulianDay(cequalComponent->currentDateTime()->julianDay() - 0.000000000001);
  timeInternal(1)->setJulianDay(cequalComponent->currentDateTime()->julianDay());

  addIdentifiers(identifiers);
}

BranchInput::~BranchInput()
{

}

bool BranchInput::setProvider(HydroCouple::IOutput *provider)
{
  m_idMapping.clear();

  if(AbstractInput::setProvider(provider) && provider)
  {
    ITimeGeometryComponentDataItem *timeGeometryDataItem = nullptr;
    IGeometryComponentDataItem *geometryDataItem = nullptr;
    IIdBasedComponentDataItem *idbasedDataItem = nullptr;
    ITimeIdBasedComponentDataItem *timeIdBasedDataItem = nullptr;

    QStringList ids = identifiers();

    if((timeGeometryDataItem = dynamic_cast<ITimeGeometryComponentDataItem*>(provider)))
    {
      if(timeGeometryDataItem->geometryCount())
      {
        for(int i = 0; i < timeGeometryDataItem->geometryCount(); i++)
        {
          IGeometry *providerGeometry = timeGeometryDataItem->geometry(i);

          for(int j = 0 ; j < ids.size(); j++)
          {
            QString id = ids[j];

            if(!QString::compare(providerGeometry->id(), id))
            {
              m_idMapping[i] = j;
              break;
            }
          }
        }
      }
    }
    else if((geometryDataItem = dynamic_cast<IGeometryComponentDataItem*>(provider)))
    {
      if(geometryDataItem->geometryCount())
      {
        for(int i = 0; i < geometryDataItem->geometryCount(); i++)
        {
          IGeometry *providerGeometry = geometryDataItem->geometry(i);

          for(int j = 0 ; j < ids.size(); j++)
          {
            QString id = ids[j];

            if(!QString::compare(providerGeometry->id(), id))
            {
              m_idMapping[i] = j;
              break;
            }
          }
        }
      }
    }
    else if((idbasedDataItem = dynamic_cast<IIdBasedComponentDataItem*>(provider)))
    {
      QStringList pIds = idbasedDataItem->identifiers();

      if(pIds.size())
      {
        for(int i = 0; i < pIds.size(); i++)
        {
          QString pId = pIds[i];

          for(int j = 0 ; j < ids.size(); j++)
          {
            QString id = ids[j];

            if(!QString::compare(pId, id))
            {
              m_idMapping[i] = j;
              break;
            }
          }
        }
      }
    }
    else if((timeIdBasedDataItem = dynamic_cast<ITimeIdBasedComponentDataItem*>(provider)))
    {
      QStringList pIds = idbasedDataItem->identifiers();

      if(pIds.size())
      {
        for(int i = 0; i < pIds.size(); i++)
        {
          QString pId = pIds[i];

          for(int j = 0 ; j < ids.size(); j++)
          {
            QString id = ids[j];

            if(!QString::compare(pId, id))
            {
              m_idMapping[i] = j;
              break;
            }
          }
        }
      }
    }

    return true;
  }

  return false;
}

bool BranchInput::canConsume(HydroCouple::IOutput *provider, QString &message) const
{
  message = "";

  ITimeGeometryComponentDataItem *timeGeometryDataItem = nullptr;
  IGeometryComponentDataItem *geometryDataItem = nullptr;
  IIdBasedComponentDataItem *idbasedDataItem = nullptr;
  ITimeIdBasedComponentDataItem *timeIdBasedDataItem = nullptr;

  if((timeGeometryDataItem = dynamic_cast<ITimeGeometryComponentDataItem*>(provider)))
  {
    return true;
  }
  else if((geometryDataItem = dynamic_cast<IGeometryComponentDataItem*>(provider)))
  {
    return true;
  }
  else if((idbasedDataItem = dynamic_cast<IIdBasedComponentDataItem*>(provider)))
  {
    return true;
  }
  else if((timeIdBasedDataItem = dynamic_cast<ITimeIdBasedComponentDataItem*>(provider)))
  {
    return true;
  }

  return false;
}

void BranchInput::retrieveValuesFromProvider()
{
  moveDataToPrevTime();
  int currentTimeIndex = timesInternal().size() - 1;
  timesInternal()[currentTimeIndex]->setJulianDay(m_cequalComponent->currentDateTime()->julianDay());
  provider()->updateValues(this);
}

void BranchInput::applyData()
{
  ITimeGeometryComponentDataItem *timeGeometryDataItem = nullptr;
  IGeometryComponentDataItem *geometryDataItem = nullptr;
  IIdBasedComponentDataItem *idbasedDataItem = nullptr;
  ITimeIdBasedComponentDataItem *timeIdBasedDataItem = nullptr;

  if((timeGeometryDataItem = dynamic_cast<ITimeGeometryComponentDataItem*>(provider())))
  {
    double currentTime = m_cequalComponent->currentDateTime()->julianDay();

    int currentTimeIndex = timeGeometryDataItem->timeCount() - 1;
    int previousTimeIndex = std::max(0 , timeGeometryDataItem->timeCount() - 2);

    double providerCurrentTime = timeGeometryDataItem->time(currentTimeIndex)->julianDay();
    double providerPreviousTime = timeGeometryDataItem->time(previousTimeIndex)->julianDay();

    if(currentTime >=  providerPreviousTime && currentTime <= providerCurrentTime)
    {
      double factor = 0.0;

      if(providerCurrentTime > providerPreviousTime)
      {
        double denom = providerCurrentTime - providerPreviousTime;
        double numer =currentTime - providerPreviousTime;
        factor = numer / denom;
      }

      switch (m_inputType)
      {
        case Inflow:
          {
            for(auto it : m_idMapping)
            {
              double value1 = 0;
              double value2 = 0;

              timeGeometryDataItem->getValue(currentTimeIndex,it.second, &value1);
              timeGeometryDataItem->getValue(previousTimeIndex,it.second, &value2);
              m_cequalComponent->m_QIND[it.first] = value2 + factor *(value1 - value2);
            }
          }
          break;
        case Temperature:
          {
            for(auto it : m_idMapping)
            {
              double value1 = 0;
              double value2 = 0;

              timeGeometryDataItem->getValue(currentTimeIndex,it.second, &value1);
              timeGeometryDataItem->getValue(previousTimeIndex,it.second, &value2);
              m_cequalComponent->m_TIND[it.first] = value2 + factor *(value1 - value2);
            }
          }
          break;
      }
    }
    else
    {
      switch (m_inputType)
      {
        case Inflow:
          {
            for(auto it : m_idMapping)
            {
              double value = 0;
              timeGeometryDataItem->getValue(currentTimeIndex,it.second, & value);
              m_cequalComponent->m_QIND[it.first] = value;
            }
          }
          break;
        case Temperature:
          {
            for(auto it : m_idMapping)
            {
              double value = 0;
              timeGeometryDataItem->getValue(currentTimeIndex,it.second, & value);
              m_cequalComponent->m_TIND[it.first] = value;
            }
          }
          break;
      }
    }
  }
  else if((timeIdBasedDataItem = dynamic_cast<ITimeIdBasedComponentDataItem*>(provider())))
  {
    double currentTime = m_cequalComponent->currentDateTime()->julianDay();

    int currentTimeIndex = timeIdBasedDataItem->timeCount() - 1;
    int previousTimeIndex = std::max(0 , timeIdBasedDataItem->timeCount() - 2);

    double providerCurrentTime = timeIdBasedDataItem->time(currentTimeIndex)->julianDay();
    double providerPreviousTime = timeIdBasedDataItem->time(previousTimeIndex)->julianDay();

    if(currentTime >=  providerPreviousTime && currentTime <= providerCurrentTime)
    {
      double factor = 0.0;

      if(providerCurrentTime > providerPreviousTime)
      {
        double denom = providerCurrentTime - providerPreviousTime;
        double numer =currentTime - providerPreviousTime;
        factor = numer / denom;
      }

      switch (m_inputType)
      {
        case Inflow:
          {
            for(auto it : m_idMapping)
            {
              double value1 = 0;
              double value2 = 0;

              timeIdBasedDataItem->getValue(currentTimeIndex,it.second, &value1);
              timeIdBasedDataItem->getValue(previousTimeIndex,it.second, &value2);
              m_cequalComponent->m_QIND[it.first] = value2 + factor *(value1 - value2);
            }
          }
          break;
        case Temperature:
          {
            for(auto it : m_idMapping)
            {
              double value1 = 0;
              double value2 = 0;

              timeIdBasedDataItem->getValue(currentTimeIndex,it.second, &value1);
              timeIdBasedDataItem->getValue(previousTimeIndex,it.second, &value2);
              m_cequalComponent->m_TIND[it.first] = value2 + factor *(value1 - value2);
            }
          }
          break;
      }
    }
    else
    {
      switch (m_inputType)
      {
        case Inflow:
          {
            for(auto it : m_idMapping)
            {
              double value = 0;
              timeIdBasedDataItem->getValue(currentTimeIndex,it.second, & value);
              m_cequalComponent->m_QIND[it.first] = value;
            }
          }
          break;
        case Temperature:
          {
            for(auto it : m_idMapping)
            {
              double value = 0;
              timeIdBasedDataItem->getValue(currentTimeIndex,it.second, & value);
              m_cequalComponent->m_TIND[it.first] = value;
            }
          }
          break;
      }
    }
  }
  else if((geometryDataItem = dynamic_cast<IGeometryComponentDataItem*>(provider())))
  {
    switch (m_inputType)
    {
      case Inflow:
        {
          for(auto it : m_idMapping)
          {
            double value = 0;
            geometryDataItem->getValue(it.second, &value);
            m_cequalComponent->m_QIND[it.first] = value;
          }
        }
        break;
      case Temperature:
        {
          for(auto it : m_idMapping)
          {
            double value = 0;
            geometryDataItem->getValue(it.second, &value);
            m_cequalComponent->m_TIND[it.first] = value;
          }
        }
        break;
    }
  }
  else if((idbasedDataItem = dynamic_cast<IIdBasedComponentDataItem*>(provider())))
  {
    switch (m_inputType)
    {
      case Inflow:
        {
          for(auto it : m_idMapping)
          {
            double value = 0;
            idbasedDataItem->getValue(it.second, &value);
            m_cequalComponent->m_QIND[it.first] = value;
          }
        }
        break;
      case Temperature:
        {
          for(auto it : m_idMapping)
          {
            double value = 0;
            idbasedDataItem->getValue(it.second, &value);
            m_cequalComponent->m_TIND[it.first] = value;
          }
        }
        break;
    }
  }
}


