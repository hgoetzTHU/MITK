/*===================================================================

 The Medical Imaging Interaction Toolkit (MITK)

 Copyright (c) German Cancer Research Center,
 Division of Medical and Biological Informatics.
 All rights reserved.

 This software is distributed WITHOUT ANY WARRANTY; without
 even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.

 See LICENSE.txt or http://www.mitk.org for details.

 ===================================================================*/

#ifndef mitkStateMachineConfig_h
#define mitkStateMachineConfig_h

#include <vtkXMLParser.h>
#include <iostream>
#include "itkObject.h"

#include "itkObjectFactory.h"
#include "mitkCommon.h"
#include <MitkExports.h>
#include <mitkPropertyList.h>
#include <mitkInteractionEvent.h>
#include <string>

namespace mitk
{

  /**
   *@brief Configuration Object for Statemachines.
   *
   * Reads given config file, which translates specific user inputs (mitk::InteractionEvents) into EventVariants that can be processed
   * by the StateMachine.
   * @ingroup Interaction
   **/

  class MITK_CORE_EXPORT StateMachineConfig: public vtkXMLParser
  {
  public:

    static StateMachineConfig *New();
    vtkTypeMacro(StateMachineConfig,vtkXMLParser);

    /**
     * @brief loads the xml file filename
     **/
    bool LoadConfig(std::string fileName);
    /**
     * @brief loads the xml config file as string
     **/
    bool LoadConfigString(std::string xmlString);

    /**
     * Returns a PropertyList that contains the properties set in the configuration file.
     * All properties are stored as strings.
     */
    const PropertyList::Pointer GetPropertyList();

    /**
     * Checks if the config object has a definition for the given event. If it has the corresponding variant name is return, else
     * an empty string is returned.
     */
    std::string GetMappedEvent(InteractionEvent* interactionEvent);

  protected:

    StateMachineConfig();
    virtual ~StateMachineConfig();

    /**
     * @brief Derived from XMLReader
     **/
    void StartElement(const char* elementName, const char **atts);
    /**
     * @brief Derived from XMLReader
     **/
    void EndElement(const char* elementName);

  private:
    /**
     * @brief Derived from XMLReader
     **/
    std::string ReadXMLStringAttribut(std::string name, const char** atts);
    /**
     * @brief Derived from XMLReader
     **/
    bool ReadXMLBooleanAttribut(std::string name, const char** atts);

    /**
     * @brief List of all global properties of the config object.
     */
    PropertyList::Pointer m_PropertyList;

    /**
     * @brief Temporal list of all properties of a Event. Used to parse an Input-Event and collect all parameters between the two <input>
     * and </input> tags.
     */
    PropertyList::Pointer m_EventPropertyList;

    struct EventMapping
    {
      std::string variantName;
      InteractionEvent::Pointer interactionEvent;
    };
    typedef std::list<EventMapping> EventListType;
    EventMapping m_CurrEventMapping;

    /**
     * Stores InteractionEvents and their corresponding VariantName
     */
    EventListType m_EventList;
    bool m_errors; // use member, because of inheritance from vtkXMLParser we can't return a success value for parsing the file.
  };

} // namespace mitk

#endif /* mitkStateMachineConfig_h */
