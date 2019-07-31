#include "mypattern.h"
#include "../data/mypathdata.h"
#include <QDateTime>

MyPattern::MyPattern(QList<MyPathData*> paths)
{
    infoName = "未命名样板";
    infoAuthor = "wushuangBaOY_WHA";
    infoSize = "M";
    pathList = paths;
}

MyPattern::~MyPattern()
{
//    int size = pathList.size();
//    if(size > 0){
//        for(int i=0; i<size; ++i)
//            if(pathList[i] != nullptr)
//                delete pathList[i];
//        pathList.clear();
//    }
}

/**
 * @brief 设置经向线
 * @param p1
 * @param p2
 * @param pathId
 */
void MyPattern::setWarpLine(QPointF p1, QPointF p2, int pathId)
{
    if(pathList.size() < pathId+1)
        return;
    if(!findWarpLine(pathId)){
        // 将p1连接p2的直线加入pathList[pathId]并设置为经向线。
        int id = pathList[pathId]->numberPath;
        pathList[pathId]->addLine(p1, p2);
        pathList[pathId]->setWarpLine(id);
    }else{
        // todo: 提示,已经存在经向线。
    }
}

/**
 * @brief 找到pathList[pathId]的经向线
 * @param pathId
 * @param p1
 * @param p2
 * @return bool 是否找到
 */
bool MyPattern::findWarpLine(int pathId, QPointF *p1, QPointF *p2)
{
    if(pathId < 0 || pathId >= pathList.size())
        return false;
    int idWarp = pathList[pathId]->warpLineId();
    if(idWarp > 0){
        int i1 = pathList[pathId]->pathData[idWarp].startPoint->id,
            i2 = pathList[pathId]->pathData[idWarp].endPoint->id;
        if(p1 != nullptr && p1 != nullptr){
            *p1 = pathList[pathId]->pointData[i1];
            *p2 = pathList[pathId]->pointData[i2];
        }
        return true;
    }
    else return false;
}

/**
 * @brief 输出为ASTM标准的dxf文件
 * @param filePath
 * @return
 */
bool MyPattern::writeASTM(QString filePath)
{
    QFile file(filePath);
    file.open(QIODevice::ReadWrite | QIODevice::Text);

    QString content = "999\nANSI/AAMA\n0\nSECTION\n2\nBLOCKS\n";
    // 遍历多个样片
    foreach(MyPathData* path, pathList){
        path->writeASTMBlock(&file);
    }
    content="  0\nSECTION\n  2\nENTITIES\n  0\nINSERT\n  8\n1\n  2\n-M\n"+MyPathData::stringPoint(0.0,0.0);
    file.write(content.toUtf8());
    MyPathData::writeText(&file,"Style Name: style");
    content=QDateTime::currentDateTime().toString("MM-dd-yyyy");
    MyPathData::writeText(&file,"Creation Date: "+content);
    MyPathData::writeText(&file,"Author: "+infoAuthor);
    MyPathData::writeText(&file,"Sample Size: "+infoSize);
    MyPathData::writeText(&file,"Grading Rule Table: "+infoName);
    MyPathData::writeText(&file,"Units: METRIC");
    content="  0\nENDSEC\n  0\nEOF";

    file.close();
    return true;
}

