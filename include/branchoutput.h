#ifndef BRANCHOUTPUT_H
#define BRANCHOUTPUT_H


#include "hydrocoupletemporal.h"
#include "cequalw2component_global.h"
#include "temporal/timeseriesidbasedexchangeitem.h"
#include "spatiotemporal/timegeometryoutput.h"

class TimeSeriesProvider;
class Quantity;
class CEQUALW2Component;
//class Dimension;

//namespace SDKTemporal {
//  class DateTime;
//  class TimeSpan;
//}

class CEQUALW2COMPONENT_EXPORT BranchOutput: public TimeSeriesIdBasedOutputDouble
{
    Q_OBJECT

  public:

    enum OutputType
    {
      Flow,
      Heat
    };

    BranchOutput(const QString& id,
                 Dimension* identifierDimension,
                 const QStringList & identifiers,
                 Dimension *timeDimension,
                 OutputType outputType,
                 Quantity *valueDefinition,
                 CEQUALW2Component *component);

    virtual ~BranchOutput() override;

    void updateValues(HydroCouple::IInput *querySpecifier) override;

    void updateValues() override;

    TimeSeriesProvider *timeSeriesProvider() const;

  private:
    OutputType m_outputType;
    CEQUALW2Component *m_cequalw2ModelComponent;
};



#endif // BRANCHOUTPUT_H
