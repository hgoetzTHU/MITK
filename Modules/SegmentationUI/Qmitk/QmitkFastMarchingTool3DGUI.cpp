/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/

#include "QmitkFastMarchingTool3DGUI.h"

#include "QmitkConfirmSegmentationDialog.h"

#include "mitkBaseRenderer.h"
#include "mitkStepper.h"
#include <QApplication>
#include <QGroupBox>
#include <QMessageBox>
#include <ctkRangeWidget.h>
#include <ctkSliderWidget.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qcheckbox.h>

MITK_TOOL_GUI_MACRO(MITKSEGMENTATIONUI_EXPORT, QmitkFastMarchingTool3DGUI, "")

QmitkFastMarchingTool3DGUI::QmitkFastMarchingTool3DGUI() : QmitkToolGUI()
{
  this->setContentsMargins(0, 0, 0, 0);

  // create the visible widgets
  QVBoxLayout *widgetLayout = new QVBoxLayout(this);
  widgetLayout->setContentsMargins(0, 0, 0, 0);

  QFont fntHelp;
  fntHelp.setBold(true);

  QLabel *lblHelp = new QLabel(this);
  lblHelp->setText("Press shift-click to add seeds repeatedly.");
  lblHelp->setFont(fntHelp);

  widgetLayout->addWidget(lblHelp);

  // Sigma controls
  {
    QHBoxLayout *hlayout = new QHBoxLayout();
    hlayout->setSpacing(2);

    QLabel *lbl = new QLabel(this);
    lbl->setText("Sigma: ");
    hlayout->addWidget(lbl);

    QSpacerItem *sp2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    hlayout->addItem(sp2);

    widgetLayout->addItem(hlayout);
  }

  m_slSigma = new ctkSliderWidget(this);
  m_slSigma->setMinimum(0.1);
  m_slSigma->setMaximum(5.0);
  m_slSigma->setPageStep(0.1);
  m_slSigma->setSingleStep(0.01);
  m_slSigma->setValue(1.0);
  m_slSigma->setTracking(false);
  m_slSigma->setToolTip("The \"sigma\" parameter in the Gradient Magnitude filter.");
  connect(m_slSigma, SIGNAL(valueChanged(double)), this, SLOT(OnSigmaChanged(double)));
  widgetLayout->addWidget(m_slSigma);

  // Alpha controls
  {
    QHBoxLayout *hlayout = new QHBoxLayout();
    hlayout->setSpacing(2);

    QLabel *lbl = new QLabel(this);
    lbl->setText("Alpha: ");
    hlayout->addWidget(lbl);

    QSpacerItem *sp2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    hlayout->addItem(sp2);

    widgetLayout->addItem(hlayout);
  }

  m_slAlpha = new ctkSliderWidget(this);
  m_slAlpha->setMinimum(-10);
  m_slAlpha->setMaximum(0);
  m_slAlpha->setPageStep(0.1);
  m_slAlpha->setSingleStep(0.01);
  m_slAlpha->setValue(-2.5);
  m_slAlpha->setTracking(false);
  m_slAlpha->setToolTip("The \"alpha\" parameter in the Sigmoid mapping filter.");
  connect(m_slAlpha, SIGNAL(valueChanged(double)), this, SLOT(OnAlphaChanged(double)));
  widgetLayout->addWidget(m_slAlpha);

  // Beta controls
  {
    QHBoxLayout *hlayout = new QHBoxLayout();
    hlayout->setSpacing(2);

    QLabel *lbl = new QLabel(this);
    lbl->setText("Beta: ");
    hlayout->addWidget(lbl);

    QSpacerItem *sp2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    hlayout->addItem(sp2);

    widgetLayout->addLayout(hlayout);
  }

  m_slBeta = new ctkSliderWidget(this);
  m_slBeta->setMinimum(0);
  m_slBeta->setMaximum(100);
  m_slBeta->setPageStep(0.1);
  m_slBeta->setSingleStep(0.01);
  m_slBeta->setValue(3.5);
  m_slBeta->setTracking(false);
  m_slBeta->setToolTip("The \"beta\" parameter in the Sigmoid mapping filter.");
  connect(m_slBeta, SIGNAL(valueChanged(double)), this, SLOT(OnBetaChanged(double)));
  widgetLayout->addWidget(m_slBeta);

  // stopping value controls
  {
    QHBoxLayout *hlayout = new QHBoxLayout();
    hlayout->setSpacing(2);

    QLabel *lbl = new QLabel(this);
    lbl->setText("Stopping value: ");
    hlayout->addWidget(lbl);

    QSpacerItem *sp2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    hlayout->addItem(sp2);

    widgetLayout->addLayout(hlayout);
  }

  m_slStoppingValue = new ctkSliderWidget(this);
  m_slStoppingValue->setMinimum(0);
  m_slStoppingValue->setMaximum(10000);
  m_slStoppingValue->setPageStep(10);
  m_slStoppingValue->setSingleStep(1);
  m_slStoppingValue->setValue(2000);
  m_slStoppingValue->setDecimals(0);
  m_slStoppingValue->setTracking(false);
  m_slStoppingValue->setToolTip("The \"stopping value\" parameter in the fast marching 3D algorithm");
  connect(m_slStoppingValue, SIGNAL(valueChanged(double)), this, SLOT(OnStoppingValueChanged(double)));
  widgetLayout->addWidget(m_slStoppingValue);

  // threshold controls
  {
    QHBoxLayout *hlayout = new QHBoxLayout();
    hlayout->setSpacing(2);

    QLabel *lbl = new QLabel(this);
    lbl->setText("Threshold: ");
    hlayout->addWidget(lbl);

    QSpacerItem *sp2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    hlayout->addItem(sp2);

    widgetLayout->addLayout(hlayout);
  }

  m_slwThreshold = new ctkRangeWidget(this);
  m_slwThreshold->setMinimum(-100);
  m_slwThreshold->setMaximum(5000);
  m_slwThreshold->setMinimumValue(-100);
  m_slwThreshold->setMaximumValue(2000);
  m_slwThreshold->setDecimals(0);
  m_slwThreshold->setTracking(false);
  m_slwThreshold->setToolTip("The lower and upper thresholds for the final thresholding");
  connect(m_slwThreshold, SIGNAL(valuesChanged(double, double)), this, SLOT(OnThresholdChanged(double, double)));
  widgetLayout->addWidget(m_slwThreshold);

  m_btClearSeeds = new QPushButton("Clear");
  m_btClearSeeds->setToolTip("Clear current result and start over again");
  m_btClearSeeds->setEnabled(false);
  widgetLayout->addWidget(m_btClearSeeds);
  connect(m_btClearSeeds, SIGNAL(clicked()), this, SLOT(OnClearSeeds()));

  m_btConfirm = new QPushButton("Confirm Segmentation");
  m_btConfirm->setToolTip("Incorporate current result in your working session.");
  m_btConfirm->setEnabled(false);
  widgetLayout->addWidget(m_btConfirm);
  connect(m_btConfirm, SIGNAL(clicked()), this, SLOT(OnConfirmSegmentation()));

  m_CheckProcessAll = new QCheckBox("Process all time steps", this);
  m_CheckProcessAll->setChecked(false);
  m_CheckProcessAll->setToolTip("Process/overwrite all time steps of the dynamic segmentation and not just the currently visible time step.");
  widgetLayout->addWidget(m_CheckProcessAll);

  m_CheckCreateNew = new QCheckBox("Create as new segmentation", this);
  m_CheckCreateNew->setChecked(false);
  m_CheckCreateNew->setToolTip("Add the confirmed segmentation as a new segmentation instead of overwriting the currently selected.");
  widgetLayout->addWidget(m_CheckCreateNew);

  connect(this, SIGNAL(NewToolAssociated(mitk::Tool *)), this, SLOT(OnNewToolAssociated(mitk::Tool *)));

  m_slSigma->setDecimals(2);
  m_slBeta->setDecimals(2);
  m_slAlpha->setDecimals(2);

  this->EnableWidgets(false);
}

QmitkFastMarchingTool3DGUI::~QmitkFastMarchingTool3DGUI()
{
  if (m_FastMarchingTool.IsNotNull())
  {
    m_FastMarchingTool->CurrentlyBusy -=
      mitk::MessageDelegate1<QmitkFastMarchingTool3DGUI, bool>(this, &QmitkFastMarchingTool3DGUI::BusyStateChanged);
  }
}

void QmitkFastMarchingTool3DGUI::OnNewToolAssociated(mitk::Tool *tool)
{
  if (m_FastMarchingTool.IsNotNull())
  {
    m_FastMarchingTool->CurrentlyBusy -=
      mitk::MessageDelegate1<QmitkFastMarchingTool3DGUI, bool>(this, &QmitkFastMarchingTool3DGUI::BusyStateChanged);
  }

  m_FastMarchingTool = dynamic_cast<mitk::FastMarchingTool3D *>(tool);

  if (m_FastMarchingTool.IsNotNull())
  {
    m_FastMarchingTool->CurrentlyBusy +=
      mitk::MessageDelegate1<QmitkFastMarchingTool3DGUI, bool>(this, &QmitkFastMarchingTool3DGUI::BusyStateChanged);

    m_FastMarchingTool->SetLowerThreshold(m_slwThreshold->minimumValue());
    m_FastMarchingTool->SetUpperThreshold(m_slwThreshold->maximumValue());
    m_FastMarchingTool->SetStoppingValue(m_slStoppingValue->value());
    m_FastMarchingTool->SetSigma(m_slSigma->value());
    m_FastMarchingTool->SetAlpha(m_slAlpha->value());
    m_FastMarchingTool->SetBeta(m_slBeta->value());
    m_FastMarchingTool->SetOverwriteExistingSegmentation(true);
    m_FastMarchingTool->ClearSeeds();
    m_CheckProcessAll->setVisible(m_FastMarchingTool->GetTargetSegmentationNode()->GetData()->GetTimeSteps() > 1);
  }
}

void QmitkFastMarchingTool3DGUI::Update()
{
  if (m_FastMarchingTool.IsNotNull())
  {
    m_FastMarchingTool->SetLowerThreshold(m_slwThreshold->minimumValue());
    m_FastMarchingTool->SetUpperThreshold(m_slwThreshold->maximumValue());
    m_FastMarchingTool->SetStoppingValue(m_slStoppingValue->value());
    m_FastMarchingTool->SetSigma(m_slSigma->value());
    m_FastMarchingTool->SetAlpha(m_slAlpha->value());
    m_FastMarchingTool->SetBeta(m_slBeta->value());
    m_FastMarchingTool->UpdatePreview();
  }
}

void QmitkFastMarchingTool3DGUI::OnThresholdChanged(double lower, double upper)
{
  if (m_FastMarchingTool.IsNotNull())
  {
    m_FastMarchingTool->SetLowerThreshold(lower);
    m_FastMarchingTool->SetUpperThreshold(upper);
    this->Update();
  }
}

void QmitkFastMarchingTool3DGUI::OnBetaChanged(double value)
{
  if (m_FastMarchingTool.IsNotNull())
  {
    m_FastMarchingTool->SetBeta(value);
    this->Update();
  }
}

void QmitkFastMarchingTool3DGUI::OnSigmaChanged(double value)
{
  if (m_FastMarchingTool.IsNotNull())
  {
    m_FastMarchingTool->SetSigma(value);
    this->Update();
  }
}

void QmitkFastMarchingTool3DGUI::OnAlphaChanged(double value)
{
  if (m_FastMarchingTool.IsNotNull())
  {
    m_FastMarchingTool->SetAlpha(value);
    this->Update();
  }
}

void QmitkFastMarchingTool3DGUI::OnStoppingValueChanged(double value)
{
  if (m_FastMarchingTool.IsNotNull())
  {
    m_FastMarchingTool->SetStoppingValue(value);
    this->Update();
  }
}

void QmitkFastMarchingTool3DGUI::OnConfirmSegmentation()
{
  if (m_FastMarchingTool.IsNotNull())
  {
    if (m_CheckCreateNew->isChecked())
    {
      m_FastMarchingTool->SetOverwriteExistingSegmentation(false);
    }
    else
    {
      m_FastMarchingTool->SetOverwriteExistingSegmentation(true);
    }

    m_FastMarchingTool->SetCreateAllTimeSteps(m_CheckProcessAll->isChecked());

    m_btConfirm->setEnabled(false);
    m_FastMarchingTool->ConfirmSegmentation();
  }
}

void QmitkFastMarchingTool3DGUI::OnClearSeeds()
{
  // event from image navigator recieved - timestep has changed
  m_FastMarchingTool->ClearSeeds();
  m_btClearSeeds->setEnabled(false);
  m_btConfirm->setEnabled(false);
  this->EnableWidgets(false);
  this->Update();
}

void QmitkFastMarchingTool3DGUI::BusyStateChanged(bool value)
{
  if (value)
  {
    QApplication::setOverrideCursor(QCursor(Qt::BusyCursor));
    this->EnableWidgets(false);
  }
  else
  {
    QApplication::restoreOverrideCursor();
    this->EnableWidgets(true);
  }
}

void QmitkFastMarchingTool3DGUI::EnableWidgets(bool enable)
{
  m_slSigma->setEnabled(enable);
  m_slAlpha->setEnabled(enable);
  m_slBeta->setEnabled(enable);
  m_slStoppingValue->setEnabled(enable);
  m_slwThreshold->setEnabled(enable);
  m_btClearSeeds->setEnabled(enable);
  m_btConfirm->setEnabled(enable);
  m_CheckCreateNew->setEnabled(enable);
  m_CheckProcessAll->setEnabled(enable);
}
