#ifndef CEQUALW2BranchInput_H
#define CEQUALW2BranchInput_H

#include "cequalw2component_global.h"
#include "temporal/timeseriesidbasedexchangeitem.h"
#include <unordered_map>

class CEQUALW2Component;
class Quantity;

class CEQUALW2COMPONENT_EXPORT CEQUALW2BranchInput:
    public TimeSeriesIdBasedInputDouble
{
    Q_OBJECT

  public:

    enum InputType
    {
      Inflow,
      Temperature
    };

    CEQUALW2BranchInput(const QString &inputId,
                const QStringList& identifiers,
                InputType inputType,
                Dimension *identifierDimension,
                Dimension *timeDimension,
                Quantity *quantity,
                CEQUALW2Component *cequalComponent);

    virtual ~CEQUALW2BranchInput() override;

    bool setProvider(HydroCouple::IOutput *provider) override;

    bool canConsume(HydroCouple::IOutput *provider, QString &message) const override;

    void retrieveValuesFromProvider() override;

    void applyData() override;

  private:
    InputType m_inputType;
    CEQUALW2Component *m_cequalComponent;
    std::unordered_map<int,int> m_idMapping;
};


#endif // CEQUALW2BranchInput_H
