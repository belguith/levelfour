TEMPLATE    = subdirs
CONFIG     += ordered
TARGET      = Components

SUBDIRS     = LevelFour \
		  VisSystemCanvas \
              VisSystemNodeClassExplorer \
		  TemplateManager \
              PropertyEditor \
              Utils \
              ProjectManager \
              MessageLog \
              ScriptEditor \
              MainWindow \
              HelpSystem \
		  ScenegraphManager \
		  Clipboard \
		  OsgCore \
		  OsgVis \
		  OsgFX \
		  OsgShadow \
		  OsgParticle \
		  OsgText \
		  AnimationManager \
              ViewportComponent \
              D2M
              
           
# Some components are under development, but the ideas and implementation
# is not yet clear for public view/modification. Until that time, 
# we bracket the code like this.
# exists(VInfoSpace/VInfoSpace.pro)
# {
# SUBDIRS    += VInfoSpace
# }

SUBDIRS += DummyCodeGen

