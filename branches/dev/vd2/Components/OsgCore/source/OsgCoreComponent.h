/****************************************************************************
**
** Copyright (C) UNO
**
** Use of this file is limited according to the terms specified by
** UNO.
**
** Details of those terms are listed in licence.txt included as
** part of the distribution package of this file. This file may not
** be distributed without including the licence.txt file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef OSGCORE_COMPONENT_H
#define OSGCORE_COMPONENT_H

#include <GCF/AbstractComponent>
#include <GCF/IAboutData>
#include "IVisSystemNodeFactory.h"
#include <QTabWidget>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QComboBox>
#include "OsgSimpleViewVisNode.h"
#include "ViewportPanel.h"
#include "ViewportsSplitter.h"

struct OsgCoreComponentData;
class OsgCoreComponent : public GCF::AbstractComponent,
                                 virtual public IVisSystemNodeFactory,
                                 virtual public IAboutData
{
    Q_OBJECT
    Q_INTERFACES(IVisSystemNodeFactory IAboutData)
    GCF_DECLARE_COMPONENT(OsgCoreComponent)

public:
    static OsgCoreComponent & instance();
    ~OsgCoreComponent();

	static QIcon nodeIcon();
	
	ViewportsSplitter * sceneView() const;

    // IContainer implementation
    QObject* containerObject();

    // IVisSystemNodeFactory implementation
    QStringList nodeClassNameList();
    IVisSystemNodeDesc* nodeDesc(QString nodeClassName);
    bool canCreate(QString nodeClassName);
    IVisSystemNode* createNode(QString nodeClassName);
    void  deleteNode(IVisSystemNode* node);

    // IAboutData implementation
    QString productName() const;
    QString organization() const;
    QImage programLogo() const;
    QString version() const;
    QString shortDescription() const;
    QString homepage() const;
    QString bugAddress() const;
    const QList<GCF::AboutPerson> authors() const;
    const QList<GCF::AboutPerson> credits() const;
    QString license() const;
    QString copyrightStatement() const;

protected:
    OsgCoreComponent();
    void initializeComponent();
    void finalizeComponent();
    void creationAnnounced();
    QObject* fetchObject(const QString& completeName) const;
    QWidget* fetchWidget(const QString& completeName) const;
    void componentDestroyed(GCF::AbstractComponent* comp);

private:
    OsgCoreComponentData* d;
};

#endif

