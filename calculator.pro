TEMPLATE = subdirs
CONFIG += ordered

# Build order follows SUBDIRS order: core library, plugin DLLs, executable.
SUBDIRS += core plugins app

core.subdir = src/core
plugins.subdir = src/plugins
app.subdir = src/app
