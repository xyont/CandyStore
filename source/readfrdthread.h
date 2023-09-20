#ifndef READFRDTHREAD_H
#define READFRDTHREAD_H

#include <QObject>
#include <QThread>
#include "femodel.h"

class readFrdMeshThread : public QThread
 {
     Q_OBJECT

 public:
     readFrdMeshThread();
     readFrdMeshThread(const QString& filename, femodel*);
     void run();
     void setFilename(const QString& filename);
     void setFeData(femodel*);
 private:
     QString fileName;
     femodel* fedata;
 };
///////////////////////////////////////////////////
class readFrdResultInfoThread : public QThread
 {
     Q_OBJECT

 public:
     readFrdResultInfoThread();
     readFrdResultInfoThread(const QString& filename, femodel*);
     void run();
     void setFilename(const QString& filename);
     void setFeData(femodel*);
 private:
     QString fileName;
     femodel* fedata;
 };
///////////////////////////////////////////////////
class readFrdResultDataThread : public QThread
 {
     Q_OBJECT

 public:
     readFrdResultDataThread();
     readFrdResultDataThread(const QString& filename, femodel*);
     void run();
     void setFilename(const QString& filename);
     void setFeData(femodel*);
 private:
     QString fileName;
     femodel* fedata;
 };

#endif // READFRDTHREAD_H
