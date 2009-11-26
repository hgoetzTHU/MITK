/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Module:    $RCSfile$
Language:  C++
Date:      $Date: 2009-03-21 19:27:37 +0100 (Sa, 21 Mrz 2009) $
Version:   $Revision: 16719 $ 

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef _QmitkNDIConfigurationWidget_H_INCLUDED
#define _QmitkNDIConfigurationWidget_H_INCLUDED

#include "ui_QmitkNDIConfigurationWidget.h"
#include "mitkNDITrackingDevice.h"
#include "mitkTrackingDeviceSource.h"
#include "QStringList"
#include "MitkIGTUIExports.h"
/*!
  \brief Heidelberg Minimally Invasive Navigation Device

  Functionality for visualizing a tracking instrument in relation to a tracked patient.

  \sa QmitkFunctionality
  \sa IGT
  \ingroup Functionalities
*/
class MitkIGTUI_EXPORT QmitkNDIConfigurationWidget : public QWidget
{  
  Q_OBJECT // this is needed for all Qt objects that should have a MOC object (everything that derives from QObject)
public: 

  QmitkNDIConfigurationWidget(QWidget* parent);
  virtual ~QmitkNDIConfigurationWidget();

  std::string GetDeviceName() const;
  mitk::NDITrackingDevice* GetTracker() const;

  signals:
    void ToolsAdded(QStringList tools);
    void Connected();
    void Disconnected();
  public slots:
    void SetDeviceName(const char* dev);  ///< set the device name (e.g. "COM1", "/dev/ttyS0") that will be used to connect to the tracking device

  protected slots:
    void OnConnect();
    void OnDisconnect();
    void OnDiscoverTools();
    void OnDiscoverDevices();
    void OnAddPassiveTool();

protected:
  typedef QMap<QString, mitk::TrackingDeviceType> PortDeviceMap;  // key is port name (e.g. "COM1", "/dev/ttyS0"), value will be filled with the type of tracking device at this port
  /**Documentation
  * \brief scans the ports provided as key in the portsAndDevices and fills the respective value of portsAndDevices with the tracking device type at that port
  *
  * 
  * \param[in] portsAndDevices keys are used to query serial ports
  * \param[out] portsAndDevices values of the existing keys will be filled with the tracking device type
  */
  void ScanPortsForNDITrackingDevices(PortDeviceMap& portsAndDevices);
  mitk::TrackingDeviceType ScanPort(QString port);
  
  QStringList GetToolNamesList(); ///< returns a string list with the names of all tools of the current tracking device

  void CreateTracker();  ///< creates new NDITrackingDevice object
  void SetupTracker();   ///< sets the parameters from the gui to the tracking device object
  QString GetStatusText(); ///< construct a status text depending on the current state of the tracking device object

  void UpdateWidgets();
  void UpdateToolTable(); ///< read all tools from the tracking device object and display them in the gui
  virtual void CreateQtPartControl(QWidget *parent);
  virtual void CreateConnections(); ///< \brief Creation of the connections of main and control widget  
 
  Ui::QmitkNDIConfigurationWidget* m_Controls;  ///< gui widgets
  mitk::NDITrackingDevice::Pointer m_Tracker;   ///< tracking device object
  mitk::TrackingDeviceSource::Pointer m_Source;
};
#endif // _QmitkNDIConfigurationWidget_H_INCLUDED
