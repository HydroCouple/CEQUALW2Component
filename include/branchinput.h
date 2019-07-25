#ifndef BRANCHINPUT_H
#define BRANCHINPUT_H
#include "cequalw2component_global.h"
#include "temporal/timeseriesidbasedexchangeitem.h"

class CEQUALW2Component;
class Quantity;

#include <unordered_map>

class CEQUALW2COMPONENT_EXPORT BranchInput:
    public TimeSeriesIdBasedInputDouble
{
    Q_OBJECT

  public:

    enum InputType
    {
      Inflow,
      Temperature
    };

    BranchInput(const QString &inputId,
                const QStringList& identifiers,
                InputType inputType,
                Dimension *identifierDimension,
                Dimension *timeDimension,
                Quantity *quantity,
                CEQUALW2Component *cequalComponent);

    virtual ~BranchInput() override;

    bool setProvider(HydroCouple::IOutput *provider) override;

    bool canConsume(HydroCouple::IOutput *provider, QString &message) const override;

    void retrieveValuesFromProvider() override;

    void applyData() override;

  private:
    InputType m_inputType;
    CEQUALW2Component *m_cequalComponent;
    std::unordered_map<int,int> m_idMapping;
};


#endif // BRANCHINPUT_H
