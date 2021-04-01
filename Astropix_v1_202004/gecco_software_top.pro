TEMPLATE = subdirs

SUBDIRS = \
        ./Apix_v1_Software \   # relative paths
        ./qthttpserver

# where to find the sub projects - give the folders
qthttpserver.subdir = ./qthttpserver
app.subdir  = ./Apix_v1_Software

app.depends = qthttpserver


