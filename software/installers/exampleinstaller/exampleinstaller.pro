#deactivate default app build behaviour
TEMPLATE = aux

#default path to the packages
PACKAGES_PATH += $$PWD/packages

#add config and packages files to the project for easier editing
OTHER_FILES += $$files($$PACKAGES_PATH/*, true)
OTHER_FILES += config.xml

#configure to use the installer framework
CONFIG += qtifw libs
