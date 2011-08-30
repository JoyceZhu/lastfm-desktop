#include <QEvent>
#include <QResizeEvent>
#include <QPainter>
#include <QToolTip>
#include <QUrl>

#include "Label.h"

Label::Label( QWidget* parent )
    :QLabel( parent ), m_linkColor( Qt::black )
{
    setAttribute( Qt::WA_LayoutUsesWidgetRect );
    setOpenExternalLinks( true );

    connect( this, SIGNAL(linkHovered(QString)), SLOT(onHovered(QString)));
}


Label::Label( const QString& text, QWidget* parent )
    :QLabel( parent )
{
    setText( text );
    setAttribute( Qt::WA_LayoutUsesWidgetRect );
    setOpenExternalLinks( true );

    connect( this, SIGNAL(linkHovered(QString)), SLOT(onHovered(QString)));
}

void
Label::onHovered( const QString& url )
{   
    QUrl displayUrl( url );
    QToolTip::showText( cursor().pos(), displayUrl.toString(), this, QRect() );
}

QString
Label::boldLinkStyle( const QString& text, QColor linkColor )
{
    return QString( "<html><head><style type=text/css>"
                     "a:link {color:%1; font-weight: bold; text-decoration:none;}"
                     "a:hover {color:%1; font-weight: bold; text-decoration:none;}"
                     "</style></head><body>%2</body></html>" ).arg( linkColor.name(), text );
}

QString
Label::boldLinkStyle( const QString& text )
{
    return boldLinkStyle( text, m_linkColor );
}

void
Label::setText( const QString& text )
{
    m_text = text;

    if ( textFormat() == Qt::RichText )
        QLabel::setText( boldLinkStyle( m_text ) );
    else
        QLabel::setText( ""  );

    update();
}

void
Label::setLinkColor( QColor linkColor )
{
    m_linkColor = linkColor;
}

QString
Label::anchor( const QString& url, const QString& text )
{
    return QString( "<a href=\"%1\">%2</a>" ).arg( url, text );
}

void
Label::paintEvent( QPaintEvent* event )
{
    if ( textFormat() == Qt::RichText )
        QLabel::paintEvent( event );
    else
    {
        QFrame::paintEvent(event);
        QPainter p(this);
        p.drawText( rect(), fontMetrics().elidedText( m_text, Qt::ElideRight, contentsRect().width() ) );
    }
}


