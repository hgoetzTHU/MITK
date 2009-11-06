SET(MODULE_TESTS 
  mitkAutoCropImageFilterTest.cpp
  mitkContourMapper2DTest.cpp
  mitkContourTest.cpp
  mitkDemonsRegistrationTest.cpp
  mitkExternalToolsTest.cpp
  mitkHistogramMatchingTest.cpp
  mitkMeshTest.cpp
  mitkOrganTypePropertyTest.cpp
  mitkPipelineSmartPointerCorrectnessTest.cpp
  mitkPlaneFitTest.cpp
  mitkPointLocatorTest.cpp
  # mitkRigidRegistrationPresetTest.cpp
  # mitkRigidRegistrationTestPresetTest.cpp
  # mitkSegmentationInterpolationTest.cpp
  mitkSymmetricForcesDemonsRegistrationTest.cpp
  # mitkTestTemplate.cpp
  mitkToolManagerTest.cpp
)
SET(MODULE_IMAGE_TESTS
  mitkUnstructuredGridVtkWriterTest.cpp
  mitkCompressedImageContainerTest.cpp  
  mitkCylindricToCartesianFilterTest.cpp
  mitkExtractImageFilterTest.cpp  
  mitkManualSegmentationToSurfaceFilterTest.cpp
  mitkOverwriteSliceImageFilterTest.cpp  
  mitkSurfaceToImageFilterTest.cpp
)
SET(MODULE_CUSTOM_TESTS 
  mitkLabeledImageToSurfaceFilterTest.cpp
)
SET(MODULE_TESTIMAGES
  US4DCyl.pic.gz
  Pic3D.pic.gz
  Pic2DplusT.pic.gz
  BallBinary30x30x30.pic.gz
  Png2D-bw.png
  binary.stl
  ball.stl
)
