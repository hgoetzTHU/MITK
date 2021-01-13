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

#ifndef PluginProject_h
#define PluginProject_h

#include <berryISelectionListener.h>

#include <QTimer>
#include <QmitkAbstractView.h>

#include "ui_PluginProjectControls.h"
#include <mitkPointSet.h>
#include <mitkRenderWindow.h>
#include <mitkStandaloneDataStorage.h>

/**
  \brief PluginProject

  \warning  This class is not yet documented. Use "git blame" and ask the author to provide basic documentation.

  \sa QmitkAbstractView
  \ingroup ${plugin_target}_internal
*/
class PluginProject : public QmitkAbstractView
{
  // this is needed for all Qt objects that should have a Qt meta-object
  // (everything that derives from QObject and wants to have signal/slots)
  Q_OBJECT

public:
  static const std::string VIEW_ID;

public slots:
  void PathTracking();

protected:
  virtual void CreateQtPartControl(QWidget *parent) override;

  virtual void SetFocus() override;

  /// \brief called by QmitkFunctionality when DataManager's selection has changed
  virtual void OnSelectionChanged(berry::IWorkbenchPart::Pointer source,
                                  const QList<mitk::DataNode::Pointer> &nodes) override;

  /// \brief Called when the user clicks the GUI button
  void DoImageProcessing();
  void DoTracking();
  void TimerTracking();
  void StopTracking();
  

  mitk::PointSet::Pointer PointSetTracking;
  mitk::StandaloneDataStorage::Pointer DS;
  mitk::RenderWindow::Pointer rdW;
  mitk::DataNode::Pointer myDN;


  QTimer *timer;

  Ui::PluginProjectControls m_Controls;
};

#endif // PluginProject_h
