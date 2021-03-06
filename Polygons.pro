QT        += opengl


HEADERS   += mainwindow.h \
             mainwidget.h \
             glwidget.h \
             Geometry/tools.h \
             Geometry/vector.h \
             Geometry/point.h \
             Geometry/polygon.h \
             Geometry/setofpolygons.h \


SOURCES   += main.cpp \
             mainwindow.cpp \
             mainwidget.cpp \
             glwidget.cpp \
             Geometry/tools.cpp \
             Geometry/vector.cpp \
             Geometry/point.cpp \
             Geometry/polygon.cpp \
             Geometry/setofpolygons.cpp


RESOURCES += Resources/Icons/Icons.qrc

RC_FILE    = myapp.rc
