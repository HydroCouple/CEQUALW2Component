/*!
 *  \file    CEQUALW2Component.h
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

#ifndef CEQUALW2COMPONENT_H
#define CEQUALW2COMPONENT_H

#include "cequalw2component_global.h"
#include "cequalw2componentinfo.h"
#include "temporal/abstracttimemodelcomponent.h"

/*!
 *
 */
extern "C"
{
    typedef int (*CE_QUAL_W2_Initialize)(const char modelDirectory[]);
    typedef int (*CE_QUAL_W2_Generic)();
    typedef int (*CE_QUAL_W2_GetArray)(int *key, void **outputArray);
}

class Dimension;
class Unit;
class Quantity;

class CEQUALW2COMPONENT_EXPORT CEQUALW2Component : public AbstractTimeModelComponent,
    public virtual HydroCouple::ICloneableModelComponent
{

    friend class CEQUALW2BranchInput;
    friend class CEQUALW2BranchOutput;

    Q_OBJECT
    Q_INTERFACES(HydroCouple::ICloneableModelComponent)

  public:

    /*!
     * \brief CEQUALW2Component constructor
     * \param id Unique identifier for this component instance.
     * \param modelComponentInfo the parent ModelComponentInfo that generated this component instance.
     */
    CEQUALW2Component(const QString &id, VOID_P libraryHandle,  CEQUALW2ComponentInfo* modelComponentInfo = nullptr);


    /*!
     * \brief ~CEQUALW2Component destructor
     */
    virtual ~CEQUALW2Component() override;


    /*!
     * \brief setFunctionPointers
     */
    void setFunctionPointers();

    /*!
     * \brief validate validates this component model instance
     * \return Returns a list of error messages.
     */
    QList<QString> validate() override;

    /*!
     * \brief prepare Prepares the model component instance.
     */
    void prepare() override;

    /*!
     * \brief update
     * \param requiredOutputs
     */
    void update(const QList<HydroCouple::IOutput*> &requiredOutputs = QList<HydroCouple::IOutput*>()) override;


    /*!
     * \brief finish
     */
    void finish() override;

    /*!
     * \brief parent
     * \return
     */
    HydroCouple::ICloneableModelComponent* parent() const override;

    /*!
     * \brief clone
     * \return
     */
    HydroCouple::ICloneableModelComponent* clone() override;

    /*!
     * \brief clones
     * \return
     */
    QList<HydroCouple::ICloneableModelComponent*> clones() const override;

  protected:

    bool removeClone(CEQUALW2Component *component);


    /*!
     * \brief intializeFailureCleanUp
     */
    void initializeFailureCleanUp() override;

  private:

    /*!
     * \brief createArguments
     */
    void createArguments() override;

    /*!
     * \brief createInputFileArguments
     */
    void createInputFileArguments();

    /*!
     * \brief initializeArguments
     * \param message
     * \return
     */
    bool initializeArguments(QString &message) override;

    /*!
     * \brief initializeInputFilesArguments
     * \param message
     * \return
     */
    bool initializeInputFilesArguments(QString &message);

    /*!
     * \brief createGeometriesMap
     */
    void createGeometries();

    /*!
     * \brief createInputs
     */
    void createInputs() override;

    /*!
     * \brief createOutputs
     */
    void createOutputs() override;

    /*!
     * \brief getCurrentJulianDateTime
     * \return
     */
    double getCurrentJulianDateTime();



 signals:

    void onAboutToDelete(CEQUALW2Component *component);

  private:

    IdBasedArgumentString *m_inputFilesArgument;
    CEQUALW2Component *m_parent = nullptr;
    QList<HydroCouple::ICloneableModelComponent*> m_clones;
    VOID_P m_libHandle;
    CE_QUAL_W2_Initialize m_initializeFunction;
    CE_QUAL_W2_Generic m_prepareForUpdateFunction, m_updateFunction, m_finalizeFunction;
    CE_QUAL_W2_GetArray m_getArrayFunction;
    int *m_startYear;
    double *m_startDateTime, *m_endDateTime, *m_currentDateTime;
    int *m_NWB =nullptr, *m_NBR, *m_KMX, *m_IMX, *m_NTR, *m_NST, *m_NWD, *m_NSTR;
    double *m_QSTR,  *m_TaveSTR, *m_QIND, *m_TIND;
    bool *m_modelFinished;
    double m_startJulianDate;
    Dimension *m_timeDimension;
    Unit   *m_heatFluxUnit, *m_temperatureUnit;

};

#endif //CEQUALW2COMPONENT_H
