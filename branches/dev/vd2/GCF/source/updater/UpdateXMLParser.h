/****************************************************************************
**
** Copyright (C) VCreate Logic Private Limited, Bangalore
**
** Use of this file is limited according to the terms specified by
** VCreate Logic Private Limited, Bangalore.  Details of those terms
** are listed in licence.txt included as part of the distribution package
** of this file. This file may not be distributed without including the
** licence.txt file.
**
** Contact info@vcreatelogic.com if any conditions of this licensing are
** not clear to you.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef UPDATE_XML_PARSER_H
#define UPDATE_XML_PARSER_H

#include "UpdateInfo.h"

class QUrl;

namespace GCF
{

    struct UpdateXMLParserData;
    class UpdateXMLParser
    {
    public:
        UpdateXMLParser();
        ~UpdateXMLParser();

        bool parse(const QUrl& updateXmlUrl, UpdateInfoList& uiList);
        bool parse(const QString& fileName, UpdateInfoList& uiList, bool urlsAreAbsolute=false);

    private:
        UpdateXMLParserData* d;
    };

}

#endif
