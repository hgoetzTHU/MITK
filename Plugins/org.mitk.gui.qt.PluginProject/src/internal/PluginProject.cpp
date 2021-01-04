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


// Blueberry
#include <berryISelectionService.h>
#include <berryIWorkbenchWindow.h>

// Qmitk
#include "PluginProject.h"

// Qt
#include <QMessageBox>
#include <itksys/SystemTools.hxx>

// mitk image
#include <mitkImage.h>

// PointSet
#include <mitkPointSet.h>

// Widgets
#include <QmitkNavigationDataSourceSelectionWidget.h>
#include <QWidget>

// Window
#include "QmitkRegisterClasses.h"
#include "QmitkRenderWindow.h"

//DataStorage
#include <mitkCone.h>
#include <mitkCylinder.h>
#include <mitkDataNode.h>
#include <mitkDataStorage.h>
#include <mitkRenderWindow.h>
#include <mitkStandaloneDataStorage.h>

//Initialisierung
// Initialisierung der Werte für die Distanzmessung
double startvalues[] = {
  0, 0, 0};             // da der Button zweimal gedrückt werden muss für Pos 1 und Pos2 muss sich der Wert ändern
double a[] = {0, 0, 0}; // Array, um die x,y,z Position von der ersten Position zu speichern
double b[] = {0, 0, 0}; // Array, um die x,y,z Position von der zweiten Position zu speichern
mitk::Point3D Pos1;     // Position 1 des NavigationData object
mitk::Point3D Pos2;     // Position 2 des NavigationData object

const std::string PluginProject::VIEW_ID = "org.mitk.views.pluginproject";

void PluginProject::SetFocus()
{
  m_Controls.buttonPerformImageProcessing->setFocus();
  m_Controls.buttonTracking->setFocus();
}

void PluginProject::CreateQtPartControl(QWidget *parent)
{
  // create GUI widgets from the Qt Designer's .ui file
  m_Controls.setupUi(parent);
  connect(m_Controls.buttonPerformImageProcessing, &QPushButton::clicked, this, &PluginProject::DoImageProcessing);
  connect(m_Controls.buttonTracking, &QPushButton::clicked, this, &PluginProject::DoTracking);
  m_Controls.YourWidget;
}

void PluginProject::OnSelectionChanged(berry::IWorkbenchPart::Pointer /*source*/,
                                                const QList<mitk::DataNode::Pointer> &nodes)
{
  // iterate all selected objects, adjust warning visibility
  foreach (mitk::DataNode::Pointer node, nodes)
  {
    if (node.IsNotNull() && dynamic_cast<mitk::Image *>(node->GetData()))
    {
      m_Controls.labelWarning->setVisible(false);
      m_Controls.buttonPerformImageProcessing->setEnabled(true);
      return;
    }
  }

  m_Controls.labelWarning->setVisible(true);
  m_Controls.buttonPerformImageProcessing->setEnabled(false);
}

void PluginProject::DoImageProcessing()
{
  QList<mitk::DataNode::Pointer> nodes = this->GetDataManagerSelection();
  if (nodes.empty())
    return;

  mitk::DataNode *node = nodes.front();

  if (!node)
  {
    // Nothing selected. Inform the user and return
    QMessageBox::information(nullptr, "Template", "Please load and select an image before starting image processing.");
    return;
  }

  // here we have a valid mitk::DataNode

  // a node itself is not very useful, we need its data item (the image)
  mitk::BaseData *data = node->GetData();
  if (data)
  {
    // test if this data item is an image or not (could also be a surface or something totally different)
    mitk::Image *image = dynamic_cast<mitk::Image *>(data);
    if (image)
    {
      std::stringstream message;
      std::string name;
      message << "Performing image processing for image ";
      if (node->GetName(name))
      {
        // a property called "name" was found for this DataNode
        message << "'" << name << "'";
      }
      message << ".";
      MITK_INFO << message.str();

      // actually do something here...
    }
  }
}
// Methode zur Distanzmessung zwischen zwei Punkten/Positionen
void PluginProject::DoTracking()
{
  // Abfrage ob Position 1
  if (startvalues[0] == 0)
  {
    // Speichern der Position
    Pos1 = m_Controls.YourWidget->GetSelectedNavigationDataSource()->GetOutput(0)->GetPosition();

    // ueberpruefung auf Konsole
    cout << "Pos1 is set       ";
    cout << Pos1;
    startvalues[0] = 1; // damit If else weiß ob schon eine Position gespeichert wurde

    // Speichern der x-, y-, und z-Werte
    a[0] = Pos1.GetElement(0);
    a[1] = Pos1.GetElement(1);
    a[2] = Pos1.GetElement(2);

	//Aenderung des Button Textes
    m_Controls.buttonTracking->setText("press again to set second point");
  }

  else
  {
    // Position erhalten
    Pos2 = m_Controls.YourWidget->GetSelectedNavigationDataSource()->GetOutput(0)->GetPosition();

    // Zur ueberprüfung auf Konsole
    cout << ("Pos2 is set       ");
    cout << Pos2;

    // Speichern der x-, y- und z-Werte
    b[0] = Pos2.GetElement(0);
    b[1] = Pos2.GetElement(1);
    b[2] = Pos2.GetElement(2);

    // Variante 2 über die bereits implementierte Methode
    // double distance2;
    // distance2 = Pos1.SquaredEuclideanDistanceTo(Pos2);
    // std::cout << "   distance eucl    ";
    // std::cout << distance2;

    // Berechnung des Abstandes nach der Formel für squared euclidean distance
    double P1 = a[0] * a[0] - 2 * a[0] * b[0] + b[0] * b[0];
    double P2 = a[1] * a[1] - 2 * a[1] * b[1] + b[1] * b[1];
    double P3 = a[2] * a[2] - 2 * a[2] * b[2] + b[2] * b[2];

    double zwischenergebnis = P1 + P2 + P3;
    double distance = sqrt(zwischenergebnis);

    // Ergebnisausgabe auf Konsole
    std::cout << "    distanz:    ";
    std::cout << distance;

    // Ergebnisausgabe auf Label
    std::string distancestr = std::to_string(distance);
    QString qstr = QString::fromStdString(distancestr);
    m_Controls.labelDistance->setText("Abstand: " + qstr + " mm");

    startvalues[0] = 0; // damit die nächste Positionsmessung wieder in Pos1 gespeichert wird

	//Aenderung des Button textes
    m_Controls.buttonTracking->setText("press to set the first point for distance measurement");

    // VISUALISIERUNG
    // PointSet erstellen und 3D Punkt hinzufügen
    mitk::Point3D myPoint(Pos1);
    mitk::Point3D myPoint2(Pos2);
    mitk::PointSet::Pointer myPointSet = mitk::PointSet::New();
    myPointSet->InsertPoint(myPoint);
    myPointSet->InsertPoint(myPoint2);

    // DataNode erstellen und PointSet als Daten setzen
    mitk::DataNode::Pointer myDataNode = mitk::DataNode::New();
    myDataNode->SetName("MyNode");
    myDataNode->SetData(myPointSet);

    // Render Window
    mitk::StandaloneDataStorage::Pointer dataStorage = mitk::StandaloneDataStorage::New();
    mitk::RenderWindow::Pointer renderWindow = mitk::RenderWindow::New();

    // DataNode der DataStorage hinzufügen
    this->GetDataStorage()->Add(myDataNode);
    dataStorage->Add(myDataNode);

    // 3D renderwindow
    renderWindow->GetRenderer()->SetMapperID(mitk::BaseRenderer::Standard3D);
    renderWindow->GetRenderer()->SetDataStorage(dataStorage);

    // Update render window
    renderWindow->GetVtkRenderWindow()->Render();
    mitk::RenderingManager::GetInstance()->RequestUpdateAll();

    auto timeGeometry = dataStorage->ComputeBoundingGeometry3D(dataStorage->GetAll());
    mitk::BaseGeometry::Pointer geometry = timeGeometry->GetGeometryForTimeStep(0);
    mitk::RenderingManager::GetInstance()->InitializeViews(geometry);
  }
}
