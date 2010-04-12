/*
   Copyright 2005-2009 Last.fm Ltd. 
      - Primarily authored by Max Howell, Jono Cole and Doug Mansell

   This file is part of the Last.fm Desktop Application Suite.

   lastfm-desktop is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   lastfm-desktop is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with lastfm-desktop.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "LoginDialog.h"
#include "lib/unicorn/QMessageBoxBuilder.h"
#include <lastfm/misc.h>
#include <lastfm/XmlQuery>
#include <lastfm/ws.h>
#include <QtGui>
#include <QDesktopServices>

#ifdef WIN32
#include <windows.h>
#endif


LoginDialog::LoginDialog( QWidget* parent )
            :QDialog( parent )
{
    setWindowModality( Qt::ApplicationModal );

    QVBoxLayout* layout = new QVBoxLayout( this );

    layout->addWidget( ui.title = new QLabel( tr("Last.fm needs your permission first!") ) );
    ui.title->setObjectName( "title" );

    layout->addWidget( ui.description = new QLabel( tr("This application needs your permission to connect to your Last.fm profile.  Click OK to go to the Last.fm website and do this.") ) );
    ui.description->setWordWrap( true );
    ui.title->setObjectName( "description" );
    ui.title->setWordWrap( true );

    layout->addWidget( ui.buttonBox = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel ) );

    connect( ui.buttonBox, SIGNAL(accepted()), SLOT(authenticate()) );
    connect( ui.buttonBox, SIGNAL(rejected()), SLOT(reject()) );
}


void
LoginDialog::authenticate()
{
    connect( unicorn::Session::getToken(), SIGNAL(finished()), SLOT(onGotToken()) );
}


void
LoginDialog::cancel()
{
    qDeleteAll( findChildren<QNetworkReply*>() );
}


void
LoginDialog::onGotToken()
{
    lastfm::XmlQuery lfm = lastfm::XmlQuery( lastfm::ws::parse( static_cast<QNetworkReply*>(sender()) ) );

    m_token = lfm["token"].text();

    QUrl authUrl( "http://www.last.fm/api/auth/" );
    authUrl.addQueryItem( "api_key", lastfm::ws::ApiKey );
    authUrl.addQueryItem( "token", m_token );

    if ( QDesktopServices::openUrl( authUrl ) )
    {
        // prepare for continue to be clicked
        accept();
    }
    else
    {
        // We were unable to open a browser - what do we do now?
        reject(); // ???
    }
}