/*
 * Copyright (C) 2019-2020 Ashar Khan <ashar786khan@gmail.com>
 *
 * This file is part of CP Editor.
 *
 * CP Editor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * I will not be responsible if CP Editor behaves in unexpected way and
 * causes your ratings to go down and or lose any important contest.
 *
 * Believe Software is "Software" and it isn't immune to bugs.
 *
 */

#include "Widgets/SupportUsDialog.hpp"
#include "Core/EventLogger.hpp"
#include "Core/Translator.hpp"
#include "Util/FileUtil.hpp"
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QPushButton>
#include <QTextBrowser>
#include <QVBoxLayout>

SupportUsDialog::SupportUsDialog(QWidget *parent)
{
    setModal(true);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(tr("Like CP Editor?"));

    auto *mainLayout = new QVBoxLayout(this);

    textBrowser = new QTextBrowser();
    mainLayout->addWidget(textBrowser);

    textBrowser->setOpenExternalLinks(true);
    textBrowser->setMarkdown(
        QString(R"(%1
-   [![](:/donate/star.png)](https://github.com/cpeditor/cpeditor/stargazers) %2
    <li>
        <a href="https://twitter.com/intent/tweet?text=&hashtags=CPEditor,CompetitiveProgramming&url=https://cpeditor.org&related=cpeditor_">
            <img src=":/donate/tweet.png"/>
        </a>
        %3
    </li>
    <li><a href="#donate"> <img src=":/donate/sponsor.png"/></a>%4</li>
</p>
<p>
%5
</p>)")
            .arg(tr("Thanks for using CP Editor! Would you mind:"))
            .arg(tr("Give us a star on GitHub"))
            .arg(tr("Share CP Editor with your friends"))
            .arg(tr("Financially support us"))
            .arg(tr("Or, <a href='https://github.com/cpeditor/cpeditor/issues/new/choose'>provide some suggestions</a> "
                    "to help us do better."))
            // this must be the last one, because the percent encoding includes %1, %2, etc., which breaks QString::arg
            .replace("?text=", "?text=" + QString::fromUtf8(QUrl::toPercentEncoding(
                                              tr("I'm using @cpeditor_, the IDE specially designed for competitive "
                                                 "programmers, which is awesome!")))));

    connect(textBrowser, &QTextBrowser::anchorClicked, this, &SupportUsDialog::onAnchorClicked);

    setMinimumHeight(textBrowser->height());
    setMinimumWidth(textBrowser->width());

    auto *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);
    mainLayout->addWidget(buttonBox);
    connect(buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &QDialog::accept);
}

void SupportUsDialog::onAnchorClicked(const QUrl &url)
{
    if (url.toString() == "#donate")
    {
        auto *dialog = new QMessageBox(this);
        dialog->setTextFormat(Qt::MarkdownText);
        dialog->setAttribute(Qt::WA_DeleteOnClose);
        dialog->setModal(true);
        dialog->setWindowTitle(tr("Support us"));
        dialog->setText(
            Util::readFile(QString(":/DONATE%1.md").arg(Core::Translator::langSuffix())).replace("resources/", ":/"));
        dialog->show();
    }
}
