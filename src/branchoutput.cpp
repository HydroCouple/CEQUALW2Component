#include "stdafx.h"
#include "branchoutput.h"
#include "core/dimension.h"
#include "temporal/timedata.h"
#include "core/valuedefinition.h"
#include "cequalw2component.h"

using namespace HydroCouple;
using namespace SDKTemporal;

BranchOutput::BranchOutput(const QString & id,
                           Dimension *identifierDimension,
                           const QStringList & identifiers,
                           Dimension *timeDimension,
                           OutputType outputType,
                           Quantity *valueDefinition,
                           CEQUALW2Component *component):
  TimeSeriesIdBasedOutputDouble(id,
                                identifierDimension,
                                timeDimension,
                                valueDefinition,
                                component),
  m_outputType(outputType),
  m_cequalw2ModelComponent(component)
{

  DateTime *dt1 = new DateTime(0, this);
  addTime(dt1);

  DateTime *dt2 = new DateTime(0.1, this);
  addTime(dt2);

  {
    double currentDateTime = m_cequalw2ModelComponent->currentDateTime()->julianDay();

    timeInternal(0)->setJulianDay(currentDateTime - 0.000000000001);
    timeInternal(1)->setJulianDay(currentDateTime);
  }


  addIdentifiers(identifiers);
}

BranchOutput::~BranchOutput()
{
}



void BranchOutput::updateValues(IInput *querySpecifier)
{
  if(!m_cequalw2ModelComponent->workflow())
  {
    ITimeComponentDataItem* timeExchangeItem = dynamic_cast<ITimeComponentDataItem*>(querySpecifier);
    QList<IOutput*>updateList;

    if(timeExchangeItem)
    {
      double queryTime = timeExchangeItem->time(timeExchangeItem->timeCount() - 1)->julianDay();

      while (m_cequalw2ModelComponent->currentDateTime()->julianDay() < queryTime &&
             m_cequalw2ModelComponent->status() == IModelComponent::Updated)
      {
        m_cequalw2ModelComponent->update(updateList);
      }
    }
    else
    {
      if(m_cequalw2ModelComponent->status() == IModelComponent::Updated)
      {
        m_cequalw2ModelComponent->update(updateList);
      }
    }
  }

  refreshAdaptedOutputs();
}

void BranchOutput::updateValues()
{
  int lastDateTimeIndex = timeCount() - 1;
  DateTime *lastDateTime = timeInternal(lastDateTimeIndex);

  if(lastDateTime->julianDay() < m_cequalw2ModelComponent->currentDateTime()->julianDay())
  {
    moveDataToPrevTime();
    lastDateTime->setJulianDay(m_cequalw2ModelComponent->currentDateTime()->julianDay());
    resetTimeSpan();

    switch (m_outputType)
    {
      case BranchOutput::Flow:
        {
          int size = *m_cequalw2ModelComponent->m_NBR;

          for (int i = 0; i < size; i++)
          {
            int nstr = 0;
            double value = 0.0;

            if((nstr = m_cequalw2ModelComponent->m_NSTR[i]) > 0)
            {
              for(int j = 0; j < nstr; j++)
              {
                value += m_cequalw2ModelComponent->m_QSTR[j * size + i];
              }
            }

            setValue(lastDateTimeIndex, i, &value);
          }
        }
        break;
      case BranchOutput::Heat:
        {
          int size = *m_cequalw2ModelComponent->m_NBR;
          for (int i = 0; i < size; i++)
          {
            int nstr = 0;
            double value = 0.0;


            if((nstr = m_cequalw2ModelComponent->m_NSTR[i]) > 0)
            {
              for(int j = 0; j < nstr; j++)
              {
                value += m_cequalw2ModelComponent->m_QSTR[j * size + i] * m_cequalw2ModelComponent->m_TaveSTR[j * size + i] * 4184.0 * 1000.0;
              }
            }

            setValue(lastDateTimeIndex, i, &value);
          }
        }
        break;
    }
  }
}

