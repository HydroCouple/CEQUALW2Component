#ifndef CEQUALW2BranchOutput_H
#define CEQUALW2BranchOutput_H


//#include "hydrocoupletemporal.h"
#include "cequalw2component_global.h"
#include "temporal/timeseriesidbasedexchangeitem.h"
//#include "spatiotemporal/timegeometryoutput.h"

class Quantity;
class CEQUALW2Component;
class Dimension;

class CEQUALW2COMPONENT_EXPORT CEQUALW2BranchOutput: public TimeSeriesIdBasedOutputDouble
{
    Q_OBJECT

  public:

    enum OutputType
    {
      Flow,
      Heat
    };

    CEQUALW2BranchOutput(const QString& id,
                 Dimension* identifierDimension,
                 const QStringList & identifiers,
                 Dimension *timeDimension,
                 OutputType outputType,
                 Quantity *valueDefinition,
                 CEQUALW2Component *component);

    virtual ~CEQUALW2BranchOutput() override;

    void updateValues(HydroCouple::IInput *querySpecifier) override;

    void updateValues() override;

  private:
    OutputType m_outputType;
    CEQUALW2Component *m_cequalw2ModelComponent;
};



#endif // CEQUALW2BranchOutput_H
