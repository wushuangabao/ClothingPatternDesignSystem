#ifndef MYPATTERN_H
#define MYPATTERN_H

#include <QList>

class MyPathData;

class MyPattern{
public:
    MyPattern(QList<MyPathData*> paths);
    ~MyPattern();

    //void mergePathData(MyPathData* path, QString astmTag = "");
    //void takeApartPath(QList<int> pathIds, MyPathData* objMerge = nullptr);
    //void deletePath(int pathId);
    void setWarpLine(QPointF p1, QPointF p2, int pathId = 0);
    bool findWarpLine(int pathId, QPointF* p1 = nullptr, QPointF* p2 = nullptr);
    bool writeASTM(QString filePath);

    QString infoName;   /**< 名称信息 */
    QString infoSize;   /**< 号型信息 */
    QString infoAuthor; /**< 作者信息 */    
    QList<MyPathData*> pathList; /**< MyPathData列表 */
};

#endif // MYPATTERN_H
