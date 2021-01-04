project(org_mitk_gui_qt_PluginProject)

mitk_create_plugin(
  EXPORT_DIRECTIVE PLUGINPROJECT_EXPORT
  EXPORTED_INCLUDE_SUFFIXES src
  MODULE_DEPENDS MitkQtWidgetsExt MitkIGT MitkIGTUI
)
