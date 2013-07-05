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

#include "mitkOverlay.h"

mitk::Overlay::Overlay() : m_LayoutedBy(NULL)
{
  m_PropertyList = mitk::PropertyList::New();
}


mitk::Overlay::~Overlay()
{
}

void mitk::Overlay::SetProperty(const char *propertyKey,
                                     BaseProperty* propertyValue,
                                     const mitk::BaseRenderer* renderer)
{
  GetPropertyList(renderer)->SetProperty(propertyKey, propertyValue);
}

void mitk::Overlay::ReplaceProperty(const char *propertyKey,
                                         BaseProperty* propertyValue,
                                         const mitk::BaseRenderer* renderer)
{
  GetPropertyList(renderer)->ReplaceProperty(propertyKey, propertyValue);
}

void mitk::Overlay::AddProperty(const char *propertyKey,
                                     BaseProperty* propertyValue,
                                     const mitk::BaseRenderer* renderer,
                                     bool overwrite)
{
  if((overwrite) || (GetProperty(propertyKey, renderer) == NULL))
  {
    SetProperty(propertyKey, propertyValue, renderer);
  }
}

void mitk::Overlay::ConcatenatePropertyList(PropertyList *pList, bool replace)
{
  m_PropertyList->ConcatenatePropertyList(pList, replace);
}

mitk::BaseProperty* mitk::Overlay::GetProperty(const char *propertyKey, const mitk::BaseRenderer* renderer) const
{
  if(propertyKey==NULL)
    return NULL;

  //renderer specified?
  if (renderer)
  {
    std::map<const mitk::BaseRenderer*,mitk::PropertyList::Pointer>::const_iterator it;
    //check for the renderer specific property
    it=m_MapOfPropertyLists.find(renderer);
    if(it!=m_MapOfPropertyLists.end()) //found
    {
      mitk::BaseProperty::Pointer property;
      property=it->second->GetProperty(propertyKey);
      if(property.IsNotNull())//found an enabled property in the render specific list
        return property;
      else //found a renderer specific list, but not the desired property
        return m_PropertyList->GetProperty(propertyKey); //return renderer unspecific property
    }
    else //didn't find the property list of the given renderer
    {
      //return the renderer unspecific property if there is one
      return m_PropertyList->GetProperty(propertyKey);
    }
  }
  else //no specific renderer given; use the renderer independent one
  {
    mitk::BaseProperty::Pointer property;
    property=m_PropertyList->GetProperty(propertyKey);
    if(property.IsNotNull())
      return property;
  }

  //only to satisfy compiler!
  return NULL;
}

bool mitk::Overlay::GetBoolProperty(const char* propertyKey, bool& boolValue, mitk::BaseRenderer* renderer) const
{
  mitk::BoolProperty::Pointer boolprop = dynamic_cast<mitk::BoolProperty*>(GetProperty(propertyKey, renderer));
  if(boolprop.IsNull())
    return false;

  boolValue = boolprop->GetValue();
  return true;
}

bool mitk::Overlay::GetIntProperty(const char* propertyKey, int &intValue, mitk::BaseRenderer* renderer) const
{
  mitk::IntProperty::Pointer intprop = dynamic_cast<mitk::IntProperty*>(GetProperty(propertyKey, renderer));
  if(intprop.IsNull())
    return false;

  intValue = intprop->GetValue();
  return true;
}

bool mitk::Overlay::GetFloatProperty(const char* propertyKey, float &floatValue, mitk::BaseRenderer* renderer) const
{
  mitk::FloatProperty::Pointer floatprop = dynamic_cast<mitk::FloatProperty*>(GetProperty(propertyKey, renderer));
  if(floatprop.IsNull())
    return false;

  floatValue = floatprop->GetValue();
  return true;
}

bool mitk::Overlay::GetStringProperty(const char* propertyKey, std::string& string, mitk::BaseRenderer* renderer) const
{
  mitk::StringProperty::Pointer stringProp = dynamic_cast<mitk::StringProperty*>(GetProperty(propertyKey, renderer));
  if(stringProp.IsNull())
  {
    return false;
  }
  else
  {
    //memcpy((void*)string, stringProp->GetValue(), strlen(stringProp->GetValue()) + 1 ); // looks dangerous
    string = stringProp->GetValue();
    return true;
  }
}

void mitk::Overlay::SetIntProperty(const char* propertyKey, int intValue, mitk::BaseRenderer* renderer)
{
  GetPropertyList(renderer)->SetProperty(propertyKey, mitk::IntProperty::New(intValue));
}
void mitk::Overlay::SetBoolProperty( const char* propertyKey, bool boolValue, mitk::BaseRenderer* renderer/*=NULL*/ )
{
  GetPropertyList(renderer)->SetProperty(propertyKey, mitk::BoolProperty::New(boolValue));
}

void mitk::Overlay::SetFloatProperty( const char* propertyKey, float floatValue, mitk::BaseRenderer* renderer/*=NULL*/ )
{
  GetPropertyList(renderer)->SetProperty(propertyKey, mitk::FloatProperty::New(floatValue));
}

void mitk::Overlay::SetStringProperty( const char* propertyKey, const char* stringValue, mitk::BaseRenderer* renderer/*=NULL*/ )
{
  GetPropertyList(renderer)->SetProperty(propertyKey, mitk::StringProperty::New(stringValue));
}

void mitk::Overlay::SetText(std::string text)
{
  SetStringProperty("text", text.c_str());
}

std::string mitk::Overlay::GetText()
{
  std::string text;
  GetPropertyList()->GetStringProperty("text", text);
  return text;
}

void mitk::Overlay::SetFontSize(int fontSize)
{
  SetIntProperty("fontSize",fontSize);
}

int mitk::Overlay::GetFontSize()
{
  int fontSize;
  GetPropertyList()->GetIntProperty("fontSize", fontSize);
  return fontSize;
}

bool mitk::Overlay::GetColor(float rgb[], mitk::BaseRenderer* renderer, const char* propertyKey) const
{
  mitk::ColorProperty::Pointer colorprop = dynamic_cast<mitk::ColorProperty*>(GetProperty(propertyKey, renderer));
  if(colorprop.IsNull())
    return false;

  memcpy(rgb, colorprop->GetColor().GetDataPointer(), 3*sizeof(float));
  return true;
}

void mitk::Overlay::SetColor(const mitk::Color &color, mitk::BaseRenderer* renderer, const char* propertyKey)
{
  mitk::ColorProperty::Pointer prop;
  prop = mitk::ColorProperty::New(color);
  GetPropertyList(renderer)->SetProperty(propertyKey, prop);
}

void mitk::Overlay::SetColor(float red, float green, float blue, mitk::BaseRenderer* renderer, const char* propertyKey)
{
  float color[3];
  color[0]=red;
  color[1]=green;
  color[2]=blue;
  SetColor(color, renderer, propertyKey);
}

void mitk::Overlay::SetColor(const float rgb[], mitk::BaseRenderer* renderer, const char* propertyKey)
{
  mitk::ColorProperty::Pointer prop;
  prop = mitk::ColorProperty::New(rgb);
  GetPropertyList(renderer)->SetProperty(propertyKey, prop);
}

bool mitk::Overlay::GetOpacity(float &opacity, mitk::BaseRenderer* renderer, const char* propertyKey) const
{
  mitk::FloatProperty::Pointer opacityprop = dynamic_cast<mitk::FloatProperty*>(GetProperty(propertyKey, renderer));
  if(opacityprop.IsNull())
    return false;

  opacity=opacityprop->GetValue();
  return true;
}

void mitk::Overlay::SetOpacity(float opacity, mitk::BaseRenderer* renderer, const char* propertyKey)
{
  mitk::FloatProperty::Pointer prop;
  prop = mitk::FloatProperty::New(opacity);
  GetPropertyList(renderer)->SetProperty(propertyKey, prop);
}

//void mitk::Overlay::SetLayout(mitk::BaseLayouter::Pointer layout, mitk::BaseRenderer* renderer)
//{
//  if(renderer==NULL || layout.IsNull())
//    return;

//  mitk::BaseLayouter::Pointer & OldLayout = m_MapOfLayouters[renderer];

//  if(OldLayout.IsNotNull())
//    OldLayout->RemoveOverlay(this);

//  m_MapOfLayouters[renderer] = layout;
//}

//mitk::BaseLayouter::Pointer mitk::Overlay::GetLayout(mitk::BaseRenderer *renderer)
//{
//  if(renderer==NULL)
//    return NULL;

//  mitk::BaseLayouter::Pointer & layout = m_MapOfLayouters[renderer];

//  return layout;
//}

void mitk::Overlay::SetVisibility(bool visible, mitk::BaseRenderer *renderer, const char *propertyKey)
{
  mitk::BoolProperty::Pointer prop;
  prop = mitk::BoolProperty::New(visible);
  GetPropertyList(renderer)->SetProperty(propertyKey, prop);
}

mitk::PropertyList* mitk::Overlay::GetPropertyList(const mitk::BaseRenderer* renderer) const
{
  if(renderer==NULL)
    return m_PropertyList;

  mitk::PropertyList::Pointer & propertyList = m_MapOfPropertyLists[renderer];

  if(propertyList.IsNull())
    propertyList = mitk::PropertyList::New();

  assert(m_MapOfPropertyLists[renderer].IsNotNull());

  return propertyList;
}

bool mitk::Overlay::BaseLocalStorage::IsGenerateDataRequired(mitk::BaseRenderer *renderer, mitk::Overlay *overlay)
{
  if( m_LastGenerateDataTime < overlay -> GetMTime () )
    return true;

  if( renderer && m_LastGenerateDataTime < renderer -> GetTimeStepUpdateTime ( ) )
    return true;

  return false;
}

mitk::PropertyList::Pointer mitk::Overlay::BaseLocalStorage::GetPropertyList()
{
  if (m_PropertyList.IsNull())
    m_PropertyList = mitk::PropertyList::New();
  return m_PropertyList;
}
