/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Link�ping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Erik Sund�n
 *
 **********************************************************************/

#include <inviwo/qt/editor/settingswidget.h>
#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/qt/widgets/properties/propertywidgetfactoryqt.h>
#include <QLayout>
#include <QFrame>
#include <QSettings>

namespace inviwo {

SettingsWidget::SettingsWidget(QString title, QWidget* parent) : InviwoDockWidget(title, parent) {
    generateWidget();
}

SettingsWidget::SettingsWidget(QWidget* parent) : InviwoDockWidget(tr("Settings"), parent) {
    generateWidget();
}

void SettingsWidget::generateWidget() {
    setObjectName("SettingsWidget");
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    tabWidget_ = new QTabWidget();    
    setWidget(tabWidget_);
    //setWidget(frame);
}

SettingsWidget::~SettingsWidget() {}

//Load settings from QSettings
void SettingsWidget::loadSettings() {
    std::vector<Settings*> settings = InviwoApplication::getPtr()->getModuleSettings();
    QSettings qmainsettings("Inviwo", "Inviwo");

    for (size_t i=0; i<settings.size(); i++) {
       
        QVBoxLayout* vLayout =  new QVBoxLayout();
        vLayout->setSpacing(0);
        QWidget* tab = new QWidget();
        tab->setLayout(vLayout);        

        std::vector<Property*> properties = settings[i]->getProperties();        
        qmainsettings.beginGroup(tr(settings[i]->getIdentifier().c_str()));
        QStringList keys = qmainsettings.allKeys();
        for (size_t j=0; j<properties.size(); j++) {
            Property* curProperty = properties[j];
            QString name = QString::fromStdString(curProperty->getIdentifier());
            if(keys.contains(name)){
                QVariant qval = qmainsettings.value(name);
                Variant val(std::string(qval.toString().toLocal8Bit().constData()));
                curProperty->setVariant(val);
            }

            PropertyWidgetQt* propertyWidget = PropertyWidgetFactoryQt::getRef().create(curProperty);
            vLayout->addWidget(propertyWidget);
            curProperty->registerWidget(propertyWidget);
        }
        qmainsettings.endGroup();

        tabWidget_->addTab(tab, tr(settings[i]->getIdentifier().c_str()) );

        vLayout->addStretch(0);
    }

}

//Save application settings to QSettings
void SettingsWidget::saveSettings() {
    const std::vector<Settings*> settings = InviwoApplication::getRef().getModuleSettings();
    QSettings qmainsettings("Inviwo", "Inviwo");
    for (size_t i=0; i<settings.size(); i++) {
        std::vector<Property*> properties = settings[i]->getProperties();
        qmainsettings.beginGroup(tr(settings[i]->getIdentifier().c_str()));
        for (size_t j=0; j<properties.size(); j++) {
            Property* curProperty = properties[j];
            qmainsettings.setValue(QString::fromStdString(curProperty->getIdentifier()), QVariant(QString::fromStdString(curProperty->getVariant().getString())));
        }
        qmainsettings.endGroup();
    }
}

} // namespace