/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013-2014 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Main file authors: Alexander Johansson, Sathish Kottravel
 *
 *********************************************************************************/

#include <inviwo/qt/widgets/properties/optionpropertywidgetqt.h>
#include <QComboBox>
#include <typeinfo>
namespace inviwo {
OptionPropertyWidgetQt::OptionPropertyWidgetQt(BaseOptionProperty* property) 
    : PropertyWidgetQt(property)
    , property_(property) {
    generateWidget();
    updateFromProperty();
}

void OptionPropertyWidgetQt::generateWidget() {
    QHBoxLayout* hLayout = new QHBoxLayout();
    comboBox_ = new QComboBox();
    updateFromProperty();

    comboBox_->setEnabled(!property_->getReadOnly());
    comboBox_->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(comboBox_,
            SIGNAL(customContextMenuRequested(const QPoint&)),
            this,
            SLOT(showContextMenu(const QPoint&)));
    
    label_ = new EditableLabelQt(this, property_->getDisplayName());
    
    hLayout->addWidget(label_);
    hLayout->addWidget(comboBox_);
    hLayout->setContentsMargins(0, 0, 0, 0);
    hLayout->setSpacing(0);
    hLayout->setStretch(1,1);
    setLayout(hLayout);
    connect(comboBox_, SIGNAL(currentIndexChanged(int)),this, SLOT(optionChanged()));
    connect(label_, SIGNAL(textChanged()),this, SLOT(setPropertyDisplayName()));   
}

void OptionPropertyWidgetQt::optionChanged() {
    if (comboBox_->count()
        && comboBox_->currentIndex() >= 0
        && comboBox_->currentIndex() != property_->getSelectedIndex()) {
        property_->setInitiatingWidget(this);
        property_->setSelectedIndex(comboBox_->currentIndex());
        property_->clearInitiatingWidget();
    }
}

void OptionPropertyWidgetQt::updateFromProperty() {
    comboBox_->blockSignals(true);
    comboBox_->clear();
    std::vector<std::string> names = property_->getDisplayNames();

    for (size_t i = 0; i < names.size(); i++) {
        QString option = QString::fromStdString(names[i]);
        comboBox_->addItem(option);
    }
    comboBox_->setCurrentIndex(static_cast<int>(property_->getSelectedIndex()));
    comboBox_->blockSignals(false);
}

void OptionPropertyWidgetQt::setPropertyDisplayName() {
    property_->setDisplayName(label_->getText());
}

} // namespace
